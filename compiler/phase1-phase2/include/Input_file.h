#ifndef INPUT_FILE_H
#define INPUT_FILE_H
#include <bits/stdc++.h>
using namespace std;


class Input_file
{
     public:
        Input_file();
        map<string,bool> get_is_terminal();
        string get_start();
        map <string,vector< vector<string> > > read_grammer();
        vector<string>split( string str, char c );
        map < string,set<string> > get_non_terminal_production();
        void set_grammar(string line);



    protected:
    private:
    ofstream log ;
    ofstream logf ;
    map<string,bool>is_terminal;
    map <string,vector< vector<string> > > grammar;
    string start;
    string check_terminal(string token);
    string non_terminal;
    map < string,set<string> > non_terminal_production;
    void left_factor();
    void do_left_recursion();
    vector<string> non_terminals;
    void set_non_terminal_production();
    void do_left_factoring(string prefix , vector<int> matched_id, string non_term, vector<string> matching_prefix);
    void print();
};

#endif // INPUT_FILE_H
