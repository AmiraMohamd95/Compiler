#include "Parser.h"
#include <bits/stdc++.h>
using namespace std;

Parser::Parser()
{

    output_file.open ("output.txt");
    table.init();
    productions.push("$");
    productions.push(table.get_start_symbol());
    output_file<<productions.top()<<endl;
    parsing_table = table.get_parsing_table();
    is_terminal = table.get_is_terminal();
}

void Parser::parse(string token)
{

    cout << "current token is : " << token <<endl;
    string top = productions.top();

    // input matched top of stack
    if(token.compare(top) == 0)
    {
        productions.pop();
        if(token.compare("$") == 0)
            output_file<<"Accept"<<endl;
        return;
    }
    else if(top.compare("$") == 0 )
    {
        output_file<<"$_ERROR:extra "<< token<<endl;
        return;
    }
    /*else if(token.compare("$") == 0 && productions.size() > 1 )
    {
        output_file<<"ERROR:missing tokens and end of input"<<endl;
        return;
    }*/
    else if(is_terminal.find(top)->second)
    {
        productions.pop();
        output_file<<"TERMINAL_ERROR:missing "<<top<<endl;
        parse(token);
    }
    // top of stack is non terminal
    else
    {
        map<string,vector<string> > production_entry = parsing_table.find(top)->second;

        // entry is empty then discard the token
        if(production_entry.find(token) == production_entry.end())
        {
            if(token.compare("$") == 0)
            {
                output_file<<"ERROR:End of tokens and productions stack is not empty"<<endl;
            }else{
                output_file<<"ERROR:EXTRA " << token <<endl;
            }
            return;
        }
        else
        {
            vector<string> entry = production_entry.find(token)->second;
            // entry is sync so discard the non terminal.
            if(entry.size() == 1 && entry[0].compare("sync") == 0)
            {
               map <string, set<string> > first = table.get_first_map();
                set<string> first_vect = first.find(top)->second;
                output_file<<"SYNC_ERROR:missing ";
               // for(int i = 0 ; i < first_vect.size() ; i++)
                //{
                for (std::set<string>::iterator it=first_vect.begin(); it!=first_vect.end(); ++it)
                {
                    if((*it).compare("sync") != 0)
                        output_file << (*it) << " ";
                }
                output_file<<endl;
                productions.pop();
                parse(token);
            }
            // replace the non terminal with its production
            else
            {
                productions.pop();
                for(int i = entry.size()-1 ; i >= 0 ; i--)
                {
                    if(entry[i].compare("\\L") != 0)
                    {
                        productions.push(entry[i]);
                        cout << "IN THE STACK " << entry[i] <<endl;
                    }
                }
                stack<string> temp ;
                while(productions.top().compare("$") != 0)
                {
                    string top_stack = productions.top();
                    productions.pop();
                    temp.push(top_stack);
                }
                output_file << endl;
                while(!temp.empty())
                {
                    string top_stack = temp.top();
                    output_file<<top_stack<<" ";
                    productions.push(top_stack);
                    temp.pop();
                }
                output_file<<endl;
                parse(token);
            }

        }
    }
}
