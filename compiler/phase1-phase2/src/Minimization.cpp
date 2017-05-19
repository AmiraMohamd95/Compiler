#include "Minimization.h"
#include <node.h>
#include <Automata.h>
#include <bits/stdc++.h>
Minimization::Minimization()
{
    aut.open ("final_minimized_auto.txt");
}
Automata Minimization::get_minimized_dfa()
{
    return minimized_dfa;
}
Minimization::~Minimization() {}
void Minimization::print_automata_itr (Automata automata)
{

    map<int,node> nodes = automata.get_nodes();
    aut << "size: " << nodes.size() << endl;
    for (std::map<int,node>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        vector <pair<node,string> > next = (it->second).get_list () ;
        for (int j = 0 ; j < next.size() ; j++)
        {
            aut << it->first  <<" ";
            aut << next[j].second  <<" ";
            aut << next[j].first.get_id() <<endl;
        }
        if ( it->second.get_accepted())
        {
            string s = (it->second).get_accepted_pattern() ;
            aut  << it->second.get_id()<<" accepted node!!! ::"<<s << endl;
        }
    }
}

void Minimization::print_automata_rec (node n, Automata dfa)
{
    vis2[n.id] = true;
    vector <pair <node, string> > next_list = n.get_list();
    for(int i = 0 ; i < next_list.size(); i++)
    {
        node next = dfa.get_nodes().find (next_list[i].first.id)->second;
        string w = next_list[i].second;
        aut<<n.id<< "," <<next.id <<" -- "<< w ;
        if (next.accepted && next.accepted_pattern.length() != 0){
            aut << " --- " << next.accepted_pattern << endl;
        }else {
            aut << endl;
        }
        if (!vis2[next.id])
        {
            print_automata_rec(dfa.get_nodes().find(next.id)->second,dfa);
        }
    }
}


void Minimization::construct_automata()
{
    for (int i=0; i<all_states.size(); i++)
    {
        node rep_node = (minimized_dfa.nodes.find(i))->second;
        node n = initialized_dfa.nodes.find(all_states[i][0].id)->second;
        vector < pair <node,string> > next = n.next;
        for (int j=0; j<next.size(); j++)
        {
            node to = next[j].first;
            node next_node = initialized_dfa.nodes.find(to.id)->second;
            int mapped_grp = next_node.mapped_group;
            rep_node.next.push_back(make_pair(minimized_dfa.nodes.find (mapped_grp)->second , next[j].second));
        }
        minimized_dfa.nodes[i] = rep_node;
    }
    aut << "rec printing " << endl;
    aut << "size " << minimized_dfa.nodes.size()<< endl;
    print_automata_rec(minimized_dfa.nodes.find(minimized_dfa.get_start().id)->second, minimized_dfa);
}

void Minimization::init (Automata dfa, vector<string> inputs_list, map<string,int> accep_patterns)
{
    initialized_dfa = dfa;
    inputs = inputs_list;
    accepted_patterns = accep_patterns;
}
vector<vector<node> > Minimization::partition_groups (vector<vector<node> > new_groups, string input)
{
    vector <vector <node> > new_grps;
    map <int,vector<node> > mapped_groups;
    int contained_grp =-1;
    bool next_found = false;

    for (int m = 0; m < new_groups.size(); m++)
    {
        mapped_groups.clear();
        for (int k=0; k<new_groups[m].size(); k++)
        {
            node s = initialized_dfa.nodes.find (new_groups[m][k].id)->second;
            node next_node;
            vector <pair<node, string> > next = s.next;
            contained_grp=-1;
            next_found = false;
            if (next.size () !=0)
            {
                for (int q=0; q<next.size(); q++)
                {
                    if (input.compare(next[q].second) == 0)
                    {

                        next_node  = initialized_dfa.nodes.find (next[q].first.id)->second;
                        next_found = true;



                        break;
                    }
                }
                if (next_found) {
                    for (int f=0; f<all_states.size()&&contained_grp==-1; f++)
                    {
                        for (int r=0; r<all_states[f].size()&&contained_grp==-1; r++)
                        {
                            if (all_states[f][r].id==next_node.id)
                            {
                                contained_grp = f;

                            }
                        }
                    }

                }
            }
            mapped_groups[contained_grp].push_back(s);
        }
        for (map<int, vector<node> >::iterator it = mapped_groups.begin(); it!=  mapped_groups.end(); it++)
        {
            new_grps.push_back ((it)->second);
        }
    }
    return new_grps;
}
void Minimization::minimize_states()
{
    initialize_states ();
    initialize_groups();
    vector<vector<node> >  init;
    vector <vector<node> > new_states;
    vector <vector<node> > new_groups;

    while (true)
    {
        new_states.clear();
        for (int i=0; i < all_states.size(); i++)
        {
            init.clear();
            init.push_back (all_states[i]);
            new_groups.clear();


            for (int j=0; j<inputs.size(); j++)
            {
                if (j==0)
                {
                    new_groups = partition_groups(init, inputs[j]);
                }
                else
                {
                    new_groups = partition_groups(new_groups, inputs[j]);
                }
            }
            for (int k=0; k<new_groups.size(); k++)
            {
                new_states.push_back(new_groups[k]);
            }
        }
        if (new_states.size() == all_states.size())
        {
            break;
        }
        else
        {
            all_states = new_states;
        }
    }
    for (int i=0; i<all_states.size(); i++)
    {
        node n;
        n.id = i;
        for (int k=0; k<all_states[i].size(); k++)
        {
            node old_node = initialized_dfa.nodes.find(all_states[i][k].id)->second;
            old_node.mapped_group =  i;
            initialized_dfa.nodes[old_node.id] = old_node;
            all_states[i][k] = old_node;
            if (all_states[i][k].accepted)
            {
                n.accepted_pattern = all_states[i][k].accepted_pattern;
                n.accepted = true;
            }
            if (old_node.id == initialized_dfa.get_start().id)
            {
                minimized_dfa.set_start(n);
            }
        }
        minimized_dfa.nodes[i] = n;

    }
    construct_automata();
}
void Minimization::initialize_states()
{
    vector<node>group;
    all_states.push_back(group);
    int tokens_num = accepted_patterns.size();
    for(int i = 0 ; i < tokens_num ; i++)
    {
        vector<node>group;
        all_states.push_back(group);
    }

}

void Minimization::initialize_groups()
{
    for (std::map<int,node>::iterator it =initialized_dfa.nodes.begin(); it!=initialized_dfa.nodes.end(); ++it)
    {
        node n_state = it->second;
        if(n_state.accepted)
        {
            int mapped_group =  accepted_patterns.find(n_state.accepted_pattern)->second;
            n_state.mapped_group = mapped_group;
            all_states[mapped_group].push_back(n_state);
        }
        else
        {
            n_state.mapped_group = 0;
            all_states[0].push_back(n_state);
        }
    }
}

