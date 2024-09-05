//
//  main.cpp
//  
//
//  Created by Gaana Jakkahalli on 5/28/23.
//

#include "Game.h"
    #include "Player.h"
    #include "Board.h"
    #include "Side.h"
    #include <iostream>
    #include <cassert>
    using namespace std;

    

    int main()
    {
        SmartPlayer bp1("Bart");
        HumanPlayer bp2("Homer");
                Board b(3, 2);
                Game g(b, &bp2, &bp1);
                g.play();
        
        cout << "Passed all tests" << endl;
    }
