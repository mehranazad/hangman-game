/*
Author: Mehran Azad
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <stdlib.h>

using namespace std;

const int g_MAX_WORDS = 1000;
int g_word_count = 0;
string g_words[g_MAX_WORDS];
string g_definitions[g_MAX_WORDS];
string g_pos[g_MAX_WORDS];

void readWords(string filename)
{
    string WORD, POS, DEFINITION;
    ifstream fin(filename);
   while(fin >> WORD >> POS)
{
    getline(fin, DEFINITION);
    DEFINITION = DEFINITION.substr(3);
    g_words[g_word_count] = WORD;
    g_pos[g_word_count] = POS;
    g_definitions[g_word_count] = DEFINITION;
    g_word_count++;
}
for (int i = 0; i < g_word_count; i++)
{
    cout << g_words[i] << " " << g_pos[i] << " '" << g_definitions[i] << endl; 
}
fin.close();
}


int getIndex(string word) 
{
    for (int i = 0; i < g_word_count; i++) {
        if (g_words[i] == word) {
            return i;
        }
    }
    return -1;
}

string getDefinition(string word) 
{
    int index = getIndex(word);
    if (index == -1) {
        return "NOT_FOUND";
    }
    return g_definitions[index];
}

string getPOS(string word) 
{
    int index = getIndex(word);
    if (index == -1) {
        return "NOT_FOUND";
    }
    return g_pos[index];
}

int countPrefix(string prefix) 
{
    int count = 0;
    for (int i = 0; i < g_word_count; i++) {
        if (g_words[i].find(prefix) == 0) {
            count++;
        }
    }
    return count;
}

bool addWord(string word, string definition, string pos)
{
    for (int i = 0; i < g_word_count; i++)
    {
        if (g_words[i] == word || g_MAX_WORDS < g_word_count)
        {
            return false;
        }
        g_words[g_word_count] = word;
        g_definitions[g_word_count] = definition;
        g_pos[g_word_count] = pos;
        g_word_count++;
        return true;
    }
}

bool editWord(string word, string definition, string pos)
{
    for (int i = 0; i < g_word_count; i++)
    {
        if (g_words[i] == word)
        {
            g_words[i] = word;
            g_definitions[i] = definition;
            g_pos[i] = pos;
            return true;
        }
    }  
    return false;
}

bool removeWord(string word)
{
    for (int i = 0; i < g_word_count; i++)
    {
        if (g_words[i] == word)
        {
            g_words[i] = g_words[g_word_count-1];
            g_definitions[i] = g_definitions[g_word_count-1];
            g_pos[i] = g_pos[g_word_count-1];
            g_words[g_word_count-1] = "";
            g_definitions[g_word_count-1] = "";
            g_pos[g_word_count-1] = "";        
            g_word_count--;
            return true;
        }
    }
    return false;
}

// game-loop for Hangman


string getRandomWord() 
{
    srand((unsigned) time(NULL));
    int index = rand() % g_word_count;
    return g_words[index];
}

string maskWord(string word)
{
    string empty_string = "";
    for (int i = 0; i < word.length(); i++)
    {
        empty_string += "_";
    }
    return empty_string;
}

int getTries(int difficulty)
{
    int tries;
    if (difficulty == 0)
    {
        tries = 9;
    }
    else if (difficulty == 1)
    {
        tries = 7;
    }
    else if (difficulty == 2)
    {
        tries = 5;
    }
    return tries;
}

void printAttempts(int tries, int difficulty)
{
    int attempts = getTries(difficulty);
    string empty = "";
    for (int i = 0; i < (attempts - tries); i++)
    {
        empty = "X" + empty;
    }
    for (int i = 0; i < tries; i++)
    {
        empty = "O" + empty;
    }

    cout << empty;
}

bool revealLetter(string word, char letter, string &current)
{
    bool variable = false;
    for (int i = 0; i <= word.length(); i++)
    {
        if (word[i] == letter)
        {
            current[i] = letter;
            variable = true;
        }
    }
    return variable;
}

void gameLoop() 
{
    int difficulty, tries;
    string word, current;
    char letter;
    while (true) {
        cout << "Welcome to Hangman!" << endl;
        cout <<  "0. easy\n1. normal\n2. hard\n3. exit\nChoose a difficulty: ";
        cin >> difficulty;
        while (difficulty < 0 || difficulty > 3) {
            cout <<  "Enough horseplay >_< !\n0. easy\n1. normal\n2. hard\n3. exit\nChoose a difficulty: ";
            cin >> difficulty;
        }
        if (difficulty == 3) {
            cout << "If you're hangry, go grab a bite! See what I did there?" << endl;
            break;
        }
        word = getRandomWord();
        current = maskWord(word);
        tries = getTries(difficulty);
        while (tries != 0) {
            cout << "Life: ";
            printAttempts(tries, difficulty);
            cout << endl << "Word: "<< current << endl;
            cout << "Enter a letter: ";
            cin >> letter;
            
            if (!revealLetter(word, letter, current)) {
                tries--;
            }
            if (current == word) {
                break;
            }
            if (tries == 2) {
                cout << "The part of speech of the word is "<< getPOS(word) << endl;
            }
            if (tries == 1) {
                cout << "Definition of the word: " << getDefinition(word) << endl;
            }
        }
        if (tries == 0) {
            cout << "The word is \"" << word << "\". Better luck next time! You're getting the ..ahem.. hang of it." << endl;
        }
        else {
            cout << "Congrats!!!" << endl;
        }
    }
}
