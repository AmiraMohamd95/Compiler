#ifndef AUTOMATA_H
#define AUTOMATA_H
#include <list>
#include <stdio.h>
#include <map>
#include <string.h>
#include <bits/stdc++.h>
#include "node.h"


class Automata
{
public:
    Automata();
    virtual ~Automata();
    node get_start ();
    void set_start(node n);
    node get_current ();
    void set_current(node n);
    void set_name(string name);
    string get_name();
    map  <int ,node>   get_nodes ();
    void set_nodes (map  <int ,node> temp);
    vector <string> get_accepted_patterns() ;
    void set_accepted_patterns (vector <string> patterns);
    map  <int ,node>  nodes;


protected:

private:
    string name;
    node start_node;
    node current_node;
    vector <string> automata_acceptance_patterns ;

};

#endif // AUTOMATA_H
