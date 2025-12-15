#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <random>
#include <fstream>
#include <math.h>


enum class NpcType {
    Unidentified = 0,
    OrcType = 1,
    SquirrelType = 2,
    BearType = 3
};

struct Observer {
    virtual void fight(const std::shared_ptr<class NPC> attacker, const std::shared_ptr<class NPC> defender, bool win) = 0;
    virtual ~Observer() = default;
};

struct NPC : public std::enable_shared_from_this<NPC> {
    NpcType type;
    int x{0};
    int y{0};
    std::vector<std::shared_ptr<Observer>> observers;

    NPC(NpcType t, int _x, int _y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<Observer> observer);
    void fight_notify(const std::shared_ptr<NPC> defender, bool win);
    bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;
    
    virtual bool accept(std::shared_ptr<NPC> visitor) = 0;

    virtual bool fight(std::shared_ptr<class Orc> other) = 0;
    virtual bool fight(std::shared_ptr<class Squirrel> other) = 0;
    virtual bool fight(std::shared_ptr<class Bear> other) = 0;

    virtual void print() = 0;
    virtual void save(std::ostream &os) const;

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};

using npc_set_t = std::set<std::shared_ptr<NPC>>;
