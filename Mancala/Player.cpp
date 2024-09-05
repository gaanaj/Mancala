//
//  Player.cpp
//  
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#include <iostream>
#include "Player.h"
using namespace std;
#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

Timer timer;

Player::Player(string name)
{
    m_name = name;
}

string Player::name()const
{
    return m_name;
}


bool HumanPlayer::isInteractive() const
{
    return true;
}

bool BadPlayer::isInteractive() const
{
    return false;
}


bool SmartPlayer::isInteractive() const
{
    return false;
}


int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    //check if any beans available on side s
    if(b.beansInPlay(s) == 0)
    {
        return -1;
    }
    
    int hole = -1;
    while(b.beans(s, hole) == -1 || b.beans(s, hole) == 0 )
    {
        //cout << "beans in hole:" << b.beans(s, hole) << "hole:" << hole<< endl;
        cout << "Please select a hole " << name() << ": ";
        cin >> hole;
    }
    return hole;
}


int BadPlayer::chooseMove(const Board& b, Side s) const
{
    //check if any beans available on side s
    if(b.beansInPlay(s) == 0)
    {
        return -1;
    }
    
    //return left most hole with beans
    int hole = 1;
    while(b.beans(s, hole) == -1 || b.beans(s, hole) == 0 )
    {
        hole++;
    }
    return hole;
}


/*if no move for player exists (i.e., game is over),
bestHole = -1
value = value of this position (i.e., +∞, -∞, or 0)
return
 
if the criterion says we should not search below this node
bestHole = -1
value = value of this position
return
 
for every hole h the player can choose
"make" the move h
chooseMove(opponent, board, h2, v2)
"unmake" the move
if v2 is better for the player than best seen so far,
    bestHole = h
    value = v2
return
*/


int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int bestHole, value;
    int depth = 0;
    //Timer timer;
    timer.start();
    chooseMove(b, s, bestHole, value, depth);
    //cout<<timer.elapsed()<<endl;
    return bestHole;
    
}


bool SmartPlayer::completeMove(Side s, int hole, Board &b, Side& endSide, int& endHole) const {
    
    if (b.sow(s, hole, endSide, endHole))
    {
        if (endHole == 0)
        {
            return false;
        }
        else if (endSide == s)
        {
            //capture
            if (b.beans(s, endHole) == 1 && b.beans(opponent(s), endHole) > 0)
            {
                b.moveToPot(opponent(s), endHole, s);
                b.moveToPot(s, endHole, s);
            }
        }
        return true;
    }
    return false;
}


void SmartPlayer::chooseMove(const Board& b, Side s, int& bestHole, int& value, int depth) const
{
    if (timer.elapsed() > 4900)
    {
        //time limit of 5 seconds reached
            bestHole = -1;
            value = evaluateMove(b);
            return;
    }
    
    if (depth > 5) {
        bestHole = -1;
        value = evaluateMove(b);
        return;
    }
    
    if (b.beansInPlay(s) == 0) {
        bestHole = -1;
        value = evaluateMove(b);
        return;
    }
    
    int first = 1; //first hole that would return a value
    for (int i = 1; i <= b.holes(); i++)
    {
        //invalid move
        if (b.beans(s, i) == 0)
        {
            first++;
            continue;
        }
        
        Board temp(b);
        Side endSide;
        int endHole, val2, hole2, hole3;
        
        if (completeMove(s, i, temp, endSide, endHole))
        {
            //still opponents turn so increase depth
            chooseMove(temp, opponent(s), hole2, val2, depth + 1);
        }
        else
        {
            //still the same player's turn so depth doesn't change
            chooseMove(temp, s, hole3, val2, depth);
        }
        
        if (i == first)
        {
            //set value and besthole to be that of the first move made, to ensure that the player would choose a legal move
            value = val2;
            bestHole = i;
        }
        
        if (s == SOUTH) {
            if (val2 > value)
            {
                //greater num is better for south
                value = val2;
                bestHole = i;
            }
        }
        else { //side is north
            if (val2 < value)
            {
                //smaller num is better for north
                value = val2;
                bestHole = i;
            }
        }
    }
    return;
}

int SmartPlayer::evaluateMove(const Board& b) const
{
    int northBeans = b.beansInPlay(NORTH);
    int southBeans = b.beansInPlay(SOUTH);
    int nPot = b.beans(NORTH, 0);
    int sPot = b.beans(SOUTH, 0);
    //game is over because one of the players cant move
    if (southBeans == 0 || northBeans == 0)
    {
        //south wins so positive infinity
        if (sPot + southBeans > nPot + northBeans)
        {
            return INT_MAX;
        }
        //north wins so return negative infinity
        else if (sPot +southBeans < nPot + northBeans)
        {
            return INT_MIN;
        }
        else
        {   //tie
            return 0;
        }
    }
    return (sPot + southBeans) - (nPot - northBeans);
}


