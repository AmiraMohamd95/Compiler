/* Mini Calculator */
/* Semantic.y */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
%}

%{
	int new_label = 1;
	int pc = 0;
	int begin_while=0;
//	map<string, int> int_id;
	map<string, pair<string,int>> symbol_table ;
	string assign_temp;

	vector<pair<int, string>> code; //address + instruction
	void calculate_address();
	void add_code(string s, int len);
	string tostr(int a);
	string tostr(float a);
	string get_bool_expression(string s);
	void modify_goto();
	void generateHeader(char *outfileName) ;
	void generate_byte_code();
	void generateFooter();
	void modify_while();
	char* add_constant(float f);
%}

%union{
	int	int_val;
	float	float_val;
	string*	op_val;
	char*	str_val;
}

%start	METHOD_BODY

%token	<str_val>	id
%token	<float_val>	num
%token	<str_val>	relop
%token	<str_val>	T_float
%token	<str_val>	T_int
%token SYSOUT


%type 	<str_val>	STATEMENT_LIST
%type 	<str_val>	STATEMENT
%type 	<str_val>	IF
%type 	<str_val>	WHILE
%type 	<str_val>	DECLARATION
%type 	<str_val>	PRIMITIVE_TYPE
%type 	<str_val>	ASSIGNMENT
%type 	<str_val>	EXPRESSION
%type 	<str_val>	SIMPLE_EXPRESSION
%type 	<str_val>	TERM
%type 	<str_val>	FACTOR





%left	Plus
%left	Mul
%left	Minus
%left	Div
%left	OPEN
%left	CLOSE
%left	T_boolean
%left	T_if
%left	T_else
%left	T_while
%left	assign
%left	Semi
%left	Comma
%left	Openbracket
%left	Closebracket

%%

METHOD_BODY: |
		 STATEMENT_LIST
		;
STATEMENT_LIST: STATEMENT
		| STATEMENT_LIST STATEMENT
		;
STATEMENT: 	DECLARATION
		|IF
		|WHILE
		| ASSIGNMENT
		|PRINT
		;
DECLARATION: 	PRIMITIVE_TYPE id{symbol_table[$2]  = make_pair ($1 , new_label++);}Semi
		;
PRIMITIVE_TYPE: T_int	{$$="i";}
		| T_float{$$="f";}
		;
		/*
		0: fload_1
	  1: fload_2
	  2: fcmpl
	  3: ifle          11 after go to
	  6: iconst_1
	  7: istore_3
	  8: goto          13 after f
	 11: iconst_2
	 12: istore_3
	 13: iload_3
	 14: ireturn
		*/
IF :	 	T_if OPEN EXPRESSION CLOSE Openbracket {add_code(get_bool_expression($3), 3);} // get if to execute expression
		STATEMENT
		Closebracket
		T_else {add_code("goto", 3); calculate_address();} Openbracket STATEMENT Closebracket {modify_goto();}

		/*
0: iconst_0
1: istore_1
2: iload_1
3: iconst_2
4: if_icmpge     13
7: iinc          1, 1
10: goto          2
13: return
		*/
WHILE : 	T_while{begin_while=pc;} OPEN EXPRESSION CLOSE Openbracket {add_code(get_bool_expression($4), 3);}
		STATEMENT
		Closebracket{add_code("goto", 3); calculate_address();modify_while();}
		;
ASSIGNMENT: 	id assign EXPRESSION Semi{add_code(symbol_table[$1].first+"store_"+tostr(symbol_table[$1].second), 1); }
		;
EXPRESSION: 	SIMPLE_EXPRESSION
		| SIMPLE_EXPRESSION relop SIMPLE_EXPRESSION{$$=$2;}
		;
SIMPLE_EXPRESSION: TERM
		| Minus TERM
		| SIMPLE_EXPRESSION Plus TERM{add_code(string($3)+"add", 1);$$=$3;}
		| SIMPLE_EXPRESSION Minus TERM{add_code(string($3)+"sub", 1);$$=$3;}
		;
TERM: 		FACTOR
		| TERM Mul FACTOR{add_code(string($3)+"mul", 1);$$=$3;}
		| TERM Div FACTOR{add_code(string($3)+"div", 1);$$=$3;}
		;



FACTOR: 	id{add_code(symbol_table[$1].first + "load_"+tostr(symbol_table[$1].second), 1);$$=new char[100];strcpy($$,symbol_table[$1].first.c_str());}
		| num{$$=add_constant($1);} | OPEN EXPRESSION CLOSE;


PRINT:
			SYSOUT OPEN{add_code("getstatic      java/lang/System/out Ljava/io/PrintStream;" , 3);} FACTOR CLOSE Semi
			{

				add_code("invokevirtual java/io/PrintStream/println(I)V" , 3);
			}
			;
/*
PRINT:
			SYSOUT OPEN FACTOR CLOSE Semi
			;

*/
%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c

  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}
int yyerror(char *s)
{
  return yyerror(string(s));
}

void add_code(string s, int len){
	code.push_back(make_pair(pc, s));
	cout << pc <<": "<< s << "\n";
	pc += len;
}

string tostr(int a){
	stringstream ss;
	ss << a;
	return ss.str();
}
string tostr(float a){
	stringstream ss;
	ss << a;
	return ss.str();
}
void calculate_address(){
	int else_addr = pc; // here stand at else
	int i = code.size() - 2; // begin from my previous
	while(i >= 0 && code[i].second.substr(0, 2).compare( "if")!=0){ // search in previous

		i--;
	}
	pair <int ,string> temp = code [i];

//  cout << "888 :: "<<code[i].second.substr(0, 2)<<endl ;
	//cout << "888 :: "<<tostr(else_addr)<<endl ;
	/*code[i].second += "\t";

	code[i].second += tostr(else_addr);*/
	code [i]= make_pair(temp.first ,temp.second + "\t"+  tostr(else_addr) ) ;
	cout << "888 :: "<<code[i].second<<endl ;



}

string get_bool_expression(string s){
	if(s.compare("<")==0)
		return "if_icmpge";
	if(s.compare(">")==0)
		return "if_icmple";
	if(s.compare(">=")==0)
		return "if_icmplt";
	if(s.compare("<=")==0)
		return "if_icmpgt";
	if(s.compare("==")==0)
		return "if_acmpne";
	if(s.compare("!=")==0)
		return "if_acmpeq";

	return "BAD_STRING";
}

void modify_goto(){
	int else_addr = pc;
	int i = code.size() - 2;
        //cout << "*************goto ****" << code[i].second;
	while(i >= 0 && code[i].second .compare( "goto")!=0){
		i--;
	}

	code[i].second += "\t";
	code[i].second += tostr(else_addr);
  cout <<"999 :"<< else_addr<<"\t"<<code[i].second <<endl;
}
void generate_byte_code(){
	freopen("byteCode.j","a",stdout);

	for(auto a: code){
	  cout <<a.first << ":\t" ;
		cout<<a.second<<endl;
	}
	cout<<pc<<": return"<<endl;
	//fclose(stdout);
}
void modify_while(){
	int while_addr = begin_while;
	int i = code.size() - 1;
	code[i].second += "\t";
	code[i].second += tostr(while_addr);
}

char* add_constant(float f){
	if(floor(f)!=f){
		add_code("ldc\t"+tostr(f), 2);
		return "f";
	}
	else {
		if(f <= 5 && f>=0){
			add_code("iconst_"+tostr(f), 1);
		}
		else {
			add_code("bipush\t"+ tostr(f), 2);
		}
		return "i";
	}
}

// prototype of bison-generated parser function
//int yyparse();

 main(int argc, char **argv)
{
  if ((argc > 1) && (freopen(argv[1], "r", stdin) == NULL))
  {
    cerr << argv[0] << ": File " << argv[1] << " cannot be opened.\n";
    //return( 1 );
  }

  yyparse();
	generateHeader(argv[1]);
  generate_byte_code();
	generateFooter();

}

void generateHeader(char * outfileName)
{
		freopen("byteCode.j","a",stdout);
		cout << ".source " + string (outfileName) <<endl;

	cout << ".class public test\n.super java/lang/Object\n"; //code for defining class
	cout <<".method public <init>()V" <<endl ;
	cout <<"aload_0"<<endl ;
	cout << "invokenonvirtual java/lang/Object/<init>()V"<<endl ;
	cout <<"return"<<endl ;
	cout <<".end method\n";
	cout <<".method public static main([Ljava/lang/String;)V"<<endl ;
	cout <<".limit locals 100\n.limit stack 100"<<endl ;

	/* generate temporal vars for syso*/
	/*defineVar("1syso_int_var",T_int);
	defineVar("1syso_float_var",T_float);*/

	/*generate line*/

}

void generateFooter()
{
	freopen("byteCode.j","a",stdout);
  cout <<"return\n";
	cout <<".end method\n";
}
