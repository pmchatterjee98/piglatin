//
//  main.cpp
//  PigLatinVicarious
//
//  Created by Priyanka Chatterjee on 2/26/17.
//  Copyright © 2017 Priyanka Chatterjee. All rights reserved.
//

#include <iostream>

using namespace std;

bool isVowel(char ch) {
    if (ch == 'a' || ch == 'A')
        return true;
    if (ch == 'e' || ch == 'E')
        return true;
    if (ch == 'i' || ch == 'I')
        return true;
    if (ch == 'o' || ch == 'O')
        return true;
    if (ch == 'u' || ch == 'U')
        return true;
    return false;
}

string convPigLatin(const string& s) {
    const char* str = s.c_str();
    int len = s.size();
    string ret_string;
    string temp_string;
    int flag = 0;
    int i;
    
    if (isVowel(str[0])) {
        return s + "yay";
    }
    else {
        for (i = 0; i < len ; i++) {
            if (!isVowel(str[i])) {
                temp_string.push_back(str[i]);
            }
            else {
                flag = 1;
                break;
            }
        }
        for (int j = i; j < len; j++)
            ret_string.push_back(str[j]);
        if (flag == 0)
            return  ret_string + temp_string;
        return  ret_string + temp_string + "ay";
    }
}

string convInputSent(const string& s) {
    string word;
    string ret_string;
    const char* str = s.c_str();
    int i;
    
    for ( i = 0; i < s.size(); i++) {
        if (isalpha(str[i])) {
            word.push_back(str[i]);
        }
        else {
            if (!word.empty())
                word = convPigLatin(word);
            ret_string = ret_string + word + str[i];
            word.clear();
        }
    }
    if (!word.empty())
        word = convPigLatin(word);

    return ret_string + word;
}

