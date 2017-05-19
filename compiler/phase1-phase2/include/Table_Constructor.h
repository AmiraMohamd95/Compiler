#include <bits/stdc++.h>
#ifndef TABLE_CONSTRUCTOR_H
#define TABLE_CONSTRUCTOR_H

using namespace std;

class Table_Constructor
{
public:

    Table_Constructor();
    virtual ~Table_Constructor();
    void find_follows();
    void find_firsts();
    void get_first_rec (string symbol);
    void get_follow_rec (string symbol);

    bool construct_table ();
    void init2 (map <string, vector<vector<string> > > _grammar,map <string, bool> _isTerminal, string _start_symbol, map <string,set <string> > _symbols_parent);
    void init ();
    map<string, map <string, vector<string> > > get_parsing_table();
    string get_start_symbol();
    map <string, bool> get_is_terminal();
    map <string, set<string> > get_first_map();



protected:

private:

    ofstream log;
    string start_symbol;
    map <string, vector<vector<string> > > grammar;
    map <string, bool> isTerminal;
    map <string, set<string> > first_map;
    map <string, set<string> > follow_map;
    map <string, map <string, vector<string> > > parsing_table;
    map <string, set <string> > symbol_parents ;

};

#endif // TABLE_CONSTRUCTOR_H
