#include "input_analyzer.h"

using namespace std;
ofstream input_file;


input_analyzer::input_analyzer()
{
    line_count = 0;
    final_output.open ("test_final_output.txt");
    tokens.open("tokens.txt");
}



void input_analyzer::read_input(Automata mini_automata,  string file_name)
{
    ifstream file(file_name.c_str());
    string line;
    string acceptance_state= "";
    string token = "";
    set <string> symbol_table;
    int last_acceptance_pattern = -1;
    int token_start = 0;


    node current = mini_automata.get_start();
    map<int,node> nodes_map = mini_automata.nodes;
    node start = nodes_map.find(current.get_id())->second;
    current = start;


    while(getline(file, line)) /// file name
    {
        cout << start.get_id()<<endl;
        line_count++;
        char current_char;
        token = "";
        last_acceptance_pattern = -1;
        acceptance_state = "";
        token_start = 0;
        int i = 0;
        current = start;
        final_output << "current line :" << line <<endl;

        while( i < line.size() )
        {

            current_char = line[i];

            pair<node, bool> returned_pair = move_state( current, current_char, nodes_map);
            node returned_node = returned_pair.first;

            if(!(returned_pair.second) || (i == line.size()-1 && !returned_node.accepted )|| isspace(current_char) )
            {
                /*roll back to the last acceptance state*/
                if(last_acceptance_pattern >= token_start)
                {
                    token = line.substr(token_start,last_acceptance_pattern - token_start +1 );
                    token = removespace(token);
                    final_output << token << " : " <<acceptance_state <<endl;
                    tokens << acceptance_state << endl;
                    if(acceptance_state.compare("assign") == 0)
                    {
                        parser.parse("=");
                    }else
                    {
                        parser.parse(acceptance_state);
                    }
                    cout << "***accepted pattern is : " << acceptance_state << " whose token is :" << token<<
                         " terminated at index : " << last_acceptance_pattern<< endl;

                    /*to be inserted in the symbol table*/
                    if(acceptance_state.compare("id") == 0)
                    {
                        symbol_table.insert(token);
                    }
                    token = "";
                    acceptance_state = "";
                    i = last_acceptance_pattern + 1;
                    token_start = i;
                    last_acceptance_pattern = -1;
                }/*no roll back exist so there is an error*/
                else
                {


                    if(!isspace(current_char)){
                        final_output << "line : " << line_count<< " invalid match for: " << token+current_char <<endl;
                        tokens << "line : " << line_count<< " invalid match for: " << token+current_char <<endl;
                    }

                    token = "";
                    acceptance_state = "";
                    i = token_start + 1;
                    token_start = i;
                    last_acceptance_pattern = -1;
                }


                current = start;
            }
            else
            {
                if(returned_node.get_accepted())
                {
                    last_acceptance_pattern = i;
                    acceptance_state = returned_node.accepted_pattern;
                }
                token+=current_char;
                current = returned_node;
                i++;
            }

        }
        if(last_acceptance_pattern >= token_start)
        {
            token = line.substr(token_start,last_acceptance_pattern - token_start +1);
            token = removespace(token);
            final_output << token << " : " <<acceptance_state <<endl;
            tokens << acceptance_state << endl;
            if(acceptance_state.compare("assign") == 0)
            {
                parser.parse("=");
            }
            else
            {
                parser.parse(acceptance_state);
            }
            cout << "**accepted pattern is : " << acceptance_state <<" and accepted token is :"
                 <<token <<endl;


            /*to be inserted in the symbol table*/
            if(acceptance_state.compare("id") == 0)
            {
                symbol_table.insert(token);
            }
        }
    }
    symbol_table_output.open("symbol_table.txt");
    std::set<string>::iterator iter;
    for(iter = symbol_table.begin(); iter != symbol_table.end(); ++iter)
    {
        string token = *iter;
      symbol_table_output << token << endl;
    }
    parser.parse("$");
    symbol_table_output.close();
    final_output.close();
    tokens.close();

}
pair<node,bool> input_analyzer::move_state( node current_node, char current_char, map<int, node> nodes_map)
{
    bool matched = false;
    bool alpha_range = false;
    node alpha_node;
    vector <pair<node, string> > next_transistion = current_node.next;
    string current_weight = "" ;
    current_weight+= current_char;
    // cout<<"current weight :" << current_weight <<endl;

    for(int i = 0 ; i < next_transistion.size() ; i++)
    {
        string weight = next_transistion[i].second;
        if(weight.compare(current_weight) == 0)
        {
            matched = true;

        }// check for 0-9 digits
        else if(isdigit(current_char) && is_digit_range(weight) )
        {
            matched = true;
        } // check for $x,x,x,x
        else if(isalpha(current_char) && !is_not_excluded(weight,current_char))
        {
            matched = true;
        }// check for A-Z | a-z
        else if(isalpha(current_char) && is_alpha_range(weight) )
        {
            alpha_range = true;
            alpha_node =  nodes_map.find(next_transistion[i].first.get_id())->second;
        }

        if(matched)
        {
            node returned =  nodes_map.find(next_transistion[i].first.get_id())->second;
            return make_pair(returned,matched);
        }

    }
    if(alpha_range)
    {
       return make_pair(alpha_node,alpha_range);
    }
    return make_pair(current_node,matched);


}

bool input_analyzer::is_digit_range(string weight)
{
    return (weight.compare("0-9") == 0);
}

bool input_analyzer::is_alpha_range(string weight)
{

    return (weight.compare("A-Z") ==0) ||  (weight.compare("a-z") ==0);
}


bool input_analyzer::is_not_excluded(string weight, char input)
{
    string inpt = "" + input;
    if(weight[0] == '$')
    {
        weight = weight.substr(1,weight.size());
        //cout << "cureent weighttttttttt:"<<weight << endl;
        weight = removespace(weight);
        const char *str = weight.c_str();
        vector<string> excluded_chars = split( str, ',' );
        for(int i = 0 ; i < excluded_chars.size() ; i++)
        {
            string excluded = excluded_chars[i];
            if(excluded.compare(inpt) == 0)
            {
                return false;
            }
        }

    }
    return true;
}


vector<string> input_analyzer::split( const char *str, char c )
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

string  input_analyzer::removespace(string str)
{
    string token = "";
    int i=0;
    for(int i = 0 ; i < str.length() ; i++)
    {
        if (isspace(str[i]))
            continue;
        token+=str[i];
    }
    return token;
}



input_analyzer::~input_analyzer()
{

}
