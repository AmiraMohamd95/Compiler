#include "node.h"

#include <iostream>
#include <fstream>

node::node()
{
    accepted = false ;
}

node::~node()
{

//dtor
}

vector < pair <node,string> > node ::get_list()
{

    return this->next ;
}
bool node :: get_accepted()
{
    return this->accepted ;
}
string node ::get_accepted_pattern(){

     return this -> accepted_pattern ;
}
int node ::get_id()
{
    return this->id ;
}

void node ::set_id(int _id)
{
this->id = _id ;
}
void node ::put_in_list(pair <node,string> next_weight)
{
    this->next.push_back (next_weight);
    //cout <<"size" << next.size() <<endl ;
}
void node:: set_accepted(bool accept)
{
    this->accepted = accept ;
}
void node :: set_accepted_pattern(string _accepted_pattern)
{

     this->accepted_pattern  = _accepted_pattern ;
}
void node ::set_list (vector <pair <node ,string> > n) {
    this->next = n;
}

