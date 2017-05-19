#ifndef ENGINE_H
#define ENGINE_H
#include "Automata.h"
#include "Automata_maker.h"
#include "node.h"
#include <map>
class Engine
{
    public:
        Engine();
        virtual ~Engine();
        void read ();
        node  start_node;
        Automata_maker maker ;

    protected:

    private:
        map<string , Automata> automaton_map;
        map<string , Automata> definition_map;
        stack<Automata> pattern;
        stack<char> operation;
        Automata automata;
        void check_precedence(char current_oper);
        vector<string> split( const char *str, char c );
        void handle_expression (string line);
        void handle_key_words (string line);
        map <int ,node> set_acceptance_id(Automata aut);
        bool check_white_spaces(string line);
        bool check_if_automata_exit(string token);
        void print (Automata aut);
        void print_rec (node start);
        Automata clone_aut (Automata aut);
        bool handle_back_slash(string token , bool is_operator);
        Automata form_all ( map<string , Automata> automaton_map) ;
        vector <string> accepted_patterns;
        vector <string> vector_input ;
        void set_vector_input (set <string> s);



};

#endif // ENGINE_H
