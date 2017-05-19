#ifndef NODE_H
#define NODE_H

#include <list>
#include <stdio.h>
#include <map>

#include <string>
#include <vector>
//#include <bits/stdc++.h>

using namespace std;

class node
{
public:
    node();
    vector < pair <node,string> > get_list();
    bool get_accepted();
    string get_accepted_pattern();
    int get_id();

    void set_id(int _id);
    void put_in_list(pair <node,string> next_weight);
    void set_accepted(bool accept);
    void set_accepted_pattern(string _accepted_pattern);
    void set_list (vector  <pair <node,string> >  n) ;
    virtual ~node();
    vector < pair <node,string> > next ;
    bool accepted ;
    string accepted_pattern ;
    int id ;
    int mapped_group;
protected:

private:

};

#endif // NODE_H
