#include "../include/npc.hpp"

NPC::NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {}

NPC::NPC(NpcType t, std::istream &is) : type(t) {
    is >> x;
    is >> y;
}

void NPC::subscribe(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win) {
    for (auto &observer : observers) {
        observer->fight(shared_from_this(), defender, win);
    }
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    int dx = x - other->x;
    int dy = y - other->y;
    return (dx * dx + dy * dy) <= (distance * distance);
}

void NPC::save(std::ostream &os) const {
    os << x << " " << y << " ";
}

std::ostream &operator<<(std::ostream &os, NPC &npc) {
    os << "{x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}
