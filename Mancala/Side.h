//
//  Side.h
//  
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#ifndef Side_h
#define Side_h


enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

inline
Side returnOpponent(Side s)
{
    if(s == NORTH)
    {
        return SOUTH;
    }else{
        return NORTH;
    }
}


#endif /* Side_h */
