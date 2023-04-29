#include <iostream>
#include <cctype>
#include <fstream>
#include "lex.h"
#include <string>
#include <sstream>
#include <regex>
using namespace std;

ostream& operator<<(ostream& out, const LexItem& tok)
{
    switch (tok.GetToken())
    {
    case WRITELN:
        out << "WRITELN" << endl;
        break;
    case IF:
        out << "IF" << endl;
        break;
    case ELSE:
        out << "ELSE" << endl;
        break;
    case IDENT:
        out << "IDENT(" << tok.GetLexeme() << ")" << endl;
        break;
    case NIDENT:
        out << "NIDENT(" << tok.GetLexeme() << ")" << endl;
        break;
    case SIDENT:
        out << "SIDENT(" << tok.GetLexeme() << ")" << endl;
        break;
    case ICONST:
        out << "ICONST(" << tok.GetLexeme() << ")" << endl;
        break;
    case RCONST:
        out << "RCONST(" << tok.GetLexeme() << ")" << endl;
        break;
    case SCONST:
        out << "SCONST(" << tok.GetLexeme() << ")" << endl;
        break;
    case PLUS:
        out << "PLUS" << endl;
        break;
    case MINUS:
        out << "MINUS" << endl;
        break;
    case MULT:
        out << "MULT" << endl;
        break;
    case DIV:
        out << "DIV" << endl;
        break;
    case EXPONENT:
        out << "EXPONENT" << endl;
        break;
    case ASSOP:
        out << "ASSOP" << endl;
        break;
    case NEQ:
        out << "NEQ" << endl;
        break;
    case NGTHAN:
        out << "NGTHAN" << endl;
        break;
    case NLTHAN:
        out << "NLTHAN" << endl;
        break;
    case CAT:
        out << "CAT" << endl;
        break;
    case SREPEAT:
        out << "SREPEAT" << endl;
        break;
    case SEQ:
        out << "SEQ" << endl;
        break;
    case SGTHAN:
        out << "SGTHAN" << endl;
        break;
    case SLTHAN:
        out << "SLTHAN" << endl;
        break;
    case COMMA:
        out << "COMMA" << endl;
        break;
    case LPAREN:
        out << "LPAREN" << endl;
        break;
    case RPAREN:
        out << "RPAREN" << endl;
        break;
    case LBRACES:
        out << "LBRACES" << endl;
        break;
    case RBRACES:
        out << "RBRACES" << endl;
        break;
    case SEMICOL:
        out << "SEMICOL" << endl;
        break;
    default:
        break;
    }
    return out;
}



LexItem id_or_kw(const string& lexeme, int linenum)
{
    if (lexeme == "writeln" || lexeme == "WRITELN")
    {
        return LexItem(WRITELN, lexeme, linenum);
    }

    if (lexeme == "if" || lexeme == "IF")
    {
        return LexItem(IF, lexeme, linenum);
    }

    if (lexeme == "else" || lexeme == "ELSE")
    {
        return LexItem(ELSE, lexeme, linenum);
    }

    if (lexeme[0] == '@')
    {
        return LexItem(SIDENT, lexeme, linenum);
    }

    if (lexeme[0] == '$')
    {
        return LexItem(NIDENT, lexeme, linenum);
    }

    return LexItem(IDENT, lexeme, linenum);
}

LexItem getNextToken(istream& in, int& linenum)
{
    enum tokState { START, INID, INSTRING, ININT, INREAL, INCOMMENT };
    tokState lexState = START;
    string lexeme = ""; // made lexeme start off as ""
    // got rid of bool isFloat = false; cuz it was pointless and redundant
    char ch;

    while (in.get(ch))
    {
        switch (lexState)
        {
        case START:
            if (isspace(ch))
            {
                if (ch == '\n')
                {
                    linenum++;
                }
                break;
            }
            if (ch == '#')
            {
                lexState = INCOMMENT;
                break;
            }
            else if (ch == '\'')
            {
                lexState = INSTRING;
                break;
            }

            lexeme += ch; // change lexeme = ch; into this
            if (isalpha(ch) || ch == '@' || ch == '$' || ch == '_')
            {
                lexState = INID;
            }
            else if (isdigit(ch))
            {
                lexState = ININT;
            }
            else if (ch == '+')
            {
                return LexItem(PLUS, lexeme, linenum); // code now instantly returns the operators instead
            }
            else if (ch == '-')
            {
                if (in.peek() == 'e' || in.peek() == 'E')
                {
                    in.get(ch);
                    if (in.peek() == 'q' || in.peek() == 'Q') {
                        lexeme += ch;
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(SEQ, lexeme, linenum);
                    }
                    else {
                        in.seekg(-1, ios::cur);
                        return LexItem(MINUS, lexeme, linenum);
                    }
                }
                else if (in.peek() == 'l' || in.peek() == 'L')
                {
                    in.get(ch);
                    if (in.peek() == 't' || in.peek() == 'T') {
                        lexeme += ch;
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(SLTHAN, lexeme, linenum);
                    }
                    else {
                        in.seekg(-1, ios::cur);
                        return LexItem(MINUS, lexeme, linenum);
                    }
                }
                else if (in.peek() == 'g' || in.peek() == 'G')
                {
                    in.get(ch);
                    if (in.peek() == 't' || in.peek() == 'T') {
                        lexeme += ch;
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(SGTHAN, lexeme, linenum);
                    }
                    else
                    {
                        in.seekg(-1, ios::cur);
                        return LexItem(MINUS, lexeme, linenum);
                    }
                }
                else
                {
                    return LexItem(MINUS, lexeme, linenum);
                }
            } // end minus
            else if (ch == '*')
            {
                if (in.peek() == '*')
                {
                    in.get(ch);
                    lexeme += ch;
                    return LexItem(SREPEAT, lexeme, linenum);
                }
                else
                {
                    return LexItem(MULT, lexeme, linenum);
                }

            }
            else if (ch == '/')
            {
                return LexItem(DIV, lexeme, linenum);
            }
            else if (ch == '=')
            {
                if (in.peek() == '=')
                {
                    in.get(ch);;
                    return LexItem(NEQ, lexeme, linenum);
                }
                else
                {
                    return LexItem(ASSOP, lexeme, linenum);
                }
            }
            else if (ch == '>')
            {
                return LexItem(NGTHAN, lexeme, linenum);
            }
            else if (ch == '<')
            {
                return LexItem(NLTHAN, lexeme, linenum);
            }
            else if (ch == ',')
            {
                return LexItem(COMMA, lexeme, linenum);
            }
            else if (ch == '(')
            {
                return LexItem(LPAREN, lexeme, linenum);
            }
            else if (ch == ')')
            {
                return LexItem(RPAREN, lexeme, linenum);
            }
            else if (ch == '{') // you accidentally put '['
            {
                return LexItem(LBRACES, lexeme, linenum);
            }
            else if (ch == '}') // you accidentally put ']'
            {
                return LexItem(RBRACES, lexeme, linenum);
            }
            else if (ch == '^')
            {
                return LexItem(EXPONENT, lexeme, linenum);
            }
            else if (ch == '.')
            {
                // got rid of EVERYTHING here cuz it was useless
                return LexItem(CAT, lexeme, linenum);
            }
            else if (ch == ';')
            {
                return LexItem(SEMICOL, lexeme, linenum);
            }
            else // removed that "hard code" thing you have so now it's just this
            {
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case INID:
            if (regex_match(lexeme + ch, regex("[_a-zA-Z]+[_a-zA-Z0-9]+")) || regex_match(lexeme + ch, regex("^\\@[_a-zA-Z][_a-zA-Z0-9]*")) || regex_match(lexeme + ch, regex("^\\$[_a-zA-Z][_a-zA-Z0-9]*")))
            {
                lexeme += ch;
            }
            else
            {
                in.putback(ch);
                return id_or_kw(lexeme, linenum);
            }
            break;

        case INSTRING:
            if (ch == '\n') // if it hits a new line, then the string was never finished
            {
                return LexItem(ERR, "\'" + lexeme, linenum);
            }
            // Check lexeme for finished string
            else if (ch == '\'') {
                return LexItem(SCONST, lexeme, linenum);
            }
            else // builds string
            {
                lexeme += ch;
            }
            break;
        case ININT:
            if (isdigit(ch))
            {
                lexeme += ch;
            }
            else if (ch == '.')
            {
                lexeme += ch;
                lexState = INREAL;
                // got rid of in.putback(ch); so I could get rid of other code, plus this was causing the problem
            }
            else
            {
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }
            break;

        case INREAL:
            if (ch == '.')
            {
                lexeme += ch;
                return LexItem(ERR, lexeme, linenum);
            }
            else if (isdigit(ch))
            {
                lexeme += ch;
            }
            else
            {
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }
            break;
        case INCOMMENT:
            if (ch == '\n')
            {
                linenum++;
                lexState = START;
            }
            break;
        }

    }

    if (in.eof())
    {
        return LexItem(DONE, "", linenum);
    }
    else
    {
        return LexItem(ERR, lexeme, linenum);
    }
}