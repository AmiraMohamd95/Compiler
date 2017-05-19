#include "Engine.h"
#include <iostream>
#include <fstream>
#include "DfaCreator.h"
#include "Minimization.h"
#include "input_analyzer.h"
using namespace std;


Engine::Engine()
{

}
void Engine::read ()
{
    ifstream file("RE.txt");
    string line;
    //int counter = 0 ;
    while(getline(file, line)) /// file name
    {
        cout << line<<endl;
        //cout << "staaaaaaaack size : " << operation.size() << " in line :" << counter++;
        if(line[0] == '{' || line[0] == '[')
        {
            handle_key_words(line);
            //s+
        }
        else if( !line.empty() && !check_white_spaces(line))
        {
            handle_expression(line);

        }

    }
    cout << "\n*** overall ***\n" ;
  Automata final_automata = form_all(automaton_map);
   // Automata final_automata = automaton_map.find("id")->second ;
    //print(final_automata);
    set <string> input = maker.get_input() ;
    set_vector_input(input) ;
    cout << "\n*** new fileeeeeeeeeeeeeeeee ***\n" ;
   // trial r ;
    //r.new_print(final_automata);

    map<string,int>accepted_priority;
    ofstream log5;
    log5.open("test_accepted_patterns.txt");
    for(int i = 0 ; i < accepted_patterns.size() ; i++)
    {
        accepted_priority[accepted_patterns[i]] = i;
        log5  << i <<" "<< accepted_patterns[i] << endl;
    }
    log5.close();


    /*
    accepted_priority["while"] = 0;
    accepted_priority["if"] = 1;
    accepted_priority["else"] = 2;
    accepted_priority["boolean"] = 3;
    accepted_priority["float"] = 4;
    accepted_priority["int"] = 5;
    accepted_priority[";"] = 6;
    accepted_priority[","] = 7;
    accepted_priority["("] = 8;
    accepted_priority[")"] = 9;
    accepted_priority["{"] = 10;
    accepted_priority["}"] = 11;
    accepted_priority["relop"] = 12;
    accepted_priority["assign"] = 13;
    accepted_priority["addop"] = 14;
    accepted_priority["mulop"] = 15;
    accepted_priority["id"] = 16;
    accepted_priority["num"] = 17;
*/

    DfaCreator dfa_creator ;
    dfa_creator.init(final_automata , accepted_priority , vector_input);
    dfa_creator.execute();
  //  dfa_creator.print_automata(dfa_creator.dfa.nodes.find(dfa_creator.dfa.get_start().id)->second,dfa_creator.dfa);
     Minimization optimizer;
     cout <<"============AMIRA===================="<<endl ;
    optimizer.init(dfa_creator.dfa,dfa_creator.dfa_inputs,dfa_creator.tokens_priority);
     optimizer.minimize_states();
     //====================
    cout << "size **************************** " <<optimizer.get_minimized_dfa().nodes.size() <<endl ;
     input_analyzer in ;
     in.read_input(optimizer.get_minimized_dfa() ,"input.txt");



}
 void Engine ::set_vector_input (set <string> myset){

    for (std::set<string>::iterator it=myset.begin(); it!=myset.end(); ++it)
    {
        if (*it != "a-z" && *it != "L"){
            vector_input .push_back(*it);
            std::cout << ' ' << *it ;

            }
    }
    vector_input.push_back("a-z");

 }


bool Engine::check_white_spaces(string line)
{
    for(int i = 0 ; i < line.length() ; i++)
    {
        if(line[i] != ' ' && line[i] !='\t') return false;
    }
    return true;
}

void Engine::handle_key_words (string line)
{
    char ch;
    string token = "";
    Automata prev_automata;
    Automata current_automata;

    for(int i = 1; i < line.length() ; i++)
    {
        /// [; , \( \) \{ \} ]
        ch = line[i];
        string s = "";
        if ((ch == '{' ||ch == '}'|| ch == '[' ||ch == ']' )&& line [i-1] !='\\' )
            continue ;
        s+=ch;
        if (ch != ' ' && ch != '\\' )
        {

            if(token == "" )
            {
                prev_automata = maker.make_automata(s,s);
            }
            else
            {
                current_automata = maker.make_automata(s,s);

                prev_automata = maker._and(prev_automata,current_automata);
            }
            token +=ch;

        }
        else if (ch == ' ' && token != "")
        {

            map <int , node > aut_m ;

            if (line[0]=='{')
            {

                aut_m = set_acceptance_id(prev_automata);
                prev_automata.set_nodes( aut_m) ;

            }
            node end_node =  prev_automata.get_current();

            end_node.set_accepted_pattern(token);
            aut_m [end_node.get_id()] = end_node;
            node start_in_node= prev_automata.get_start();
            aut_m [start_in_node.get_id()] = start_in_node;

            prev_automata.set_nodes( aut_m) ;

            accepted_patterns.push_back(token);
            print(prev_automata);
            automaton_map[token] = prev_automata;
            token = "" ;




        }
    }
    //  print(prev_automata);
}


map <int ,node> Engine::set_acceptance_id(Automata aut)
{
    map <int ,node> nodes = aut.get_nodes();
    for (std::map<int,node>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        node n = it->second;
        n.set_accepted(true);
        n.set_accepted_pattern("id");
        nodes[n.get_id()] = n ;
    }

    return nodes;

}

void Engine::handle_expression (string line)
{
    bool contain_backslash = false;
    char ch;
    bool type_determined = false;
    bool is_operator = true;
    bool is_Automata = false;
    string token = "";
    string automata_name;
    for(int i = 0 ; i <= line.length() ; i++)
    {
        ch = line[i];
        if(ch == ' ' )
        {
            //cout << "token before space :" <<token <<endl;
            if(token != "")
            {
                //cout << "error" <<endl ;
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                }
                else
                {
                    //cout << "else" <<endl;
                    aut = maker.make_automata(token,token);
                }
                pattern.push(aut);
                check_precedence('X');
                is_operator = false;
                token ="" ;
            }
        }
        else if(ch == 'L')
        {
            contain_backslash = false;
            Automata temp  = maker.make_automata("eps", "L");
            pattern.push(temp);
            if(!is_operator)
            {
                check_precedence('X');
            }
            is_operator = false;
            token = "" ;

        }
        else if(ch == 'E')
        {
            if (token != "" )
            {
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                }
                else
                {
                    //cout << "else" <<endl;
                    aut = maker.make_automata(token,token);
                }

                pattern.push(aut);
                check_precedence('X');

            }
            Automata temp  = maker.make_automata("E", "E");
            pattern.push(temp);
            // if(!is_operator)
            //{
            check_precedence('X');
            //}
            token = "" ;
            is_operator = false;

        }
        else if(ch == '.')
        {
            cout << "in dot****" <<endl ;
            if (token != "" )
            {
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                }
                else
                {
                    //cout << "else" <<endl;
                    aut = maker.make_automata(token,token);
                }

                pattern.push(aut);
            }
            check_precedence('X');
            Automata temp  = maker.make_automata(".", ".");
            pattern.push(temp);
            // if(!is_operator)
            //{
           // check_precedence('X');
            //}
            token = "" ;
            is_operator = false;
        }
        else if(ch == '=' && line[i-1] != '\\')
        {
            is_Automata = false;
            is_operator = true;
            automata_name = token;
            token = "";
            type_determined = true;
        }
        else if(ch == ':')
        {
            is_operator = true;
            is_Automata = true;
            automata_name = token;
            token = "";
        }
        else if(ch == '|')
        {
            Automata automata;
            if(contain_backslash)
            {

                // cout << "in handle back slash\n";
                contain_backslash = false;
                string s = "\\";
                if(token[0]!='\\')
                    token = s + token;
                is_operator = handle_back_slash(token, is_operator);
            }
            else
            {
                if(token != "")
                {
                    Automata aut;
                    if(check_if_automata_exit(token))
                    {
                        aut = definition_map.find(token)->second;
                        aut = clone_aut (aut);
                    }
                    else
                    {
                        aut = maker.make_automata(token,token);
                    }
                    pattern.push(aut);
                }
            }
            //cout << "go to check precedence";
            check_precedence('|');
            is_operator = true;
            token = "";
        }
        else if((ch == '*' || ch == '+') && line[i-1]!='\\')
        {
           // Automata automata;
            /*handle digit+ */
            if(token != "")
            {
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    cout << "handle digit+\n";
                    cout << "id counter before: " << maker.get_counter() << endl;
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                    cout << "id counter before: " << maker.get_counter() << endl;

                }
                else
                {
                    aut = maker.make_automata(token,token);
                }
                pattern.push(aut);
            }
            Automata temp = pattern.top();
            pattern.pop();
            if(ch == '*')
            {
                pattern.push(maker._kleen_closure(temp));
            }
            else
            {
                pattern.push(maker._positive_closure(temp));
            }
            //if(!is_operator) check_precedence('X');
            is_operator = false;
            token = "";
        }
        else if(ch == '(')
        {

            if(line [i-1] != '|' )
            {

                if (token != "" )
                {
                    Automata aut;
                    if(check_if_automata_exit(token))
                    {
                        aut = definition_map.find(token)->second;
                        aut = clone_aut (aut);
                    }
                    else
                    {
                        //cout << "else" <<endl;
                        aut = maker.make_automata(token,token);
                    }
                    pattern.push(aut);
                }
                check_precedence('X');

            }

            //cout << "--------------" <<endl;
            is_operator = true;
            token = "";
            operation.push('(');
        }
        else if(ch == ')')
        {
            //  cout << "endddddddddddddddddddddd \n\n" ;

            if(token != "")
            {
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                }
                else
                {
                    aut = maker.make_automata(token,token);
                }

                pattern.push(aut);
            }

            while(operation.top() != '(')
            {

                Automata temp1 = pattern.top();
                pattern.pop();
                Automata temp2 = pattern.top();
                pattern.pop();
                if(operation.top() == 'X')
                    pattern.push(maker._and(temp2,temp1));
                else
                    pattern.push(maker._or(temp2,temp1));
                operation.pop();

            }
            operation.pop();
            token = "";
        }
        else if(ch == '\0')
        {
            if(contain_backslash)
            {
                contain_backslash = false;
                string s = "\\";
                if(token[0]!='\\')
                    token = s + token;
                is_operator = handle_back_slash(token, is_operator);
            }
            else if(token != "")
            {
                Automata aut;
                if(check_if_automata_exit(token))
                {
                    aut = definition_map.find(token)->second;
                    aut = clone_aut (aut);
                }
                else
                {
                    aut = maker.make_automata(token,token);
                }

                pattern.push(aut);
            }
            while(!operation.empty())
            {
                //cout << "*****************" <<endl;
                Automata temp1 = pattern.top();
                pattern.pop();
                Automata temp2 = pattern.top();
                pattern.pop();

                char c = operation.top ();
                //  cout << "c ====== " << c<<endl;
                if(operation.top() == 'X')
                    pattern.push(maker._and(temp2,temp1));
                else
                {
                    //   cout << "*********in or********" <<endl;
                    pattern.push(maker._or(temp2,temp1));
                    // cout << "*********out of or********" <<endl;
                }
                operation.pop();
            }
            Automata final_automata =  pattern.top();
            pattern.pop();
            final_automata.set_name(automata_name);
            node final_end = final_automata.get_current();
            final_end.set_accepted_pattern(automata_name);
            map <int ,node> m_aut = final_automata.get_nodes();
            m_aut[final_end.get_id()]= final_end;
            final_automata.set_nodes(m_aut);

            if(is_Automata)
            {
                vector <string> accepted ;///
                accepted.push_back(automata_name);///
                //  cout <<"************************\n***********\nautomata_name"<<automata_name << endl ;
                final_automata .set_accepted_patterns(accepted);
                automaton_map[automata_name] = final_automata;
            }
            else
            {
                cout << "DEFINITION MAAAAAAAAAP:" << automata_name <<endl;
                definition_map[automata_name] = final_automata;

            }
            cout << "name :    "<< final_automata.get_name() <<endl;

            print(final_automata);
            // cout <<  final_automata.get_start().get_id() <<endl;
            //print_rec(final_automata.get_start());
            is_operator = true;
            is_Automata = false;
            type_determined = false;

        }
        else  //(ch != '|' || ch != '(' || ch != ')' || ch != '*' || ch != '+'|| ch != ' ' || ch != '\\' || ch != '\n')
        {

            // cout << "appended      " << ch<<endl ;
            if(ch == '\\')
            {
                contain_backslash = true;
            }
            token+=ch;
        }

    }

    cout << ch; // Or whatever
}

void Engine::check_precedence(char current_oper)
{
    while(!operation.empty() &&( (operation.top() == 'X' && current_oper == 'X' ) || (operation.top() == 'X' && current_oper == '|' )
                                 ||(operation.top() == '|' && current_oper == '|' ) ) )
    {
        Automata temp1 = pattern.top();
        pattern.pop();
        Automata temp2 = pattern.top();
        pattern.pop();
        if(operation.top() == 'X')
            pattern.push(maker._and(temp2,temp1));
        else
            pattern.push(maker._or(temp2,temp1));
        operation.pop();
    }
    operation.push(current_oper);

}


vector<string> Engine::split( const char *str, char c )
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    }
    while (0 != *str++);

    return result;
}

bool Engine::check_if_automata_exit(string token)
{
    return ( definition_map.find(token) != definition_map.end() );

}


void Engine ::print_rec (node start)
{

    vector <pair<node,string> > nodes = start.get_list();
    for (int i = 0 ; i < nodes.size() ; i++)
    {
        //   cout << "*******in*********" <<endl ;
        node n = nodes[i].first;
        cout << start.get_id()<<" ";
        cout << nodes[i].second<<" ";
        cout <<  n.get_id() <<endl;
        print_rec(n);
    }

}

void Engine::print (Automata aut)
{

    map<int,node> nodes = aut.get_nodes() ;
    vector <string> sv = aut.get_accepted_patterns() ;

     for (int i = 0 ; i <sv.size() ; i++)
        cout << "Accepted  pattern in automata :: "<<sv[i]<<endl  ;

      for (std::map<int,node>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
      {

        vector <pair<node,string> > next = (it->second).get_list () ;


        for (int j = 0 ; j < next.size() ; j++)
        {

            std::cout << it->first  <<" ";
            cout << next[j].second  <<" ";
            cout << next[j].first.get_id() <<endl;
         //   cout <<"node id from the map "  <<nodes.find(next[j].first.get_id())->second.get_id() << endl;

        }
          if ( it->second.get_accepted()){
              string s = (it->second).get_accepted_pattern() ;
              cout << it->second.get_id()<<" accepted node!!! ::"<<s << endl;
        }
    }
}

Automata Engine::clone_aut (Automata aut)
{
    Automata new_aut;
    map<int, node> new_map;
    map<int, int > map_old_new;
    map<int,node> nodes = aut.get_nodes() ;
    int id_counter = maker.get_counter();
    int current_id = aut.get_current().get_id();
    int start_id = aut.get_start().get_id();
    new_aut.set_name(aut.get_name());
    for (std::map<int,node>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        // itr->first -> id , it.second -> node
        node old_node = it->second;
        node new_node;
        int parent_id;
        if(map_old_new.find(it->first) == map_old_new.end() )
        {
            parent_id = id_counter++;
            new_node.set_id(parent_id);
            map_old_new[it->first] = id_counter-1;
        }
        else
        {
            parent_id = map_old_new.find(it->first)->second;
            new_node = new_map.find(parent_id)->second;
        }
        vector <pair<node,string> > next = (old_node).get_list ();
        vector <pair<node,string> > new_next;
        for (int j = 0 ; j < next.size() ; j++)
        {
            int id =  next[j].first.get_id();
            node new_child_node;
            if(map_old_new.find(id) == map_old_new.end())
            {
                new_child_node.set_id(id_counter++);
                map_old_new[id] = id_counter-1;
                new_map[id_counter-1] = new_child_node;

            }
            else
            {
                int child_id = map_old_new.find(id)->second;
                new_child_node = new_map.find(child_id)->second;
            }

            new_next.push_back(make_pair(new_child_node,next[j].second));
        }
        new_node.set_list(new_next);
        new_map[parent_id] = new_node;

        if(it->first == current_id )
        {
            new_aut.set_current(new_node);
        }
        else if(it->first  == start_id)
        {
            new_aut.set_start(new_node);
        }
    }
    new_aut.set_nodes(new_map);
    maker.set_counter(id_counter);

    for (std::map<int,node>::iterator it=new_map.begin(); it!=new_map.end(); ++it)
    {
        // itr->first -> id , it.second -> node
        node s = it->second;
        // cout<<"node id : "<< s.get_id() <<endl;

    }
    return new_aut;

}
bool Engine::handle_back_slash(string token,bool is_operator)
{

    //  cout << "*********"<<token << "*********\n";
    const char *char_token = token.c_str();
    vector <string> vect = split(char_token, '\\');
    if(token != "" && vect[0] == "") /// then we have backslash  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        //cout<<"we have a backslash here\n";
        for(int i = 1 ; i < vect.size() ; i++)
        {
            //  cout << "vector after splitting :" << vect[i] <<endl;
            if(!is_operator)
            {
                check_precedence('X');
            }
            /*to handle | \x\x */
            is_operator = false;

            Automata aut;
            if(check_if_automata_exit(vect[i]))
            {
                aut = definition_map.find(vect[i])->second;
                aut = clone_aut (aut);
            }
            else
            {
                aut = maker.make_automata(vect[i],vect[i]);
            }
            pattern.push(aut);
        }
    }
    return is_operator;

}

Automata Engine::form_all ( map<string, Automata> automaton_map)
{

    Automata all_automata ;
    node common_start ;
    common_start.set_id(maker.get_counter());
    vector <pair <node,string> > start_next;
    map <int,node> big_map ;

    for (std::map<string,Automata>::iterator it=automaton_map.begin(); it!=automaton_map.end(); ++it)
    {
        /// make common start points to inner start
        map<int, node > temp1 =it->second.get_nodes();
        node temp_start = it->second.get_start();
        node inner_start = it->second.get_start();//temp1.find(temp_start.get_id())->second;
        pair <node,string> eps_to_start = make_pair(inner_start,"L");
        start_next.push_back(eps_to_start);

        ///put its acceptence
        vector <string> sv = it->second.get_accepted_patterns() ;
        for (int i = 0 ; i <sv.size() ; i++)
            accepted_patterns.push_back(sv[i]) ;

        /// put its nodes in the big automata

        for (std::map<int,node>::iterator it2=temp1.begin(); it2!=temp1.end(); ++it2)
        {
            big_map[it2->second.get_id()] = it2->second ;
        }
    }
    ///set map &start &name
    common_start.set_list(start_next);
    big_map[common_start.get_id()] = common_start;
    all_automata .set_start(common_start);
    all_automata.set_name("all_automata");
    all_automata.set_nodes(big_map);
    all_automata.set_accepted_patterns(accepted_patterns);
    return all_automata ;
}

Engine::~Engine()
{

}
