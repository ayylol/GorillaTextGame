#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Menu
{
private:
    string title;
    int current = 0;
    vector<string> options;

public:
    Menu()
    {
        //from https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
        ifstream f("title");
        if (f)
        {
            ostringstream ss;
            ss << f.rdbuf();
            title = ss.str();
        }
        //////////////////////////////////////////////////////////////////
        options.push_back("Play");
        options.push_back("Exit");
    }

    int act(char &key)
    {
        //check if a valid key was pressed
        if (key == 'w')
        {
            if (current > 0)
                current--;
            key = '`'; //reset the key so that it is not processed again (thats what is happening everytime you see this line)
        }
        else if (key == 's')
        {
            if (current < options.size() - 1)
                current++;
            key = '`';
        }
        else if (key == 'p')
        {
            key = '`';
            if (current == 0)
                return 1;
            else
                return 4;
        }
        return 0;
    }

    string printScreen()
    {
        string output = title + '\n';
        for (int i = 0; i < options.size(); i++)
        {
            if (current == i)
            {
                output = output + "[" + options[i] + "]\n";
            }
            else
            {
                output = output + options[i] + "\n";
            }
        }
        return output; //print the title
    }
};
class npc
{
private:
    int current = 0;
    int moves[3];
    bool aggresive;
    int sight;
    int focus;
    void newMoves()
    {
        for (int i = 0; i < 3; i++)
            moves[i] = rand() % 4; //0=down, 1=right, 2=up, 3=left
        current = 0;
    }

public:
    int row;
    int col;
    char symbol;
    npc(bool a, int r, int c, char s, int si, int f)
    {
        aggresive = a;
        symbol = s;
        sight = si;
        focus = f;
        row = r;
        col = c;
        newMoves();
    }
    void nextMove(string &gameBoard, int rowSize, int pRow, int pCol)
    {
        if (aggresive)
        {
            int hDiff = col - pCol;
            int vDiff = row - pRow;
            int follow = rand() % 100;
            if ((follow > focus) && (((hDiff >= -sight) && (hDiff <= 0)) || ((hDiff >= 0) && (hDiff <= sight))) &&
                (((vDiff >= -sight) && (vDiff <= 0)) || ((vDiff >= 0) && (vDiff <= sight))))
            { //checks if npc is within range
                int vertOrHori;
                if (vDiff == 0)
                    vertOrHori = 1;
                else if (hDiff == 0)
                    vertOrHori = 0;
                else
                    vertOrHori = rand() % 2;
                if (vertOrHori == 0)
                { //move closer vertically
                    if (vDiff > 0)
                    { //go down
                        moves[current] = 2;
                    }
                    else
                    { //go up
                        moves[current] = 0;
                    }
                }
                else
                { //move closer horizontally
                    if (hDiff > 0)
                    { //go right
                        moves[current] = 3;
                    }
                    else
                    { //go left
                        moves[current] = 1;
                    }
                }
            }
        }
        bool notDone = true;
        int move;
        int tR;
        int tC;
        int counter = 0;
        while (notDone && counter < 5)
        {
            //get the postion of the move
            if (moves[current] == 0)
            {
                tR = row + 1;
                tC = col;
                move = ((rowSize + 1) * (row + 1) + col);
            }
            else if (moves[current] == 1)
            {
                tR = row;
                tC = col + 1;
                move = ((rowSize + 1) * row + (col + 1));
            }
            else if (moves[current] == 2)
            {
                tR = row - 1;
                tC = col;
                move = ((rowSize + 1) * (row - 1) + col);
            }
            else if (moves[current] == 3)
            {
                tR = row;
                tC = col - 1;
                move = ((rowSize + 1) * row + (col - 1));
            }
            //If the move is to an empty slot do nothing otherwise get another moveset and try again
            if (gameBoard[move] == ' ')
            {
                gameBoard[move] = symbol;
                gameBoard[((rowSize + 1) * row + col)] = ' ';
                row = tR;
                col = tC;
                current++;
                if (current > 2)
                    current = 0;
                notDone = false;
            }
            else
            {
                //cout << "New moves\n";
                tR = row;
                tC = col;
                newMoves();
            }
            counter++;
        }
    }
};
class Game
{
private:
    //GameBoard variables
    int rowSize;
    string gameBoard;

    vector<npc> bots;
    int pRow;
    int pCol;

public:
    int ongoing = 0;
    void ini()
    {
        //from https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
        ifstream f("map");
        if (f)
        {
            ostringstream ss;
            ss << f.rdbuf();
            gameBoard = ss.str();
        }
        //////////////////////////////////////////////////////////////////

        //Finiding the width of the game board
        rowSize = 0;
        while (gameBoard[rowSize] != '\n')
            rowSize++;

        //Initializing npcs
        bool aggr;
        int sight;
        int focus;
        int row = 0;
        int col = 0;

        for (int i = 0; i < gameBoard.length(); i++)
        {
            if (gameBoard[i] == 'M')
            {
                pRow = row;
                pCol = col;
            }
            else if (gameBoard[i] != ' ' && gameBoard[i] != '\n' && gameBoard[i] != '#' && gameBoard[i] != 'z' && gameBoard[i] != 'o')
            { //initialize an npc if they are not one of these characters
                if (gameBoard[i] == 'Z')
                {
                    aggr = true;
                    focus = rand() % 55 + 5;
                    sight = (rand() % 10) + 5;
                }
                else
                {
                    aggr = false;
                    sight = 0;
                    focus = 0;
                }
                npc temp(aggr, row, col, gameBoard[i], sight, focus);
                bots.push_back(temp);
            }
            //changing row and column
            if (gameBoard[i] == '\n')
            {
                row++;
                col = 0;
            }
            else
            {
                col++;
            }
        }
        ongoing = 1;
    }

    int check()
    {
        int rC = pRow - 1;
        int cC = pCol - 1;
        int pos;
        //loop through cells surrounding player see if one is a zookeeper, or exit
        for (int i = rC; i <= pRow + 1; i++)
        {
            for (int j = cC; j <= pCol + 1; j++)
            {
                pos = ((rowSize + 1) * i + j);
                if (gameBoard[pos] == 'Z')
                {
                    return 2;
                }
                else if (gameBoard[pos] == 'z' || gameBoard[pos] == 'o')
                {
                    return 3;
                }
            }
        }
        return 1; //game is ongoing
    }
    int act(char &c, bool botsMove)
    {
        if (c == 'p')
        {
            c = '`';
            return 0; //pausing game
        }
        else
        { //game continuing
            int prevPos = ((rowSize + 1) * pRow + pCol);
            int pos;
            bool playerMoved;
            if (c == 'w')
            {
                pos = ((rowSize + 1) * (pRow - 1) + pCol);
                if (gameBoard[pos] == ' ')
                {
                    gameBoard[prevPos] = ' ';
                    gameBoard[pos] = 'M';
                    pRow--;
                    playerMoved = true;
                }
                c = '`';
            }
            else if (c == 'a')
            {
                pos = ((rowSize + 1) * pRow + (pCol - 1));
                if (gameBoard[pos] == ' ')
                {
                    gameBoard[prevPos] = ' ';
                    gameBoard[pos] = 'M';
                    pCol--;
                    playerMoved = true;
                }
                c = '`';
            }
            else if (c == 's')
            {
                pos = ((rowSize + 1) * (pRow + 1) + pCol);
                if (gameBoard[pos] == ' ')
                {
                    gameBoard[prevPos] = ' ';
                    gameBoard[pos] = 'M';
                    pRow++;
                    playerMoved = true;
                }
                c = '`';
            }
            else if (c == 'd')
            {
                pos = ((rowSize + 1) * pRow + (pCol + 1));
                if (gameBoard[pos] == ' ')
                {
                    gameBoard[prevPos] = ' ';
                    gameBoard[pos] = 'M';
                    pCol++;
                    playerMoved = true;
                }
                c = '`';
            }
            if (botsMove || playerMoved)
            {
                for (int i = 0; i < bots.size(); i++)
                {
                    bots[i].nextMove(gameBoard, rowSize, pRow, pCol);
                }
            }
            return check();
        }
    }
    string printScreen()
    {
        return gameBoard;
    }
};

int main()
{
    Menu m;
    Game g;
    char key;
    int status = 0; //status codes: 0=menu 1=game 2=lost 3=won
    string oldPrint = "";
    string toPrint = "";
    clock_t time;
    clock_t cTime;
    int ms;
    while (true)
    {
        //from https://stackoverflow.com/questions/20349585/c-library-function-to-check-the-keypress-from-keyboard-in-linux
        if (kbhit())
        {
            key = getch();
            //cout << "Entered:" << key << "\n";
            if (key == 'e')
                break;
        }
        //////////////////////////////////////////////////////////////////
        //cout << status;
        if (status == 0)
        {
            toPrint = m.printScreen();
            if (oldPrint.compare(toPrint) != 0)
            {
                cout << string(100, '\n') << toPrint;
                oldPrint = toPrint;
            }
            status = m.act(key);
        }
        else if (status == 1)
        {
            if (g.ongoing == 0)
            {
                g.ini();
                time = clock();
            }
            else
            {
                toPrint = g.printScreen();
                if (oldPrint.compare(toPrint) != 0)
                {
                    cout << string(100, '\n') << toPrint;
                    //cout << toPrint;
                    oldPrint = toPrint;
                }
                cTime = clock() - time;
                ms = cTime / CLOCKS_PER_SEC * 1000;
                //cout << ms;
                if (ms >= 5)
                {
                    status = g.act(key, true);
                    time = clock();
                }
                else
                {
                    status = g.act(key, false);
                }
            }
        }
        else if (status > 1)
        {
            if (status < 4)
            {
                string fName;
                if (status == 3)
                    fName = "Winner";
                else if (status == 2)
                    fName = "GameOver";
                //from https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
                ifstream f(fName);
                if (f)
                {
                    ostringstream ss;
                    ss << f.rdbuf();
                    toPrint = ss.str();
                }
                //////////////////////////////////////////////////////////////////
                cout << string(100, '\n') << toPrint;
            }
            break;
        }
    }
    return 0;
}