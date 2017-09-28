#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <fstream>
#include <typeinfo>
#include <list>
#include <map>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

int main();

void gameLoop();

void checkNumber(ifstream& file, set<string> dict, vector<string>& wordsLeft, int& wordLength);

void viewRest(bool& vrb){

    string viewRestString;
    cout << "Do you want to see how many words you have left after each guess? y/n"<< endl;
    cin >> viewRestString;

    if(viewRestString == "y"){
      vrb = true;
    }
    else if(viewRestString == "n"){
        vrb = false;
    }
    else{
        viewRest(vrb);
    }
}

void guessLetter(string& guess){

    bool valid = false;
    cout<< "Please guess a lower-case letter:"<< endl;
    cin >> guess;

    for(auto a: ALPHABET){
        if( guess[0] == a && guess.length() == 1){
           valid = true;
        }
    }
    if(!valid){
        cout<< "Please write a valid letter"<< endl;
        guessLetter(guess);
    }
}

void playAgain(){

    string ans;
    cout << "Do you want to play again?! (y/n)" << endl;
    cin >> ans;
    if(cin.fail()){
        cin.clear();
        cin.ignore(256, '\n');
        playAgain();
    }
    else{
        if(ans == "y"){
            main();
        }
        else if(ans == "n"){
            exit(0);
        }
        else{
            playAgain();
        }
    }
}

void endGame(string str, vector<string> myVector, int guessNumb){

    if(str == myVector.at(0)){
        cout << "You WON!! Your word was " << str << endl;
        playAgain();
    }
    if(guessNumb <= 0){
        cout << "You lose. The word you were looking for was: " << myVector.at(0) <<endl;
        playAgain();
    }
}

void divideFamily(map<string,vector<string>>& myMap,vector<string> myVector, string guess, string& newstr){
    string str;

    for(vector<string>::iterator i = myVector.begin(); i != myVector.end(); i++){
        string word = *i;
        str = "";

        int wordLength = word.length();
        for(int j = 0; j < wordLength; ++j){

                if(word[j] == guess[0]){
                    str.append(guess);
                }
                else if(word[j] == newstr[j]){
                    str += newstr[j];
                }
                else
                    str.append("-");
        }
        myMap[str].push_back(word);
    }

}

void sizeOfVector(int& currentSize, map<string,vector<string>>& myMap, string& newstr, vector<string>& v1){

    for(pair<string, vector<string>> newVector : myMap){

        int words = newVector.second.size();

        if(words > currentSize){
            newstr = newVector.first;
            v1 = newVector.second;
            currentSize = words;
        }
    }
}

void gameLoop(vector<string> myVector, string guess, string& newstr, int guessNumb, bool viewWords){// returns the largest set

    map<string,vector<string>> myMap;
    vector<string> v1;
    int currentSize = 0;

    divideFamily(myMap, myVector, guess, newstr);

    sizeOfVector(currentSize, myMap, newstr, v1);

    guessNumb -= 1;
    endGame(newstr, v1, guessNumb);

    cout << "Guesses left: "<< guessNumb  << endl;
    if(viewWords){
        cout << "Words left in dict: " << currentSize << endl;
    }

    cout << "\n" << "Your word: "<< newstr << endl;

    guessLetter(guess);

    gameLoop(v1,guess, newstr, guessNumb, viewWords);
}

void guessAmount(int& guessNumb){

    int currGuess;
    cout << "Please write an amount > 0 of guesses you want" << endl;
    cin >> currGuess;

    if(cin.fail()){
       cin.clear();
       cin.ignore(256, '\n');
       guessAmount(guessNumb);
    }
    else if(currGuess < 1){
        cin.clear();
        cin.ignore(256, '\n');
        guessAmount(guessNumb);
    }

    else{
         guessNumb = currGuess;
    }
}

void readFile(ifstream& file, set<string> dict, vector<string>& wordsLeft, int wordLength){
    string line;
    while(getline(file, line)){
        dict.insert(line);
        int lineLength = line.length();
        if(lineLength == wordLength){
            wordsLeft.push_back(line);
        }
    }

}

void checkNumber(ifstream& file, set<string> dict, vector<string>& wordsLeft, int& wordLength){

    int num;
    cout << "Please write the number of char you want to play with:"<<  endl;
    cin >> num;

    if(cin.fail()){
        cout << "Please write a valid number" << endl;
        cin.clear();
        cin.ignore(256,'\n');
        checkNumber(file, dict, wordsLeft, wordLength);
    }
    else{
        readFile(file, dict, wordsLeft, num);
        if(wordsLeft.empty()){
           cout << "Ain't no words with that length" << endl;
           cin.clear();
           cin.ignore(256,'\n');
           file.close();
           file.open("dictionary.txt");
           checkNumber(file, dict, wordsLeft, wordLength);
       }
       else{
            wordLength = num;
       }
    }
}

int main() {
    int wordLength;
    int guessNumb;
    bool viewRestBool;
    string guess;

    ifstream file ("dictionary.txt");
    set<string> dictionary;
    vector<string> wordsLeft;

    cout << "Welcome to Hangman." << endl;

    checkNumber(file, dictionary, wordsLeft, wordLength);

    guessAmount(guessNumb);

    viewRest(viewRestBool);

    cout << "number of guesses left: "<< guessNumb << endl;
    cout << "currentword: ";
    for(int i = 0; i < wordLength; i++){
        cout << "-" ;
    }

    cout << endl;
    cout << endl;

    string startKey = "";

    guessLetter(guess);

    gameLoop(wordsLeft, guess, startKey, guessNumb, viewRestBool);

    return 0;
}
