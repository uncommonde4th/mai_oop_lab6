#include "../include/npc.hpp"
#include "../include/squirrel.hpp"
#include "../include/bear.hpp"
#include "../include/orc.hpp"
#include "../include/fight_logic.hpp"

#include <ctime>


class TextObserver : public Observer {
public:
    void fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        if (win) {
            std::cout << "\n";
            attacker->print();
            std::cout << " --> ";
            defender->print();
        }
    }
};

class FileObserver : public Observer {
private:
    std::ofstream log_file;

public:
    FileObserver(const std::string& filename) {
        log_file.open(filename, std::ios_base::app);
    }

    void fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        if (win && log_file.is_open()) {
            log_file << *attacker << " убивает " << *defender << "\n";
        }
    }
};

// Глобальные наблюдатели
std::shared_ptr<TextObserver> text_observer = std::make_shared<TextObserver>();
std::shared_ptr<FileObserver> file_observer = std::make_shared<FileObserver>("log.txt");

// Фабрика для создания NPC из потока или по типу
std::shared_ptr<NPC> create_npc(std::istream &is) {
    std::shared_ptr<NPC> result;
    int type_code;
    if (is >> type_code) {
        switch (static_cast<NpcType>(type_code)) {
            case NpcType::SquirrelType:
                result = std::make_shared<Squirrel>(is);
                break;
            case NpcType::BearType:
                result = std::make_shared<Bear>(is);
                break;
            case NpcType::OrcType:
                result = std::make_shared<Orc>(is);
                break;
            default:
                break;
        }
    }

    if (result) {
        result->subscribe(text_observer);
        result->subscribe(file_observer);
    }

    return result;
}

std::shared_ptr<NPC> create_npc(NpcType type, int x, int y) {
    std::shared_ptr<NPC> result;

    switch (type) {
        case NpcType::SquirrelType:
            result = std::make_shared<Squirrel>(x, y);
            break;
        case NpcType::BearType:
            result = std::make_shared<Bear>(x, y);
            break;
        case NpcType::OrcType:
            result = std::make_shared<Orc>(x, y);
            break;
        default:
            break;
    }

    if (result) {
        result->subscribe(text_observer);
        result->subscribe(file_observer);
    }

    return result;
}

// Сохранение в файл
void save_npcs(const npc_set_t &npcs, const std::string &filename) {
    std::ofstream fs(filename);
    fs << npcs.size() << "\n";
    for (const auto &npc : npcs) {
        npc->save(fs);
    }
    fs.flush();
    fs.close();
}

// Загрузка из файла
npc_set_t load_npcs(const std::string &filename) {
    npc_set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open()) {
        int count;
        is >> count;

        for (int i = 0; i < count; ++i) {
            result.insert(create_npc(is));
        }

        is.close();
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const npc_set_t &npcs) {
    for (const auto &npc : npcs) {
        npc->print();
    }
    return os;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    npc_set_t npc_list;

    std::cout << "Генерация NPC...\n";
    for (size_t i = 0; i < 50; ++i) {
        int type = std::rand() % 3 + 1;
        int x = std::rand() % 501;
        int y = std::rand() % 501;
        npc_list.insert(create_npc(static_cast<NpcType>(type), x, y));
    }

    std::cout << "Сохранение в файл...\n";
    save_npcs(npc_list, "npcs.txt");

    npc_list.clear();

    std::cout << "Загрузка из файла...\n";
    npc_list = load_npcs("npcs.txt");

    std::cout << "Исходный список NPC:\n" << npc_list;

    std::cout << "\nЗапуск боевой симуляции...\n";
    for (size_t distance = 20; distance < 500 && !npc_list.empty(); distance += 10) {
        auto dead_npcs = fight(npc_list, distance);
        for (const auto &dead : dead_npcs) {
            npc_list.erase(dead);
        }
        std::cout << "Раунд: дистанция " << distance << "\n" << "Убито: " << dead_npcs.size() << "\n\n";
    }

    std::cout << "Список выживших:\n" << npc_list;

    return 0;
}
