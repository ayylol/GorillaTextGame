#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
class Menu{
    private:
        string title;
    public:
        int current=0;
        vector<string> options;

        Menu(){
            //from https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
            ifstream f("title");
            if(f){
                ostringstream ss;
                ss << f.rdbuf();
                title = ss.str();
            }
            //////////////////////////////////////////////////////////////////
            options.push_back("Play");
            options.push_back("Exit");
        }

        int act(char &key){
            if(key=='w'){
                if(current>0)current--; 
                key='`';
            }else if(key=='s'){
                if(current<options.size()-1) current++;
                key='`';
            }else if(key=='\n'){
                return current+1;
                key='`';
            }
            return 0;
        }
       
        string printScreen(){
            string output = title + '\n';
            for(int i=0;i<options.size();i++){
                if(current==i){
                    output = output + "[" + options[i] + "]\n";
                }else{
                    output = output + options[i] + "\n";
                }
            }
            return output; //print the title
        }

};

class Game{
    public:
        int ongoing=0;
};

int main(){
    Menu m;
    //Game g();
    char key;
    while(true){
        //from https://stackoverflow.com/questions/20349585/c-library-function-to-check-the-keypress-from-keyboard-in-linux
        if(kbhit()){
            key = getch();
            //cout << "Entered:" << key << "\n";
            if(key=='e') break;
        }
        //////////////////////////////////////////////////////////////////
        m.act(key);
        
        cout << string(100, '\n')+m.printScreen();
    }
    return 0;
}