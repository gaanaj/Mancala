//
//  Game.hpp
// 
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#ifndef Game_h
#define Game_h
#include "Player.h"
#include "Board.h"

class Game
{
    public:
    //use member initialization list?
    Game(const Board& b, Player* south, Player* north):board1(1,1)
    {
        southPlayer = south;
        northPlayer = north;
        currentPlayer = SOUTH;
        board1 = b;
    };
    //:southPlayer(south), northPlayer(north){};
        void display() const;
        void status(bool& over, bool& hasWinner, Side& winner) const;
        bool move(Side s);
        void play();
        int beans(Side s, int hole) const;
    
    private:
        Player* southPlayer;
        Player* northPlayer;
        Side currentPlayer;
        Board board1;
        
        Player* playerType(Side s);
    
    
};


#endif /* Game_hpp */
