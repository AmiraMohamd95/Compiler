#include "Automata_maker.h"
#include "node.h"


#include <vector>
#include <set>
#include <algorithm>
using namespace std;
Automata_maker::Automata_maker()
{
    id_counter = 0;
}
void Automata_maker ::set_input (set <string> s){

 this ->input = s;
}
set <string> Automata_maker ::get_input (){

 return this-> input ;

}


Automata Automata_maker::make_automata(string name, string pattern )
{

    cout << "in make" <<endl ;
    input.insert(pattern);
    Automata automata ;
    /*start*/
    node start;
    start.set_id(id_counter++);
    //cout << "**************    id  ::::::::" << id_counter<<endl ;
    automata.set_name(name);

    /*accpetance*/
    node end_node;
    end_node.set_id(id_counter++);
   // cout << "**************    id  ::::::::" << id_counter<<endl ;

    end_node.set_accepted(true);
    end_node.set_accepted_pattern(pattern);
    pair<node,string> my_pairs;
    my_pairs  = make_pair(end_node,pattern);
    start.put_in_list(my_pairs);
    automata.set_start(start);
    automata.set_current (end_node);
    map  <int ,node> temp ;
    temp [start.get_id()] =start;
    temp[end_node.get_id()]=end_node;
    automata.set_nodes(temp);

  //  cout << "out of make" <<endl ;
    return automata;
}

Automata Automata_maker::_or(Automata a1, Automata a2)
{
    cout << "in or ****" <<endl ;
    Automata automata;
    /*set common start*/
    node common_start;
    common_start.set_id(id_counter++);
    pair<node,string> first_pair;
    first_pair  = make_pair(a1.get_start(),"L");
    common_start.put_in_list(first_pair);

    pair<node,string> second_pair;
  //  cout << a2.get_start().get_id() << "  :hhhhh"<<endl ;
    second_pair  = make_pair(a2.get_start(),"L");
    common_start.put_in_list(second_pair);

    automata.set_name(a1.get_name() + a2.get_name());


    /*common end*/
    node common_end;
    common_end.set_id(id_counter++);
    pair<node,string> _first_pair;
    _first_pair  = make_pair(common_end,"L");
    node end1 = a1.get_current();
    end1.put_in_list(_first_pair);
    end1.set_accepted(false);

    map<int , node> temp1 =a1.get_nodes();
   // temp1.erase(temp1.end());
    temp1[end1.get_id()]=end1;
    a1.set_nodes(temp1);//
    a1.set_current(end1);


    pair<node,string> _second_pair;
    _second_pair  = make_pair(common_end,"L");
    node end2 = a2.get_current();
//cout << end2.get_id() << "  :hhhhh"<<endl ;

    end2.put_in_list(_second_pair);
    end2.set_accepted(false);
    common_end.set_accepted(true);


    map <int ,node> temp2 =a2.get_nodes();
   // temp2.erase(temp2.end());
    temp2[end2.get_id()]=end2;
    a2.set_nodes(temp2);
    a2.set_current(end2);
      //cout << a2.get_start().get_id()  << "  :llllllllllllll"<<endl ;

    automata.set_start(common_start);
    automata.set_current(common_end);

    map <int ,node > new_nodes ;
    new_nodes[common_start.get_id()]= common_start;

     for (std::map<int,node>::iterator it=temp1.begin(); it!=temp1.end(); ++it){

       node n  = it->second;
      //  cout << "-------------*****--------------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }
    for (std::map<int,node>::iterator it=temp2.begin(); it!=temp2.end(); ++it){

       node n  = it->second;
        //cout << "-------------*****--------------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }


    new_nodes[common_end.get_id()]=common_end;


    automata.set_nodes(new_nodes);
    return automata;


}

Automata Automata_maker::_and(Automata a1, Automata a2)
{
    cout << "in And ************* " <<endl ;
    Automata automata;
    int start_id = a2.get_start().get_id();

    automata.set_name(a1.get_name() + a2.get_name());

    node end1 = a1.get_current();
    end1.set_accepted(false);

    vector <pair <node,string> > a2_nodes = a2.get_start().get_list();
    vector <pair <node,string> >end1_next =end1.get_list() ;

    pair <node ,string> eps_pair=make_pair(a2.get_start(),"L");
     end1_next .push_back(eps_pair);
    end1.set_list(end1_next);

    map <int ,node> temp1 =a1.get_nodes();
   // temp1.erase(temp1.end());
    temp1[end1.get_id()] = end1;
    a1.set_nodes(temp1);


    automata.set_start(a1.get_start());
    automata.set_current(a2.get_current());

    map<int,node > new_nodes ;
    map <int ,node> temp2 =a2.get_nodes();

    temp1[automata.get_start().get_id()]= automata.get_start() ;
    for (std::map<int,node>::iterator it =temp1.begin(); it!=temp1.end(); ++it){

       node n  = it->second;
        //cout << "-------------*****--------------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }
    for (std::map<int,node>::iterator it=temp2.begin(); it!=temp2.end(); ++it){
       //  if (it ->first == start_id)
         //   continue ;
       node n  = it->second;
       // cout << "-------------*****---222-----------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }


   // new_nodes.push_back(a2.get_current());

    automata.set_nodes(new_nodes);
   // cout << "iiiiiiiiiiiiiiiiiiiiiiiiii" <<endl ;
    return automata;

}
Automata Automata_maker::_kleen_closure(Automata a1)
{
 cout << "in kleen ****" <<endl ;
    Automata automata;
    //name
    automata.set_name(a1.get_name()+ "*");

    node old_start = a1.get_start();
    node old_end = a1.get_current();
    // to connect orginal start and old
    pair<node,string> back_edge;
    back_edge  = make_pair(old_start,"L");

    //to add new start  connected to old start and new end
    node new_start ;
    new_start.set_id(id_counter++);
  // new end connected  to old end
    node new_end ;
    new_end.set_id(id_counter++);
    new_end.set_accepted(true);
     old_end.set_accepted(false);

      //1- new start to old start
    pair<node,string>  new_start_old_start = make_pair(old_start,"L");
    vector <pair <node,string> > new_start_next ;
    new_start_next.push_back(new_start_old_start);

  // old end to new end

    pair<node,string> old_end_new_end = make_pair(new_end,"L");
    ///
    vector <pair <node,string> > end_next = old_end.get_list();
    end_next.push_back(old_end_new_end);
    end_next.push_back(back_edge);
    old_end.set_list(end_next);

    //
    map<int ,node> temp1 =a1.get_nodes();
   // temp1.erase(temp1.end());
    temp1[old_end.get_id()] = old_end;
    a1.set_nodes(temp1);

    //new start to new end
    pair <node ,string> new_start_new_end =  make_pair(new_end , "L");
    new_start_next.push_back(new_start_new_end );
    new_start.set_list(new_start_next);

  /// new automata vector


    automata.set_start(new_start);
    automata.set_current(new_end);

    map<int ,node > new_nodes ;
    new_nodes[new_start.get_id()] = new_start;
     for (std::map<int,node>::iterator it =temp1.begin(); it!=temp1.end(); ++it){

       node n  = it->second;
       // cout << "-------------*****--------------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }
       new_nodes[new_end.get_id()]=new_end ;///


    automata.set_nodes(new_nodes);
    return automata;

}
Automata Automata_maker::_positive_closure(Automata a1)
{
  cout << "in positve ****" <<endl ;
    Automata automata;
    automata.set_name(a1.get_name() + "+");

    node old_start = a1.get_start();
    node old_end = a1.get_current();
    pair<node,string> back_edge;
    back_edge  = make_pair(old_start,"L");


    vector <pair <node,string> > end_next = old_end.get_list();
    end_next.push_back(back_edge);
    old_end.set_list(end_next);

    automata.set_start(old_start);
    automata.set_current(old_end);

    map<int , node > temp1 =a1.get_nodes();
    //temp1.erase(temp1.end());
    temp1[old_end.get_id()] = old_end;
    a1.set_nodes(temp1);


    ///new vector automata
    map<int ,node > new_nodes ;

    temp1[automata.get_start().get_id()]= automata.get_start() ;

   for (std::map<int,node>::iterator it =temp1.begin(); it!=temp1.end(); ++it){

       node n  = it->second;
       // cout << "-------------*****--------------"<<  n.get_id()<<endl ;
        new_nodes[it-> first ]= n;
    }

    automata.set_nodes(new_nodes);
    return automata;


}

void Automata_maker:: set_counter (int _count){

   this -> id_counter = _count ;
}
int Automata_maker:: get_counter (){

  return this ->id_counter ;
}


Automata_maker::~Automata_maker()
{
    //dtor
}
