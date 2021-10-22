#include <iostream>
#include <cstdlib> 
#include "io.h"
#include <fstream>

#include "GameManager.h"

using std::cerr;
using std::ifstream;
using std::ofstream;
using std::endl;

const int KB_ESCAPE = 27;
//-------------------------------------------------------------------

int main()
{
    std::system("cls");
    Screen::resetLocation();

    ifstream BoardFile;
    BoardFile.open("Board.txt"); //file to read from
    if (!BoardFile.is_open())
    {
        cerr << "Cannot open file..." << endl;
        exit(EXIT_FAILURE); // if the file cannot be open
    }

    GameManager myGame;
    myGame.StartGame(BoardFile);

    return EXIT_SUCCESS;
}







/*
for (auto exit = false; !exit; )
    {
        auto c = Keyboard::getch();
        switch (c)
        {
        case 0:
        case SPECIAL_KEY:
            handleSpecialKey();
            break;
        default:
            exit = handleRegularKey(c);
            break;
        }
    }
}
//---------------------------------------------
bool handleRegularKey(int c)
{
    switch (c)
    {
    case 'A':
        break;
    case 'a':
        break;
    case KB_ESCAPE:
        return true;
    default:
        break;
    }
    return false;
}
//-----------------------------------------
void handleSpecialKey()
{
    auto c = Keyboard::getch();
    switch (c)
    {
    case KB_UP: 
        break;
    case KB_DOWN:
        break;
    case KB_LEFT:
        break;
    case KB_RIGHT:
        break;
    default:
        //std::cout << "Unknown special key pressed (code = " << c << ")\n";
        break;
    }
}
*/