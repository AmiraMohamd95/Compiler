#ifndef PARSER_H
#define PARSER_H
#include "Table_Constructor.h"
#include <bits/stdc++.h>
using namespace std;

class Parser
{
    public:
        Parser();
        void parse(string token);

    protected:
    private:
    stack<string>productions;
    map<string, map <string, vector<string> > > parsing_table;
    map<string,bool> is_terminal;
    Table_Constructor table;
    ofstream output_file;

};

#endif // PARSER_H
