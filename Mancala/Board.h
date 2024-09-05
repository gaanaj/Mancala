//
//  Board.h
//  
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#ifndef Board_h
#define Board_h
#include "Side.h"

class Board
{
    public:
        Board(int nHoles, int nInitialBeansPerHole);
        Board(const Board& other);
        Board& operator=(const Board& other);
        int holes() const;
        int beans(Side s, int hole) const;
        int beansInPlay(Side s) const;
        int totalBeans() const;
        bool sow(Side s, int hole, Side& endSide, int& endHole);
        bool moveToPot(Side s, int hole, Side potOwner);
        bool setBeans(Side s, int hole, int beans);
        ~Board();
    
    private:
        int m_Holes;
        int m_InitialBeansPerHole;
        int m_arraysize;
        int *b;
    
};

#endif /* Board_h */
