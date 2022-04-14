%{
	#include <pandora/ast.hpp>
  #include <cstdio>
  #include <cstdlib>
	pandora::ast::block *global_top_ast_block; /* the top level root node of our final AST */

	extern int yylex();
	void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }
%}

/* Represents the many different ways we can access our data */
%union {
	pandora::ast::node *node;
	pandora::ast::block *block;
	pandora::ast::expression *expr;
	pandora::ast::statement *stmt;
	pandora::ast::identifier *ident;
	pandora::ast::variable_declaration *var_decl;
	std::vector<pandora::ast::variable_declaration*> *varvec;
	std::vector<pandora::ast::expression*> *exprvec;
	std::string *string;
	int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> TPLUS TMINUS TMUL TDIV
%token <token> TRETURN TEXTERN

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl extern_decl
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : stmts { global_top_ast_block = $1; }
		;
		
stmts : stmt { $$ = new pandora::ast::block(); $$->add_statement($<stmt>1); }
	  | stmts stmt { $1->add_statement($<stmt>2); }
	  ;

stmt : var_decl | func_decl | extern_decl
	 | expr { $$ = new pandora::ast::expression_statement($1); }
	 | TRETURN expr { $$ = new pandora::ast::return_statement($2); }
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
	  | TLBRACE TRBRACE { $$ = new pandora::ast::block(); }
	  ;

var_decl : ident ident { $$ = new pandora::ast::variable_declaration($1, $2); }
		 | ident ident TEQUAL expr { $$ = new pandora::ast::variable_declaration($1, $2, $4); }
		 ;

extern_decl : TEXTERN ident ident TLPAREN func_decl_args TRPAREN
                { $$ = new pandora::ast::extern_declaration($2, $3, *$5); delete $5; }
            ;

func_decl : ident ident TLPAREN func_decl_args TRPAREN block 
			{ $$ = new pandora::ast::function_declaration($1, $2, $6, *$4); delete $4; }
		  ;
	
func_decl_args : /*blank*/  { $$ = new std::vector<pandora::ast::variable_declaration*>(); }
		  | var_decl { $$ = new std::vector<pandora::ast::variable_declaration*>(); $$->push_back($<var_decl>1); }
		  | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
		  ;

ident : TIDENTIFIER { $$ = new pandora::ast::identifier(*$1); delete $1; }
	  ;

numeric : TINTEGER { $$ = new pandora::ast::integer(atol($1->c_str())); delete $1; }
		| TDOUBLE { $$ = new pandora::ast::number(atof($1->c_str())); delete $1; }
		;
	
expr : ident TEQUAL expr { $$ = new pandora::ast::assignment($<ident>1, $3); }
	 | ident TLPAREN call_args TRPAREN { $$ = new pandora::ast::call($1, *$3); delete $3; }
	 | ident { $<ident>$ = $1; }
	 | numeric
         | expr TMUL expr { $$ = new pandora::ast::binary_operator($2, $1, $3); }
         | expr TDIV expr { $$ = new pandora::ast::binary_operator($2, $1, $3); }
         | expr TPLUS expr { $$ = new pandora::ast::binary_operator($2, $1, $3); }
         | expr TMINUS expr { $$ = new pandora::ast::binary_operator($2, $1, $3); }
 	 | expr comparison expr { $$ = new pandora::ast::binary_operator($2, $1, $3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
	 ;
	
call_args : /*blank*/  { $$ = new std::vector<pandora::ast::expression*>(); }
		  | expr { $$ = new std::vector<pandora::ast::expression*>(); $$->push_back($1); }
		  | call_args TCOMMA expr  { $1->push_back($3); }
		  ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE;

%%

