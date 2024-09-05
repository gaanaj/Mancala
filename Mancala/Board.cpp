//
//  main.cpp
// 
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#include <iostream>
#include "Board.h"
#include "Side.h"
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if(nHoles <= 0)
    {
        m_Holes = 1;
    }else{
        m_Holes = nHoles;
    }
    
    if(nInitialBeansPerHole < 1)
    {
        m_InitialBeansPerHole = 0;
    }else{
        m_InitialBeansPerHole = nInitialBeansPerHole;
    }
    
    m_arraysize = (m_Holes*2)+2;
    b = new int[m_arraysize];
    //fill every element with beans and set pot to 0
    for(int i = 0; i < m_arraysize; i++)
    {
        if(i == m_Holes || i == m_arraysize -1)
        {
            //set pot to 0 beans
            b[i] = 0;
        }else
        {
            //set hole to initial beans
            b[i] = m_InitialBeansPerHole;
        }
    }
    
}

Board::Board(const Board& other)
{
    m_Holes = other.m_Holes;
    m_InitialBeansPerHole = other.m_InitialBeansPerHole;
    m_arraysize = other.m_arraysize;
    
    b = new int[m_arraysize];
    
    for (int i = 0; i < m_arraysize; i++) {
        b[i] = other.b[i];
    }
}

Board& Board::operator=(const Board& other)
{
    if (this != &other) {
        // Perform a deep copy of the data members
        m_Holes = other.m_Holes;
        m_InitialBeansPerHole = other.m_InitialBeansPerHole;
        m_arraysize = other.m_arraysize;
        
        // Allocate new memory and copy the array elements
        delete[] b;
        b = new int[m_arraysize];
        for (int i = 0; i < m_arraysize; i++) {
            b[i] = other.b[i];
        }
    }

    return *this;
}


Board::~Board()
{
    delete[] b;
}


int Board::holes() const
{
    return m_Holes;
}

//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const
{
    if(hole > m_Holes || hole < 0)
    {
        return -1;
    }
    
    if(s == SOUTH)
    {
        if(hole == 0)
        {
            return b[m_Holes];
        }
        
        return b[hole-1];
    }else
    {
        if(hole == 0)
        {
            return b[m_arraysize-1];
        }
        
        return b[(m_arraysize-1)-hole];
    }
    
}

//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::beansInPlay(Side s) const
{
    int counter = 0;
    if(s == SOUTH)
    {
        for(int i = 0; i < m_Holes; i++)
        {
            counter += b[i];
        }
    }else
    {
        for(int i = m_Holes+1; i < m_arraysize-1; i++)
        {
            counter += b[i];
        }
    }
    return counter;
}


//Return the total number of beans in the game, including any in the pots.
int Board::totalBeans() const
{
    int counter = 0;
    for(int i = 0; i < m_arraysize; i++)
    {
        counter += b[i];
    }
    return counter;
}

//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed.
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    //check if invalid
    if(hole < 1 || hole > m_Holes)
    {
        return false;
    }
    
    int numBeans = beans(s,hole);
    
    //check if hole is empty
    if(numBeans == 0)
    {
        //cout << "no beans in hole"<< endl;
        return false;
    }
    
    int skipPot;
    int arrayIndex;
    if(s == SOUTH)
    {
        skipPot = m_arraysize-1;
        arrayIndex = hole-1;
        b[arrayIndex] = 0;
        //cout << "skippot south "<< skipPot<<" index "<<arrayIndex<<endl;
        
    }else{
        skipPot = m_Holes;
        arrayIndex = m_arraysize-1-hole;
        b[arrayIndex] = 0;
        //cout << "skippot north "<< skipPot<<" index "<<arrayIndex<<endl;
    }

    while (numBeans != 0)
    {
        //cout << "numbeans: "<<numBeans<<endl;
        arrayIndex++;
        arrayIndex = arrayIndex % m_arraysize;
        if (arrayIndex != skipPot)
        {
            b[arrayIndex]++;
            //cout<<"beans at index: "<<b[arrayIndex]<<endl;
            numBeans--;
        }
    }
    
    //cout<<"complete sow!"<<endl;    //set endside and endhole
    if(arrayIndex <= m_Holes)
    {
        endSide = SOUTH;
        if(arrayIndex == m_Holes)
        {
            endHole = 0;
        }else{
            endHole = arrayIndex+1;
        }
    }else{
        endSide = NORTH;
        if(arrayIndex == m_arraysize-1)
        {
            endHole = 0;
        }else{
            endHole = m_arraysize- 1 - arrayIndex;
        }
    }
    
    return true;
}

//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole < 1 || hole > m_Holes)
    {
        return false;
    }
    
    if(potOwner == SOUTH)
    {
        b[m_Holes] += beans(s,hole);
    }else{
        b[m_arraysize-1] += beans(s,hole);
    }
    
    setBeans(s, hole, 0);
    
    return true;
}


//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.)
bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole < 1 || hole > m_Holes)
    {
        return false;
    }
    
    if(s == SOUTH)
    {
        b[hole-1] = beans;
    }else{
        b[m_arraysize- 1-hole] = beans;
    }
    return true;
}















