#ifndef INPUT_ANALYZER_H
#define INPUT_ANALYZER_H
#include "node.h"
#include "Automata.h"
#include "Parser.h"


class input_analyzer
{
    public:
        input_analyzer();
        virtual ~input_analyzer();
        void read_input(Automata mini_automata ,  string file_name);

    protected:

    private:
        int line_count = 0;
        ofstream final_output;
        ofstream symbol_table_output;
        ofstream tokens;
        pair<node,bool> move_state( node current_node, char current_char , map<int , node> nodes_map);
        bool is_digit_range(string weight);
        bool is_alpha_range(string weight);
        bool is_not_excluded(string weight, char input);
        vector<string> split( const char *str, char c );
        string removespace(string str);
        Parser parser;





};

#endif // INPUT_ANALYZER_H
