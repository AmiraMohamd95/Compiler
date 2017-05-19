#ifndef AUTOMATA_MAKER_H
#define AUTOMATA_MAKER_H

#include "Automata.h"
#include "node.h"
#include <map>
class Automata_maker
{
public:
    Automata_maker();
    virtual ~Automata_maker();
    Automata make_automata(string name, string pattern);
    Automata _or(Automata a1, Automata a2);
    Automata _and(Automata a1, Automata a2);
    Automata _kleen_closure(Automata a1);
    Automata _positive_closure(Automata a1);
    void set_counter (int _count);
    int get_counter ();
    void set_input (set <string> s );
    set <string> get_input ();

protected:

private:

    int id_counter = 0;
    set <string> input ;
};

#endif // AUTOMATA_MAKER_H
