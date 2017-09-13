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
    if(ans == "y"){
        main();
    }
    else if(ans == "n"){
        exit(0);
    }
}

void endGame(string str, vector<string> myVector, int guessNumb){
    if(str == myVector.at(0)){
        cout << "You WON!!" << endl;
        playAgain();
    }
    if(guessNumb == 0){
        cout<<"You lose. The word you were looking for was: "<< myVector.at(0) <<endl;
        playAgain();
    }
}

void divideFamilies(vector<string> myVector, string guess, string& newstr, int guessNumb, bool viewWords){// returns the largest set

    map<string,vector<string>> myMap;
    string str;

    for(vector<string>::iterator i = myVector.begin(); i != myVector.end(); i++){
        string word = *i;
        str = "";

        for(int j = 0; j < word.length(); ++j){

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


    int currentSize = 0;
    vector<string> v1;

    for(pair<string, vector<string>> newVector : myMap){

        int words = newVector.second.size();        

        if(words > currentSize){
            newstr = newVector.first;
            v1 = newVector.second;
            currentSize = words;
        }
    }
    guessNumb -= 1;
    endGame(newstr, v1, guessNumb);

    cout << "Guesses left: "<< guessNumb  << endl;
    if(viewWords){
        cout << "Words left in dict: " << currentSize << endl;
    }

    cout << "\n" << "Your word: "<< newstr << endl;

    guessLetter(guess);

    divideFamilies(v1,guess, newstr, guessNumb, viewWords);

}

int guessAmount(){
    int guess;
    cout << "Please write an amount > 0 of guesses you want" << endl;
    cin >> guess;
    if(cin.fail()){
       cin.clear();
       cin.ignore(256, '\n');
       guessAmount();
    }
    else{
        return guess;
    }
}

int wrongNumber(){
    int num;
    cout << "Please write the number of char you want to play with:"<<  endl;
    cin >> num;
    if(!cin){
       cin.clear();
       cin.ignore(256,'\n');
       wrongNumber();
    }
    else{
        return num;
    }
}

int main() {
    int wordLength;
    int guessNumb;
    bool viewRestBool;
    string guess;
    cout << "Welcome to Hangman." << endl;

    wordLength = wrongNumber();

    ifstream file ("dictionary.txt");
    string line;
    set<string> dictionary;
    vector<string> wordsLeft;

    while(getline(file, line)){
        dictionary.insert(line);
        if(line.length() == wordLength){
            wordsLeft.push_back(line);
        }

    }
    if(wordsLeft.empty()){
        cout << "Ain't no words with that length" << endl;
        exit(0);
    }

    guessNumb = guessAmount();

    viewRest(viewRestBool);

    cout << "number of guesses left: "<< guessNumb << endl;
    cout << "currentword: ";
    for(int i = 0; i < wordLength; i++){
        cout << "-" ;
    }
                cout << endl;

    /*if(viewRestBool == true){
        cout << "words left: " << endl;
        for(auto w : wordsLeft){
            cout<< w << endl;
        }
    }*/
    cout << endl;
    string s = "";

    guessLetter(guess);
    divideFamilies(wordsLeft, guess, s, guessNumb, viewRestBool);
    // TODO: Finish the program!

    return 0;
}
