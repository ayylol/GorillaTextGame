#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
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
            return current + 1;
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
    void newMoves()
    {
        for (int i = 0; i < 3; i++)
            moves[i] = rand() % 4;
        current = 0;
    }

public:
    int row;
    int col;
    char symbol;
    npc(bool aggressive, int r, int c, char s)
    {
        symbol = s;
        row = r;
        col = c;
        newMoves();
    }
    void nextMove(string &gameBoard, int rowSize)
    {
        //cout << "in next move " << row << " " << col<< "\n";
        bool notDone = true;
        int move;
        int tR;
        int tC;
        int counter = 0;
        while (notDone && counter < 5)
        {
            //cout << "moveLoop ";
            //get the postion of the move
            if (moves[current] == 0)
            {
                //cout << "down ";
                tR = row + 1;
                tC = col;
                move = ((rowSize + 1) * (row + 1) + col);
            }
            else if (moves[current] == 1)
            {
                //cout << "right ";
                tR = row;
                tC = col + 1;
                move = ((rowSize + 1) * row + (col + 1));
            }
            else if (moves[current] == 2)
            {
                //cout << "up ";
                tR = row - 1;
                tC = col;
                move = ((rowSize + 1) * (row - 1) + col);
            }
            else if (moves[current] == 3)
            {
                //cout << "left ";
                tR = row;
                tC = col - 1;
                move = ((rowSize + 1) * row + (col - 1));
            }
            //cout << "moveChecked "<< move << " ";
            //If the move is to an empty slot do nothing otherwise get another moveset and try again
            if (gameBoard[move] == ' ')
            {
                //cout << "Moving\n";
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
                    aggr = true;
                else
                    aggr = false;
                npc temp(aggr, row, col, gameBoard[i]);
                //cout << temp.symbol << '\n';
                bots.push_back(temp); ///////////THIS LINE
                //cout << bots.back().symbol << "\n\n";
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
        //cout << "in Check\n";
        for (int i = rC; i <= pRow + 1; i++)
        {
            for (int j = cC; j <= pCol + 1; j++)
            {
                pos = ((rowSize + 1) * i + j);
                if (gameBoard[pos] == 'Z')
                {
                    //cout << "returning Check 2\n";
                    return 2;
                }
                else if (gameBoard[pos] == 'z' || gameBoard[pos] == 'o')
                {
                    //cout << "returning Check 3\n";
                    return 3;
                }
            }
        }
        //cout << "returning Check 1\n";
        return 1; //game is ongoing
    }
    int act(char &c)
    {
        if (c == 'p')
        {
            //cout << "Pausing Game\n";
            c = '`';
            return 0; //pausing game
        }
        else
        { //game continuing
            //cout << "performing next moves\n";
            if (time(0) % 2 == 0)
            {
                for (int i = 0; i < bots.size(); i++)
                {
                    //cout << i << "\n";
                    bots[i].nextMove(gameBoard, rowSize);
                    //cout << i << "\n\n";
                }
                //cout << "performed next moves\n";
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
                g.ini();
            else
            {
                cout << string(100, '\n') + g.printScreen();
                //cout << g.printScreen();
                //cout << "About to act\n";
                status = g.act(key);
                //cout << "Acted\n";
            }
        }
        else
            break;
    }
    return 0;
}