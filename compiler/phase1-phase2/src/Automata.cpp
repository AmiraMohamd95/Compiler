#include "Automata.h"
#include <iostream>


using namespace std;

Automata::Automata()
{
    //ctor
}
node  Automata::get_start ()
{

    return this->start_node ;
}
void   Automata:: set_start(node n)
{
    this -> start_node = n;
}
node   Automata::get_current ()
{
    return this -> current_node ;
}
void  Automata:: set_current(node n)
{
    this -> current_node = n ;
}

void Automata::set_name(string name)
{
    this ->name = name ;
}
string Automata::get_name()
{
    return this -> name ;
}
map  <int,node>   Automata::get_nodes ()
{

    return this -> nodes ;
}

void  Automata:: set_nodes (map  <int,node>  temp)
{
    this -> nodes = temp ;

}

vector <string> Automata ::get_accepted_patterns()
{

    return this -> automata_acceptance_patterns;

}
void Automata ::set_accepted_patterns (vector <string> patterns)
{

    this ->automata_acceptance_patterns = patterns ;

}


Automata::~Automata()
{
    //dtor
}
