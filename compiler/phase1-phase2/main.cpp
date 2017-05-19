#include <iostream>
#include "Engine.h"
#include"DfaCreator.h"
#include "Table_Constructor.h"
#include "Input_file.h"
using namespace std;
ofstream myfile;
//bool vis[10];
Automata nfa;
int main()
{

    Engine engine;
    engine.read();
   Input_file ipt ;
  //  map<string , vector<vector<string>> > m = ipt.read_grammer();
  Table_Constructor t ;
  t.init ();
    /*typedef map<string , vector<vector<string>> >::iterator it_type;
    for(it_type iterator = m.begin(); iterator != m.end(); iterator++) {
        cout <<  iterator->first <<":" ;
        vector<vector<string>> v = iterator->second ;
        for(int i = 0 ; i < v.size() ; i++)
        {
            vector <string> v2 = v[i];
            for(int j = 0 ; j < v2.size() ; j++)
            {
                cout << v2[j] <<" ";
            }
            if(i != v.size() -1)
            cout << "|";
        }
        cout <<endl;
    }
*/


    return 0;
}
