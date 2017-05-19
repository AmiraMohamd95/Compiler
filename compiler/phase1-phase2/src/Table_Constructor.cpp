#include "Table_Constructor.h"
#include "Input_file.h"

Table_Constructor::Table_Constructor()
{
    //ctor
    log.open ("test_table_construction.txt");
}

Table_Constructor::~Table_Constructor()
{
    //dtor
    log.close();
}
void Table_Constructor::init ()
{
    Input_file grammar_constructor;
    grammar =grammar_constructor.read_grammer();
    start_symbol =grammar_constructor.get_start();
    isTerminal =grammar_constructor.get_is_terminal();
    symbol_parents = grammar_constructor.get_non_terminal_production() ;

    find_firsts() ;
    find_follows() ;
    construct_table() ;
}
void Table_Constructor::init2 (map <string, vector<vector<string> > > _grammar,map <string, bool> _isTerminal, string _start_symbol,  map <string,set <string> > _symbol_parents)
{
    grammar = _grammar;
    isTerminal = _isTerminal;
    start_symbol = _start_symbol;
    symbol_parents = _symbol_parents ;

}
void Table_Constructor::get_first_rec (string symbol)
{
    if (isTerminal [symbol])
    {
        first_map[symbol].insert (symbol);
    }
    else  if (symbol.compare ("\\L") == 0)
    {
        first_map[symbol].insert ("\\L");
    }
    // if non terminal find its productions
    else
    {
        vector <vector <string> > productions = grammar [symbol];
        for (int i=0 ; i <productions.size() ; i++)
        {
            if (isTerminal [productions[i][0]])
            {

                    first_map[symbol].insert (productions[i][0]);
            }
            // if nonterminal take its first  X=Y1Y2Y3............Yn
            else
            {
                bool contains_eps =true  ;
                for (int j=0 ; j < productions[i].size() && contains_eps; j++)
                {
                    contains_eps = false;
                    if (first_map [productions[i][j]] .size() == 0)
                    {
                        get_first_rec(productions[i][j]);
                    }
                    set<string> first =first_map [productions[i][j]];
                     for (std::set<string>::iterator it=first.begin(); it!=first.end(); ++it)
                    {
                        if (*it!= "\\L")
                        {

                            first_map[symbol].insert (*it);
                        }
                        else
                        {
                            contains_eps = true ;
                        }
                    }
                }
                // it handles also x-> /L
                if (contains_eps)
                {
                    first_map[symbol].insert ("\\L");
                }
            }
        }
    }
}
void Table_Constructor::get_follow_rec (string symbol)
{
    if (symbol.compare (start_symbol)==0)
    {
        follow_map[symbol].insert ("$");
    }
    set <string > parents =symbol_parents [symbol];
    set <string > depends_on;
    for (std::set<string>::iterator it=parents.begin(); it!=parents.end(); ++it)
    {
        string parent = *it;
        vector <vector <string> > productions = grammar[parent];
        for (int i=0; i<productions.size(); i++)
        {
            for (int j = 0; j < productions[i].size(); j++)
            {
                if (productions[i][j].compare(symbol) == 0)
                {
                    bool contain_eps = false;
                    // it's not the last one
                    if ( j != productions[i].size()-1 )
                    {
                        string followed_symbol = productions[i][j+1];
                        if (isTerminal[followed_symbol])
                        {
                            follow_map[symbol].insert(followed_symbol);
                        }
                        else
                        {
                            contain_eps = true ;
                            for (int fol = j+1 ; fol < productions[i].size() &&contain_eps ; fol ++)
                            {
                                followed_symbol = productions[i][fol];
                                contain_eps = false ;
                              //  for (int k=0; k<first_map[followed_symbol].size(); k++)
                                //{
                            for (std::set<string>::iterator it=first_map[followed_symbol].begin(); it!=first_map[followed_symbol].end(); ++it)
                                {

                                    if ((*it).compare("\\L") == 0)
                                    {
                                        contain_eps = true ;
                                    }
                                    else
                                    {
                                        follow_map[symbol].insert(*it);
                                    }
                                }
                            }
                        }
                    }
                    // if it's last one
                    if (j == productions[i].size()-1 || contain_eps)
                    {
                        if(follow_map[parent].size()!=0)
                        {
                            for (std::set<string>::iterator it=follow_map[parent].begin(); it!=follow_map[parent].end(); ++it)
                            {
                                follow_map[symbol].insert (*it);
                            }
                        }
                        else if (parent.compare (symbol) !=0)
                        {
                            //depends_on.insert (parent);
                            get_follow_rec(parent);
                            for (std::set<string>::iterator it=follow_map[parent].begin(); it!=follow_map[parent].end(); ++it)
                            {
                                follow_map[symbol].insert (*it);
                            }
                        }
                    }
                }
            }
        }
    }
    /* for (set<string>::iterator it = depends_on.begin(); it != depends_on.end(); it++)
     {
         string parent = *it;
         get_follow_rec(parent);
         for (std::set<string>::iterator it=follow_map[parent].begin(); it!=follow_map[parent].end(); ++it)
         {
             follow_map[symbol].insert (*it);
         }
     }*/
}

void Table_Constructor::find_firsts()
{
    log << "---------  first of each symbol ------------------" <<endl ;
    get_first_rec("\\L") ;
    for (std::map<string,bool>::iterator it=isTerminal.begin(); it!=isTerminal.end(); ++it)
    {
        string symbol = it->first ;
        if ((first_map[symbol]).size ()==0)
        {
            get_first_rec(symbol);
        }
        log << "first of " << symbol << ": "  ;
        for (std::set<string>::iterator it=first_map[symbol].begin(); it!=first_map[symbol].end(); ++it)
        {
            log <<*it<< " ";
        }
        log << endl;
    }
}

void Table_Constructor::find_follows()
{
    log << "---------  follow of each symbol ------------------" <<endl ;
    for (std::map<string,bool>::iterator it=isTerminal.begin(); it!=isTerminal.end(); ++it)
    {
        string symbol = it->first ;

        if ( !isTerminal[symbol] && (follow_map[symbol]).size () == 0)
        {
            get_follow_rec(symbol);

        }
        if (!isTerminal[symbol])
        {
            log << "follow of " << symbol << ": " ;
            for (std::set<string>::iterator it=follow_map[symbol].begin(); it!=follow_map[symbol].end(); ++it)
            {
                log <<*it<< " ";
            }
            log << endl;
        }
    }
}
bool Table_Constructor::construct_table ()
{
    log << "------------------ table -----------------------" <<endl ;
    bool ambiguous = false;
    bool  sync;
    string non_terminal;
    vector <vector <string> > productions;
    for (map <string, vector<vector<string> > >::iterator it = grammar.begin(); it!=grammar.end(); it++)
    {
        sync = true ;
        non_terminal = it->first;
        productions = it-> second;
        for (int i=0; i<productions.size(); i++)
        {
            if (isTerminal[productions[i][0]] && productions[i][0] != "\\L")
            {

                if (parsing_table[non_terminal][productions[i][0]].size() == 0)
                {
                    parsing_table[non_terminal][productions[i][0]] = productions[i];
                }

            }
            else
            {
                //for (int k=0; k<first_map[productions[i][0]].size(); k++)
                //{
                for (std::set<string>::iterator it=first_map[productions[i][0]].begin(); it!=first_map[productions[i][0]].end(); ++it)
                {
                    if ((*it).compare("\\L") !=0 )
                    {
                        if (parsing_table[non_terminal][(*it)].size() == 0)
                        {
                            parsing_table[non_terminal][(*it)] = productions[i];
                        }
                         else if (!sync) // gehad
                             ambiguous = true   ;

                    }
                    else
                    {
                        sync = false ;
                    }
                }
                if (!sync)
                {
                    for (std::set<string>::iterator it=follow_map[non_terminal].begin(); it!=follow_map[non_terminal].end(); ++it)
                    {
                        if (parsing_table[non_terminal][*it].size() == 0)
                        {
                            parsing_table[non_terminal][*it] = productions[i];
                        }
                    }
                }
            }
        }
        if (sync)
        {
            for (std::set<string>::iterator it=follow_map[non_terminal].begin(); it!=follow_map[non_terminal].end(); ++it)
            {
                if (parsing_table[non_terminal][*it].size() == 0)
                {
                    parsing_table[non_terminal][*it].push_back ("sync");
                }

            }
        }
    }
    if (ambiguous)
    {
        log  << " *********** Error: ambiguous Grammar Not LL(1)" << endl;
    }
    for (map<string,map <string,vector <string> > > :: iterator  it = parsing_table.begin() ; it != parsing_table.end() ; it ++)
    {
        log << "nonTerminal :: "<<it -> first <<endl  ;
        map <string,vector <string> > temp_production = it->second ;
        for (map <string,vector <string> > :: iterator  it2 = temp_production.begin() ; it2 != temp_production.end() ; it2 ++)
        {

            log <<"input "<< it2 -> first << ": " ;
            vector <string> pro = it2->second ;
            for (int i =0 ; i<pro.size(); i++)
            {
                log << pro[i] << " ";
            }
            log <<endl ;
        }
    }
    return ambiguous;
}
map<string, map <string, vector<string> > > Table_Constructor::get_parsing_table()
{
    return parsing_table;
}

map <string, bool> Table_Constructor ::get_is_terminal()
{
    return isTerminal;
}

string Table_Constructor::get_start_symbol()
{
    return start_symbol;
}

map <string, set<string> > Table_Constructor::get_first_map()
{
    return first_map;
}
