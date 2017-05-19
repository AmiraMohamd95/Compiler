#include "Input_file.h"
#include <fstream>
#include <iostream>
#include<bits/stdc++.h>

Input_file::Input_file()
{
    start= "";
    log.open("opt_grammar");
    logf.open("final_atomatic_grammer");
}


map<string,vector<vector<string> > > Input_file::read_grammer()
{
    ifstream file("input_file");
    string line;
    getline(file,line);
    string prev = line;
    string next = "" ;

    while(getline(file,line))
    {
        next = line;
        if( next[0] != '#')
        {
            prev = prev + " " + next;
        }
        else
        {
            cout << prev <<endl;
            set_grammar(prev);
            prev = next ;
        }

    }
    cout << prev <<endl;
    set_grammar(prev);
    cout << "map before left factor***************\n";
    //print();
    do_left_recursion() ;
    left_factor();
     print();

    cout << "after left factoring\n";
    set_non_terminal_production();

    //print();
    return grammar;
}


void Input_file:: set_grammar(string line)
{
    //const char *char_line = line.c_str();
    vector<string> tokens = split(line,' ');
    non_terminal = tokens[1];
    if(start.compare("") == 0)
    {
        start = non_terminal;
    }
    is_terminal[non_terminal] = false;
    non_terminals.push_back(non_terminal);
    vector<vector<string> > productions;
    vector<string> sub_production;
    for(int i = 3 ; i < tokens.size() ; i++)
    {
        string current_token = tokens[i];
        if(current_token.compare("|") == 0)
        {
            productions.push_back(sub_production);
            sub_production.clear();
        }
        else
        {
            current_token = check_terminal(current_token);
            sub_production.push_back(current_token);
        }
    }
    productions.push_back(sub_production);
    grammar[non_terminal] = productions;
}
void Input_file::print()
{
    map<string, vector<vector<string>> > m = grammar;
    typedef map<string, vector<vector<string>> >::iterator it_type;
    for(it_type iterator = m.begin(); iterator != m.end(); iterator++)
    {
        logf <<  "##    "<<iterator->first <<"  -->" ;
        vector<vector<string>> v = iterator->second ;
        for(int i = 0 ; i < v.size() ; i++)
        {
            vector <string> v2 = v[i];
            for(int j = 0 ; j < v2.size() ; j++)
            {
                logf << v2[j] <<"   ";
            }
            if(i != v.size() -1)
                logf << "|";
        }
        logf <<endl;
    }
}

vector<string> Input_file::split( string str, char c )
{
    vector<string> result;
    string token = "";
    int len = str.length();
    int i = 0;
    while(i != len)
    {
        if(str[i] == c)
        {
            if(token.compare("") != 0)
            {
                result.push_back(token);
                token = "";
            }
        }
        else
        {
            token+=str[i];
        }
        i++;

    }
    if(token.compare("") != 0)
    {
        result.push_back(token);
    }
    return result;
}

string Input_file::check_terminal(string token)
{
    if(token[0] == '\'')
    {
        token = token.substr(1,token.length()-2);
        is_terminal[token] = true;
    }
    return token;
}

void Input_file::left_factor()
{
    cout << "IN left factoring\n";
    int len = non_terminals.size();
    for(int i = 0  ; i < len ; i++)
    {

        string non_term = non_terminals[i];
        //cout << non_term <<endl;
        vector <int> matched_id;
        vector<vector<string>> prods = grammar.find(non_term)->second;

        bool found_common_prefix = false;
        string prefix;
        // loop through all the productions
        for(int j = 0 ; j < prods.size() && !found_common_prefix ; j++)
        {
            prefix = prods[j][0];
            // search for a prefix in the rest of the productions
            for(int k = j+1 ; k < prods.size() ; k++)
            {
                if(prefix.compare(prods[k][0]) == 0)
                {
                    found_common_prefix = true;
                    matched_id.push_back(k);
                }
            }
            if(found_common_prefix)
            {
                matched_id.push_back(j);
            }
        }

        if(found_common_prefix)
        {

            vector<string> matching_prefix;
            matching_prefix.push_back(prefix);
            vector<string> first = prods[matched_id[0]];
            bool not_matching = false;
            for(int i = 1 ; i < first.size() && !not_matching ; i++)
            {
                string production = first[i];
                for(int j = 1 ; j < matched_id.size()  && !not_matching; j++)
                {
                    if(prods[matched_id[j]].size() < (i+1) || prods[matched_id[j]][i].compare(production) != 0)
                    {
                        not_matching = true;
                    }
                    else
                    {
                        cout << "Matching prefix are : " << production << " : " << prods[j][i] <<endl;
                    }

                }

                if(!not_matching)
                {
                    prefix+=production;
                    matching_prefix.push_back(production);
                }
            }

            do_left_factoring(prefix, matched_id, non_term, matching_prefix);
            i--;
        }
    }
}

void Input_file::set_non_terminal_production()
{
    typedef map<string, vector<vector<string>> >::iterator it_type;
    for(it_type iterator = grammar.begin(); iterator != grammar.end(); iterator++)
    {
        string parent = iterator->first;
        vector<vector<string>> v = iterator->second ;
        for(int i = 0 ; i < v.size() ; i++)
        {
            vector <string> v2 = v[i];
            for(int j = 0 ; j < v2.size() ; j++)
            {
                string token = v2[j];
                if(token.compare("\\L") != 0 && !is_terminal[token])
                {

                    if(non_terminal_production.find(token) == non_terminal_production.end())
                    {
                        set<string> nonTerminals;
                        nonTerminals.insert(parent);
                        non_terminal_production[token] = nonTerminals;
                    }
                    else
                    {
                        non_terminal_production.find(token)->second.insert(parent);
                    }
                }

            }
        }
    }
}

void Input_file::do_left_factoring(string prefix, vector<int> matched_id, string non_term, vector<string> matching_prefix)
{
    cout << "In do left factoring\n";
    vector<vector<string>> prods = grammar.find(non_term)->second;
    vector<vector<string>> new_prods1;
    vector<vector<string>> new_prods2;
    vector<string> factored = matching_prefix;
    factored.push_back(non_term+"_"+prefix);
    for(int i = 0 ; i < matched_id.size() ; i++)
    {

        //remove matching prefix elements in the production
        for(int j = 0 ; j < matching_prefix.size() ; j++ )
        {
            prods[matched_id[i]].erase(prods[matched_id[i]].begin());
        }
        //insert eps
        if(prods[matched_id[i]].size() == 0)
        {
            prods[matched_id[i]].push_back("\\L");
        }
        vector<string> updated_vect = prods[matched_id[i]];
        new_prods2.push_back(updated_vect);

    }
    bool temp[matched_id.size()];
    for(int i = 0 ; i < matched_id.size() ; i++)
    {
        temp[matched_id[i]] = true;
    }
    for(int i = 0 ; i < prods.size() ; i++)
    {
        if(!temp[i])
        {
            new_prods1.push_back(prods[i]);
        }
    }
    new_prods1.push_back(factored);
    grammar[non_term] = new_prods1;
    grammar[non_term+"_"+prefix] = new_prods2;
    non_terminals.push_back(non_term+"_"+prefix);
    is_terminal[non_term+"_"+prefix] = false;
}

map < string,set<string> > Input_file::get_non_terminal_production()
{
    return non_terminal_production;
}

string Input_file::get_start()
{
    return start;
}
map<string,bool> Input_file ::get_is_terminal()
{

    return is_terminal ;
}

void Input_file ::do_left_recursion()
{
    string non_terminal;
    vector <vector <string> > productions;
    vector <vector <string> > productions_;
    bool is_left_recursive = false;
    vector <vector <string> > alpha_productions;
    vector <vector <string> > beta_productions;
    vector <vector <string> > rep_productions;
    vector <string> before;
    vector <string> after;
    bool found;
    vector <string> new_non_terminals;
    for (map <string,bool >::iterator it = is_terminal.begin(); it!=is_terminal.end(); it++)
    {
       if (it->second)
            continue;
        is_left_recursive = false;
        non_terminal = it->first;
        productions = grammar[ it->first];
        bool visited_production[productions.size()];



        fill (visited_production, visited_production+productions.size(), false);

        rep_productions.clear();

        vector <vector <string> > temp_productions = productions;

        for (map <string,bool >::iterator it2  = is_terminal.begin(); it2 != it; it2++)
        {
        if (it2->second)
            continue;
            string nonTerminal = it2->first;
            productions_ = grammar[ it2->first];
            if (!is_terminal[nonTerminal] && nonTerminal.compare(non_terminal) != 0)
            {
                for (int a=0; a < productions.size() ; a++)
                {

                    before.clear();
                    after.clear();
                    found = false;

                    for (int b=0; b < productions[a].size(); b++)
                    {
                        if (!found)
                        {
                            if (productions[a][b].compare(nonTerminal) == 0 && !visited_production[a])
                            {
                                visited_production [a] = true;
                                auto  it_in = find(temp_productions.begin(), temp_productions.end(), productions[a]);
                                if( it_in != temp_productions.end())
                                {
                                    auto index = distance(temp_productions.begin(),  it_in);
                                    temp_productions.erase (temp_productions.begin()+index);
                                }
                                found = true;
                            }
                            else
                            {
                                before.push_back (productions[a][b]);
                            }
                        }
                        else
                        {
                            after.push_back (productions[a][b]);
                        }
                    }
                    if (found)
                    {
                        for (int c=0; c< productions_.size(); c++)
                        {
                            vector <string> new_production;
                            rep_productions.push_back (new_production);
                            int ind = rep_productions.size()-1;
                            for (int e=0; e<before.size(); e++)
                            {
                                rep_productions[ind].push_back(before[e]);
                            }
                            for (int f = 0; f<productions_[c].size(); f++)
                            {
                                rep_productions[ind].push_back(productions_[c][f]);
                            }
                            for (int g=0; g<after.size(); g++)
                            {
                                rep_productions[ind].push_back(after[g]);
                            }
                        }
                    }
                }
            }
        } // end for each non terminal check its existence in productions of the current non terminal
        for (int y=0; y<rep_productions.size(); y++)
        {
            temp_productions.push_back(rep_productions[y]);
        }

        alpha_productions.clear ();
        beta_productions .clear ();

        grammar[non_terminal] = temp_productions;

        productions =  grammar[non_terminal];

        for (int i=0; i<productions.size(); i++)
        {
            if (non_terminal.compare (productions[i][0])==0)
            {
                alpha_productions .push_back(productions[i]);
                is_left_recursive = true ;
            }
            else
            {
                beta_productions .push_back(productions[i]);
            }
        }
        if (is_left_recursive)
        {
            string  new_non_terminal = non_terminal + "_" ;

            // update parents
            new_non_terminals.push_back(new_non_terminal);
           non_terminal_production [new_non_terminal].insert (new_non_terminal);
            non_terminal_production [new_non_terminal] .insert (non_terminal);
            set <string > parents =non_terminal_production [non_terminal];
            for (std::set<string>::iterator i=parents.begin(); i!=parents.end(); ++i){
             non_terminal_production [new_non_terminal] .insert (*i);
            }

            ///beta

            vector <vector <string> > new_orginal_productions;

            for (int j = 0 ; j<beta_productions.size(); j++)
            {
                beta_productions[j].push_back(new_non_terminal);
                new_orginal_productions.push_back(beta_productions[j]);
            }

            grammar[non_terminal] =  new_orginal_productions;

            ///alpha
            vector <vector <string> > new_productions;
            vector <string> alpha;

            for (int k=0 ; k< alpha_productions.size (); k++)
            {
                alpha.clear ();
                for (int l = 1 ; l<alpha_productions[k].size(); l++)
                {
                    alpha.push_back(alpha_productions[k][l]);
                }
                alpha.push_back(new_non_terminal);
                new_productions.push_back(alpha);
            }
            vector <string> eps;
            eps.push_back("\\L");
            new_productions.push_back(eps);
            grammar[new_non_terminal] =  new_productions;
        } // end if left recursive
    } // end for each non_terminal remove left recursion
    // updte non terminal map
    for (int i = 0 ; i < new_non_terminals.size() ; i++)
    {
        is_terminal[new_non_terminals[i]] = false;
        non_terminals.push_back (new_non_terminals[i]);
    }
    // print grammar
    for (map<string, vector<vector<string> > >::iterator it = grammar.begin(); it!=  grammar.end(); it++)
    {
        vector<vector<string> > productions;
        productions = it->second;
        if (is_terminal[it->first])
            continue ;
        log << "##  "  << it->first << " -->  " ;
        for (int i=0; i<productions.size(); i++)
        {
            for (int j=0; j<productions[i].size(); j++)
            {
                log << productions[i][j] << " " ;
            }
            if (i!=productions.size()-1)
            {
                log << " | " ;
            }
        }
        log << endl;
     }
     log <<"nonterminals & terminals  size : " << is_terminal.size () <<endl;
}
