#include <iostream>
#include <cctype>
#include <fstream>
#include "lex.h"
#include <string>
#include <sstream>
#include <algorithm> 
#include <list>
#include<string.h>
using namespace std;


int main(int argc, char* argv[])
{

    ifstream file;


    int fileCount = 0;
    bool v{ false }, ident{ false }, nconst{ false }, sconst{ false };

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            v = true;
        }
        else if (strcmp(argv[i], "-nconst") == 0)
        {
            nconst = true;
        }
        else if (strcmp(argv[i], "-sconst") == 0)
        {
            sconst = true;
        }
        else  if (strcmp(argv[i], "-ident") == 0)
        {
            ident = true;
        }
        else if (argv[i][0] == '-')
        {
            cerr << "UNRECOGNIZED FLAG {" << argv[i] << "}" << endl;
            exit(-1);
        }
        else {
            fileCount++;
        }
    }

    int numOfLines = 0;
    string line;
    if (fileCount == 1) {
        file.open(argv[1]);
        if (file.is_open()) {
            if (file.peek() == ifstream::traits_type::eof())
            {
                while (getline(file, line))
                {
                    numOfLines++;
                }
                cout << "Lines: " << numOfLines << endl;
                cout << "Empty File." << endl;
                exit(-1);
            }
        }
        else if (!file.is_open())
        {
            cout << "CANNOT OPEN THE FILE " << argv[1] << endl;
            exit(-1);
        }
    }

    if (fileCount < 1)
    {
        cerr << "NO SPECIFIED INPUT FILE." << endl;
        return 0;
    }
    else if (fileCount > 1)
    {
        cerr << "ONLY ONE FILE NAME IS ALLOWED." << endl;
        exit(-1);
    }

    string lexeme;
    LexItem tok;
    int linenum = 0;

    int totalTokens = 0;
    int totalIdents = 0;
    int totalNums = 0;
    int totalStrings = 0;

    list<string> idents;
    list<string> nums;
    list<string> stringConsts;

    while ((tok = getNextToken(file, linenum)) != DONE)
    {
        lexeme = tok.GetLexeme();
        if (tok == ERR)
        {
            cout << "Error in line " << (tok.GetLinenum() + 1) << " (" << lexeme << ')' << endl;
            exit(-1);
        }
        totalTokens++; // code NEEDS to increment the counters REGARDLESS if the flags are true or false
        if (v == true)
        {
            // had to move everything here up there ^
            cout << tok;
        }

        // removed all the if statements for the flags being true or else it would mess up the counter
        if ((tok == IDENT || tok == NIDENT || tok == SIDENT) && find(idents.begin(), idents.end(), lexeme) == idents.end())
        {
            totalIdents++;
            idents.push_back(lexeme);
        }

        if ((tok == ICONST || tok == RCONST) && find(nums.begin(), nums.end(), lexeme) == nums.end())
        {
            totalNums++;
            bool addedToList = false;
            list<string> ::iterator it;

            for (it = nums.begin(); it != nums.end(); it++)
            {
                if (stod(lexeme) < stod(*it))
                {
                    int i = 0;
                    while (lexeme[i] == '0') { i++; } // removes leading zeros
                    if (lexeme[i] != '.')
                    {
                        lexeme.erase(0, i); // if num is not a double, removes all leading zeros
                    }
                    else
                    {
                        lexeme.erase(0, i - 1);// if decimal found removes every leading zero except one infront of decimal
                    }
                    nums.insert(it, 1, lexeme); //at position it insert one instance/copy of lexeme
                    addedToList = true;
                    break; //escape for-loop
                }
            }
            if (!addedToList)
            {
                nums.push_back(lexeme);
            }
        }
        if (tok == SCONST && find(stringConsts.begin(), stringConsts.end(), lexeme) == stringConsts.end())
        {
            totalStrings++;
            stringConsts.push_back(lexeme);
        }
    }

    cout << "\nLines: " << linenum << endl;
    cout << "Total Tokens: " << totalTokens << endl;
    cout << "Identifiers: " << totalIdents << endl;
    cout << "Numbers: " << totalNums << endl;
    cout << "Strings: " << totalStrings << endl;

    if (ident == true)
    {
        list<string> ::iterator it;
        idents.sort();
        cout << "IDENTIFIERS:" << endl;
        for (it = idents.begin(); it != idents.end(); it++)
        {
            if (it == idents.begin())
            {
                cout << *it;
                continue;
            }
            cout << ", " << *it;
        }
        cout << endl;
    }
    if (nconst == true)
    {
        list<string> ::iterator it;
        cout << "NUMBERS:" << endl;
        for (it = nums.begin(); it != nums.end(); it++)
        {
            cout << *it << endl;
        }
    }
    if (sconst == true)
    {
        list<string> ::iterator it;
        stringConsts.sort();
        cout << "STRINGS:" << endl;
        for (it = stringConsts.begin(); it != stringConsts.end(); it++)
        {
            cout << '\'' << *it << '\'' << endl;
        }
    }


    return 0;
}
