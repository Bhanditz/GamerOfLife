#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include "Stage.hpp"

class Player {
 private:
    char *name;
    std::map<int, Stage> Stages;
 public:
    Player() {}

};
#endif
