#ifndef DFACREATOR_H
#define DFACREATOR_H
#include <bits/stdc++.h>
#include <node.h>
#include <Automata.h>


using namespace std;

class DfaCreator
{
public:
    DfaCreator();
    virtual ~DfaCreator();
    vector <node> create_eps_closure (vector <node> nfa_states);
     map<string,int> tokens_priority;
     Automata dfa;
    // run subset construction algorithm
    int compare_states (string state);
    string get_accepted_token(vector<node> states);
    void print_automata(node n,Automata dfa);
    string change_state_to_string(vector<node>formed_state);
    void  execute ();
    void init (Automata automata_nfa, map<string,int>tokens, vector <string> weights);
     vector<string>dfa_inputs;
    ofstream myfile;

protected:
private:
    Automata nfa;

   // ofstream auto_file;///////////////
    bool vis[100];//////////////
    node dfa_head;
    bool excluded_input;
    bool letter_exist;
    string excluded;
    vector <string> inputs;
    vector < pair<vector <node>,node> >   dstates;
    map<string,node>existed_state;
    int num_states;
    map<string,bool>existed_input;

    //map < vector<node>,vector<pair<vector<node>,char*> > > transition_table;
    //map <int, vector <pair <int, char* > > > dtran;
};

#endif // DFACREATOR_H
