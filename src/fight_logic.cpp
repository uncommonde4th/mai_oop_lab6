#include "../include/fight_logic.hpp"

npc_set_t fight(const npc_set_t &npcs, size_t distance) {
    npc_set_t dead_list;

    for (const auto &attacker : npcs) {
        for (const auto &defender : npcs) {
            if (attacker != defender &&
                dead_list.find(defender) == dead_list.end() &&
                attacker->is_close(defender, distance)) {

                bool success = defender->accept(attacker);
                if (success) {
                    dead_list.insert(defender);
                }
            }
        }
    }

    return dead_list;
}
