//
//  Game.cpp
// 
//
//  Created by Gaana Jakkahalli on 5/28/23.
//
#include <iostream>
#include "Game.h"
using namespace std;



void Game::display() const
{
    cout<< northPlayer->name() << endl;
    
    cout<<"  ";
    for(int i = 1; i <= board1.holes(); i++)
    {
        cout<<board1.beans(NORTH,i)<<" ";
    }
    
    cout<<endl;
    cout<<board1.beans(NORTH, 0);
    cout<< "                 ";
    cout<<board1.beans(SOUTH, 0)<<endl;
    
    cout<<"  ";
    for(int i = 1; i <= board1.holes(); i++)
    {
        cout<<board1.beans(SOUTH,i)<<" ";
    }
    cout<<endl;
    cout<< southPlayer->name() << endl;
}


//If the game is over (i.e., the move member function has been called and returned false), set over to true;otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    //move(currentPlayer) == false
    if(board1.beansInPlay(currentPlayer) == 0)
    {
        over = true;
        if(board1.beans(SOUTH, 0) > board1.beans(NORTH, 0))
        {
            hasWinner = true;
            winner = SOUTH;
        }else if(board1.beans(SOUTH, 0) < board1.beans(NORTH, 0))
        {
            hasWinner = true;
            winner = NORTH;
        }else{
            hasWinner = false;
        }
    }else{
        over = false;
    }
    
}


//Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
bool Game::move(Side s)
{
    currentPlayer = s;
     //no move can be made
    Side Opponent = returnOpponent(s);
    
     if(board1.beansInPlay(s) == 0)
     {
         //add beans in opponents hole to their pot
         for(int i = 1; i <= board1.holes(); i++)
         {
             board1.moveToPot(Opponent, i, Opponent);
         }
         display();
         return false;
     }else{
         //cout<<"there are beans on this side to move"<< endl;
     }
    
     
    int chosenhole = playerType(s)->chooseMove(board1, s);
    Side endSide;
    int endHole;
    board1.sow(s, chosenhole, endSide, endHole);
    if(!playerType(s)->isInteractive())
    {
        cout << "Press ENTER to continue...";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout<<endl;
    }
    
    
    //continue turn while endhole is the pot
    while(endHole == 0 && board1.beansInPlay(s) != 0)
    {
        //cout<< "again endhole: "<< endHole;
        display();
        chosenhole = playerType(s)->chooseMove(board1, s);
        board1.sow(s, chosenhole, endSide, endHole);
        if(!playerType(s)->isInteractive())
        {
            cout << "Press ENTER to continue...";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout<<endl;
        }
    }
    
    //if endhole is 1 and opponent hole has beans, moves beans to pot
    if((endSide == s) && board1.beans(s,endHole) == 1)
    {
        if(board1.beans(Opponent, endHole) != 0)
        {
            //cout << "moving opponents beans to pot "<< endl;
            board1.moveToPot(Opponent, endHole, s);
            board1.moveToPot(s, endHole, s);
        }
    }
    
    currentPlayer = Opponent;
    return true;
}



void Game::play()
{
    display();
    while(move(currentPlayer) != false)
    {
        display();
    }
    
    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    
    if(hasWinner)
    {
        if(winner == SOUTH)
        {
            cout<<southPlayer->name()<<" is the winner!"<<endl;
        }else{
            cout<<northPlayer->name()<<" is the winner!"<<endl;
        }
    }else{
        cout<<"It's a tie!"<<endl;
    }
    
}


Player* Game::playerType(Side s)
{
    if(s == NORTH)
    {
        return northPlayer;
    }else{
        return southPlayer;
    }
}

int Game::beans(Side s, int hole) const
{
    return board1.beans(s, hole);
}


