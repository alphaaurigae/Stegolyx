#ifndef OBFUSCATION_H
#define OBFUSCATION_H

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

// ¯\_(ツ)_/¯
unordered_map<char, string> obfuscationMap = {
    {'A', "\xEF\xBB\xBF"}, {'B', "\xE2\x81\xA7"}, {'C', "\xE2\x81\xAF"}, {'D', "\xE2\x82\x99"},
    {'E', "\xE2\x80\x8F"}, {'F', "\xE2\x81\x97"}, {'G', "\xE2\x81\x86"}, {'H', "\xE2\x80\xAC"},
    {'I', "\xE2\x81\xA3"}, {'J', "\xE2\x80\xAE"}, {'K', "\xE2\x81\x8E"}, {'L', "\xE2\x80\xB0"},
    {'M', "\xE2\x80\xB1"}, {'N', "\xE2\x81\xA8"}, {'O', "\xE2\x81\x9E"}, {'P', "\xE2\x80\xB4"},
    {'Q', "\xE2\x80\xB5"}, {'R', "\xE2\x81\x92"}, {'S', "\xE2\x81\xB0"}, {'T', "\xE2\x81\x96"},
    {'U', "\xE2\x81\x88"}, {'V', "\xE2\x81\x9C"}, {'W', "\xE2\x80\xBB"}, {'X', "\xE2\x81\xAD"},
    {'Y', "\xE2\x80\xBD"}, {'Z', "\xE2\x80\xBE"}, 
    {'a', "\xE2\x80\x8A"}, {'b', "\xE2\x81\x94"}, {'c', "\xE2\x81\x83"}, {'d', "\xE2\x80\x83"},
    {'e', "\xE2\x80\x89"}, {'f', "\xE2\x81\x8B"}, {'g', "\xE2\x81\x9A"}, {'h', "\xE2\x81\x95"},
    {'i', "\xE2\x80\xBA"}, {'j', "\xE2\x81\x9D"}, {'k', "\xE2\x80\xB3"}, {'l', "\xE2\x81\x9F"},
    {'m', "\xE2\x80\x84"}, {'n', "\xE2\x81\xA1"}, {'o', "\xE2\x81\xA2"}, {'p', "\xE2\x80\xAD"},
    {'q', "\xEF\xB8\xBC"}, {'r', "\xE2\x81\xA4"}, {'s', "\xE2\x80\xA8"}, {'t', "\xE2\x81\xB7"},
    {'u', "\xE2\x80\x87"}, {'v', "\xE2\x80\xAA"}, {'w', "\xE2\x80\x80"}, {'x', "\xE2\x81\xA6"},
    {'y', "\xE2\x82\x9B"}, {'z', "\xE2\x80\x85"},
    {'0', "\xE2\x80\x8D"}, {'1', "\xE2\x80\x8C"}, {'2', "\xE2\x80\x88"}, {'3', "\xE2\x81\xAE"},
    {'4', "\xE2\x80\x8B"}, {'5', "\xEF\xBF\xBC"}, {'6', "\xE2\x80\xB2"}, {'7', "\xE2\x81\xA9"},
    {'8', "\xE2\x81\xAA"}, {'9', "\xE2\x81\xAB"},
    {' ', "\xE2\x81\x81"}, {'!', "\xE2\x80\x86"}, {'"', "\xE2\x81\xAC"}, {'#', "\xE1\xA0\x8E"},
    {'$', "\xE2\x80\xBC"}, {'%', "\xE2\x80\x81"}, {'&', "\xE2\x80\xB6"}, {'\'', "\xE2\x80\xBF"},
    {'(', "\xE0\x02\x09"}, {')', "\xEF\xA0\x8A"}, {'*', "\xE2\x81\x82"}, {'+', "\xEF\xA0\x8B"},
    {',', "\xE2\x81\x84"}, {'-', "\xE2\x81\x85"}, {'.', "\xE2\x80\xAB"}, {'/', "\xE2\x81\x87"},
    {':', "\xE2\x80\xB9"}, {';', "\xE2\x81\xB6"}, {'<', "\xE2\x81\x8A"}, {'=', "\xE2\x81\x99"},
    {'>', "\xE2\x81\x8C"}, {'?', "\xE2\x81\x8D"}, {'@', "\xE2\x80\xAF"}, {'[', "\xE2\x81\x8F"},
    {'\\', "\xE2\x81\xA5"}, {']', "\xE2\x81\x91"}, {'^', "\xE2\x81\xA0"}, {'_', "\xE2\x81\x93"},
    {'`', "\xE2\x80\x8E"}, {'{', "\xE2\x81\x9B"}, {'|', "\xE2\x80\xB8"}, {'}', "\xE2\x81\x90"},
    {'~', "\xE2\x81\x98"}

// needed for multi line embedding text??? Not working either way yet...
// , {'\n', "\xE2\x80\xA9"}, {'\r', "\xE2\x80\xAA"}
/*
buggy symbols
 ,{'§', "\xE2\x80\xB7"},{'’', "\xE2\x80\x93"},{'¸', "\xE2\x80\x94"},{'°', "\xE2\x80\x83"}, {'¹', "\xC2\xAD"}, {'²', "\xD9\x9C"}
*/

};

// \xF0\xA0\x80\x80 Tag Space (U+E0000


inline string obfuscateText(const string &text) {
    string obfuscatedText;
    for (char c : text) {
        auto it = obfuscationMap.find(c);
        if (it != obfuscationMap.end()) {
            obfuscatedText += it->second;
        } else {
            obfuscatedText += c;
        }
    }
    return obfuscatedText;
}

inline string deobfuscateText(const string &obfuscatedText) {
    string deobfuscatedText;
    string currentInvisible;
    for (char c : obfuscatedText) {
        currentInvisible += c;
        for (const auto &entry : obfuscationMap) {
            if (entry.second == currentInvisible) {
                deobfuscatedText += entry.first;
                currentInvisible.clear();
                break;
            }
        }
        if (currentInvisible.empty()) {
            continue;
        }
    }
    return deobfuscatedText;
}

inline string deobfuscateText(const string &obfuscatedText) {
    string deobfuscatedText;
    string currentInvisible;
    size_t i = 0;

    while (i < obfuscatedText.size()) {
        currentInvisible.clear();
        for (const auto &entry : obfuscationMap) {
            currentInvisible += obfuscatedText[i++];
            if (entry.second == currentInvisible) {
                deobfuscatedText += entry.first;
                break;
            }
        }
        if (currentInvisible.empty()) {
            throw runtime_error("Deobfuscation error: no matching sequence found.");
        }
    }

    return deobfuscatedText;
}

#endif