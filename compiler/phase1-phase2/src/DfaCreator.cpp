#include "DfaCreator.h"
#include <node.h>
#include <Automata.h>
#include <bits/stdc++.h>
#include <string>


DfaCreator::DfaCreator() {}

/* init values we need in dfa and start node */
void DfaCreator::init(Automata automata_nfa, map<string,int> tokens, vector <string> weights)
{

    inputs = weights;
    dfa_inputs = weights;
    tokens_priority = tokens;
    nfa = automata_nfa;
    num_states = 0;
    std::vector <node> init;
    init.push_back(nfa.nodes.find(nfa.get_start().id)->second);
    vector<node> eps_closure = create_eps_closure (init);
    node start ;
    start.id = 0;

    string accepted_pattern = get_accepted_token(eps_closure);
    if(accepted_pattern.length() != 0)
    {
        start.accepted_pattern = accepted_pattern;
        start.accepted = true;
    }

    dstates.push_back (make_pair(eps_closure,start));
    string state_ids = change_state_to_string(eps_closure);
    existed_state[state_ids] = start;
    num_states++;
}

DfaCreator::~DfaCreator()
{
    //dtor
}

/* get vectors of nodes with its eps_closure*/
vector<node> DfaCreator::  create_eps_closure (std::vector <node> nfa_states)
{

    stack<node> nfaStates;

    vector<node> eps_closure;

    bool visited[1000];
    for(int i = 0 ; i < 1000; i++)
    {
        visited[i] = false;
    }
    for (int i=0; i<nfa_states.size(); i++)
    {
        if(! visited[nfa_states[i].id])
        {
            visited[nfa_states[i].id] = true;
            nfaStates.push (nfa.nodes.find(nfa_states[i].id)->second);
            eps_closure.push_back (nfa.nodes.find(nfa_states[i].id)->second);


        }
    }
    while (!nfaStates.empty())
    {
        node n = nfaStates.top();
        nfaStates.pop();

        vector < pair <node,string> > next_states = nfa.nodes.find(n.id)->second.next;

        for (vector<pair<node,string> >::iterator it=next_states.begin(); it != next_states.end(); ++it)
        {
            node n_state = nfa.nodes.find(((*it).first).id)->second;
            string weight = (*it).second;

            if (weight.compare("L")==0)
            {
                if(!visited[n_state.id])
                {
                    visited[n_state.id] = true;
                    node temp = nfa.nodes.find(n_state.id)->second ;
                    eps_closure.push_back (temp);
                    nfaStates.push (temp);
                }
            }
        }
    }
    return eps_closure;
}

/* start the execution of dfa algorithm*/
void DfaCreator::execute()
{
    while (!dstates.empty())
    {
        vector<node> dfa_state = dstates[0].first;
        node old_state = dstates[0].second;
        excluded_input = false;// exclude input for letter
        excluded ="";
        dstates.erase (dstates.begin());
        for (int j = 0; j < inputs.size(); j++)
        {
            string input = inputs[j];
            vector <node>  new_dfa_state;
            for (int i = 0 ; i < dfa_state.size(); i++)
            {
                node n = dfa_state[i];
                int next_size = (nfa.nodes.find(n.id)->second).next.size();

                for(int k = 0; k < next_size; k++ )
                {
                    node n_state = nfa.nodes.find(n.next[k].first.id)->second;
                    string weight = n.next[k].second;

                    /*****************handle input***********************/
                    if (weight.compare(input)==0 )
                    {
                        new_dfa_state.push_back (n_state);
                    }
                    else if((weight.compare("a-z") == 0) && (input.size() ==1) &&
                            (input[0] >= 97 && input[0] <=122 ))
                    {

                        excluded_input = true;
                        if (excluded.find(input) == std::string::npos)
                        {
                            excluded += input+",";
                        }
                        new_dfa_state.push_back (n_state);
                    }

                }

            }
            if (new_dfa_state.empty())
            {
                continue;
            }
            /************ change value of input*****************/
            if(excluded_input && input.compare("a-z")==0)
            {

                input = "$"+excluded;
                input = input.substr(0, input.size()-1);
                if ( existed_input.find(input) == existed_input.end() )
                {
                    dfa_inputs.push_back(input);
                    existed_input[input] = true;
                }

            }

            vector<node> state_with_eps_closure = create_eps_closure(new_dfa_state);
            string new_state_id = change_state_to_string(state_with_eps_closure);
            int state_index = compare_states (new_state_id);
            if (state_index == -1)
            {
                node new_state ;
                new_state.id = num_states;
                string accepted_pattern = get_accepted_token(state_with_eps_closure);
                if(accepted_pattern.length() != 0)
                {

                    new_state.accepted_pattern = accepted_pattern;
                    new_state.accepted = true;
                    myfile << "acc_p: " << new_state.accepted_pattern << endl;
                }

                old_state.next.push_back (make_pair(new_state,input));
                dstates.push_back (make_pair(state_with_eps_closure,new_state));
                existed_state[new_state_id] = new_state;
                num_states++;
            }
            else
            {
                node n = existed_state.find(new_state_id)->second;
                old_state.next.push_back (make_pair(n,input));
            }

        }
        dfa.nodes[old_state.id]= old_state;
        if(dfa.nodes.size()==1)
        {
            dfa.set_start(old_state);
        }
    }
}

/* compare if the new state is already existed or not */
int  DfaCreator::compare_states (string state)
{

    if ( existed_state.find(state) == existed_state.end() )
        return -1;
    return 1;
}


/* return the string of the higher accepted token*/
string  DfaCreator::get_accepted_token (vector<node> states)
{
    int pattern_no = -1;
    string highest_priority ="";
    for(int i = 0; i< states.size(); i++)
    {
        int accepted = -1;
        node n = states[i];
        if(n.accepted)
        {
            accepted = tokens_priority.find(n.accepted_pattern)->second;
            if(pattern_no == -1 || accepted < pattern_no)
            {
                highest_priority = n.accepted_pattern;
                pattern_no = accepted;
            }
        }

    }
    return highest_priority;
}

/*  return a string that contain the nodes in ascending order of the given states
/*  to check if its already existed or not */
string DfaCreator::change_state_to_string(vector<node>states)
{
    set <int> node_set;
    string formed_nodes = "";
    for (int i = 0 ; i < states.size(); i++)
    {
        node_set.insert (states[i].id);
    }

    for(set<int>::iterator it1=node_set.begin(); it1!=node_set.end(); ++it1)
    {
        int node_id = *it1;
        stringstream ss;
        ss << node_id;
        string str = ss.str();
        formed_nodes += str;
    }
    return formed_nodes;
}
