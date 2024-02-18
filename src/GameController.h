#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    void removeR(string &satir);

    bool IsItPowerup(BlockFall &game);
};


#endif //PA2_GAMECONTROLLER_H
