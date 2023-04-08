#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(const string& in, const string& floating, const set<string>& dict, set<string>& answers, int idx);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{

    //create a set of string for all the possible answers
    set<string> answers;

    //keep track of current spot
    int idx = 0;

    //using "in" put in "floating" in the blanks first before the alphabet

    //first call to recursive function
    wordleHelper(in, floating, dict, answers, idx);

    return answers;


}

// Define any helper functions here
void wordleHelper(const string& in, const string& floating, const set<string>& dict, set<string>& answers, int idx) 
{
    // cout << "yellow when recursed: " << floating << endl;

    //base case: used all of the floating letter AND at the last letter
    if(floating.size() == 0 && idx == in.size()) {
        
        set<string>::iterator it = dict.find(in);
        //check if the completed word is in the dictionary
        if(it != dict.end()) {
            //if it is, add the word to the set of answers
            answers.insert(in);
        }
        return; //if word has no more blanks, return (if found)/undo (if not found)
    }


    //used as a placeholder
    string yellow = floating;
    string green = in;

    //count how many letters are blank
    size_t blank = 0;
    for(size_t i = 0; i < green.size(); i++) {
        if(green[i] == '-') 
            blank++;
    }

    //case 1: if the current index is blank (no letter)
    if(green[idx] == '-') { 
        
        //case 1a: if the number of blank spaces == yellow letters
        if(blank == yellow.size()) {
            //then it should have those yellow letters 
            for(size_t i = 0; i < yellow.size(); i++) {
                green[idx] = yellow[i]; //insert the letter in the blank space
                
                int indexPos = i;
                yellow.erase(indexPos, 1); //delete the letter from yellow bank after it was assigned

                //recurse to the next index
                wordleHelper(green, yellow, dict, answers, idx+1);

                //reset the yellow letters for other blanks
                yellow = floating;

            }
        }
        
        //case 1b: if there are more blanks than yellow letters
        else if(blank > yellow.size()) { //if there are more blanks than yellow letters

            //loop over the entire alphabet
            for(size_t i = 'a'; i <= 'z'; i++) {
                green[idx] = i;

                if(yellow.find(i) != string::npos) { //if alphabet is a yellow letter
                    // cout << "inside if yellow.find(i)" << endl;
                    int indexPos = yellow.find(i);
                    yellow.erase(indexPos,1); //delete the letter from yellow bank after it was assigned (green[idx] = i;)
                    
                    //recurse
                    wordleHelper(green, yellow, dict, answers, idx+1);
                    
                    //reset the yellow letters for other blanks
                    yellow = floating;
                }
                else { //only recurse, no yellow letter to delete
                    wordleHelper(green, yellow, dict, answers, idx+1); 
                }
            }

        }
        else { //more floating than blanks
            return;
        }


    }
    //case 2: if the current index is filled
    else { //already has a letter, so go onto the next index
        wordleHelper(green, yellow, dict, answers, idx+1);
    }

    return;

}