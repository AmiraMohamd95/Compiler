#ifndef MINIMIZATION_H
#define MINIMIZATION_H
#include <DfaCreator.h>
#include <Automata.h>
#include <node.h>
class Minimization
{
    public:

    Minimization();
    virtual ~Minimization();
    void initialize_states();
    void minimize_states();
    void initialize_groups(node start);
    vector<vector<node> > partition_groups (vector<vector<node> >  new_groups, string input);
    Automata get_minimized_dfa();
    void init (Automata dfa, vector<string> inputs_list, map<string,int> accep_patterns);
    void construct_automata();
    void initialize_groups();
    void print_automata_itr (Automata aut);
    void print_automata_rec(node n,Automata min_dfa);

    protected:

    private:

    Automata minimized_dfa;
    Automata initialized_dfa;
    vector< vector<node> > all_states;
    map<string,int> accepted_patterns;
    vector<string>  inputs;
    ofstream aut;
    bool vis1[100];
    bool vis2[100];

};

#endif // MINIMIZATION_H
