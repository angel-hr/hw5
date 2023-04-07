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
/*void arrange(std::set<std::string>& jumbles, const std::string& in,
             const std::string& floating, size_t pos1, size_t pos2);
*/
void arrange(std::set<std::string>& jumbles, const std::string& in,
             const std::string& floating, size_t pos);

void fill(std::set<std::string>& words, const std::string& jumbled, 
          char letter, size_t step);

bool triple(const std::string& in, const std::string& floating, size_t pos1, size_t pos2);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict){   
    std::set<std::string> jumbled; //create an empty set

    arrange(jumbled, in, floating, 0);

    std::set<std::string> words;
    std::set<std::string>::iterator it;
    for (it = jumbled.begin(); it != jumbled.end(); ++it){
        if (dict.find(*it) != dict.end()){
            words.insert(*it); 
        }
    }

    return words;
}

// Define any helper functions here
void arrange(std::set<std::string>& jumbles, const std::string& in,
             const std::string& floating, size_t pos)
{
    //first loop to set charatcer in any given location
    if (pos == floating.size()) {
        std::string word = in; //deconvert in from const
        fill(jumbles, word, 'a', 0);
        return;
    }

    for (size_t i = 0; i < in.size(); i++){
        std::string temp(in);
        if (in[i] == '-'){
            bool check = triple(in, floating, i, pos);
            if (check){ 
                temp[i] = floating[pos];
                arrange(jumbles, temp, floating, ++pos); //increment the size of floating[pos] so that we can get to the next letter
                --pos;
            }
        }
    }
}

bool triple(const std::string& in, const std::string& floating, size_t pos1, size_t pos2){
    if (pos1 > 1){ // aa-a
        if (in[pos1-1] == floating[pos2] && in[pos1 -2] == floating[pos2]){ //then adding floating[pos2] to in[pos1] would cause a triple
            return false;
        }
    } 

    if (pos1 > 0 && in.size() > pos1 + 1) { // a-a or aa-a
        if (in[pos1-1] == floating[pos2] && in[pos1+1] == floating[pos2]){
            return false;
        }
    }

    if (in.size() > pos1 +2){ //  -aa or a-aa or aa-aa
        if(in[pos1+1] == floating[pos2] && in[pos1 +2] == floating[pos2]){
            return false;
        }
    }
    return true;
}

/*
void arrange(std::set<std::string>& jumbles, const std::string& in,
             const std::string& floating, size_t pos1, size_t pos2)
{
    if (pos1 == in.size()-1) {return;}
    if (pos2 == floating.size()){
        std::string word = in; //deconvert in from const
        fill(jumbles, word, 'a', 0);
        return;
    }
    
    arrange(jumbles, in, floating, ++pos1, pos2);
    if (in[pos1] != '-') {return;}
    
    //check for triple formations
    if (pos1 > 1){ // aa-a
        if (in[pos1-1] == floating[pos2] && in[pos1 -2] == floating[pos2]){ //then adding floating[pos2] to in[pos1] would cause a triple
            arrange(jumbles, in, floating, pos1, ++pos2); //move to the next location in the floating string
        }
    } 

    if (pos1 > 0 && in.size() > pos1 + 1) { // a-a or aa-a
        if (in[pos1-1] == floating[pos2] && in[pos1+1] == floating[pos2]){
            arrange(jumbles, in, floating, pos1, ++pos2);
        }
    }

    if (in.size() > pos1 +2){ //  -aa or a-aa or aa-aa
        if(in[pos1+1] == floating[pos2] && in[pos1 +2] == floating[pos2]){
            arrange(jumbles, in, floating, pos1, ++pos2);
        }
    }


    std::string blah(in);   //make a copy of in because its const
    blah[pos1] = floating[pos2];   //replace the '-' with the letter at floating[pos2]

    arrange(jumbles, blah, floating, pos1, ++pos2);
}
*/

void fill(std::set<std::string>& words, const std::string& jumbled, char letter, size_t step) //there exists no three letter repetitions
{
    if (letter > 122){return;}  //if the charachter is greater than lowercase z
    if (jumbled.size() == step) { //these will take care of the empty case and when we reach the limit
        if (jumbled.size() > 0){
            words.insert(jumbled); //if we reach the end of the word then jumbled word has been filled
        }
        return;
    } 

    /*if (jumbled[step] == 'q'){ //q is always followed by u
        if (step+1 < jumbled.size()){
            std::string word(jumbled);
            word[step + 1] = 'u';
            fill(words, word, 'a', ++step);  
        }
    }*/

    if (jumbled.size() < 3){ //we dont have to worry about repetitons
        if (jumbled[step] != '-') {
            fill(words, jumbled, letter, ++step);
        }
        std::string word(jumbled);
        word[step] = letter;
        fill(words, jumbled, ++letter, step); //this will insert all the words: a, b, c, d ...  
        fill(words, word, 'a', step++);
    }
    else { //if jumble size is 3 or greater than we need to avoid three letter repetitions
        if (jumbled[step] != '-'){
            fill(words, jumbled, letter, ++step);
        }
        else {
            if (step == 0){ //case -nn ==> inn or zoo
                if (jumbled[step+1] == letter && jumbled[step+2] == letter){      //check two letters foward
                    letter++;  //increase the letter if it would make a triple
                }
            }
            else {  
                if (step+1 < jumbled.size()){
                    if (jumbled[step-1] == letter && jumbled[step+1] == letter){//check one back and one front
                        letter++;
                    } 
                }
                if (step + 2 < jumbled.size()){ //check if the two up front make a triple
                    if (jumbled[step +1] == letter && jumbled[step+2] == letter){
                        letter++;
                    }
                }
            }       

            std::string word(jumbled);
            word[step] = letter;
            fill(words, jumbled, ++letter, step);
            fill(words, word, 'a', ++step);
        }
    }
}

