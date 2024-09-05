//
//  Player.hpp
//  
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#ifndef Player_h
#define Player_h
#include "Board.h"
#include "Side.h"
#include <string>
using namespace std;

class Player
{ 
    public:
        Player(string name);
        string name() const;
        virtual bool isInteractive() const = 0;
        virtual int chooseMove(const Board& b, Side s) const = 0;
        virtual ~Player(){}
        
    private:
        string m_name;
    
};


class HumanPlayer : public Player
{
    public:
        HumanPlayer(string name):Player(name)
        {}
        virtual bool isInteractive() const;
        virtual int chooseMove(const Board& b, Side s) const;
        virtual ~HumanPlayer(){};
};


class BadPlayer : public Player
{
    public:
        BadPlayer(string name):Player(name)
        {}
        virtual bool isInteractive() const;
        virtual int chooseMove(const Board& b, Side s) const;
        virtual ~BadPlayer(){};
};


class SmartPlayer : public Player
{
    public:
        SmartPlayer(string name):Player(name)
        {}
        virtual bool isInteractive() const;
        virtual int chooseMove(const Board& b, Side s) const;
        virtual ~SmartPlayer(){};
    private:
        void chooseMove(const Board& b, Side s, int & bestHole, int& value, int depth) const;
        int evaluateMove(const Board& b) const;
        bool completeMove(Side s, int hole, Board &b, Side& endSide, int& endHole) const;

};
 

#endif /* Player_h */

