// -*- C++ -*-

 /*****************************************************************************
  * bison options 
  ****************************************************************************/

/* generate a c++ parser */
%skeleton "lalr1.cc"

/* generate your code in your own namespace */
%define namespace "pnapi::parser::woflan::yy"

/* do not call the generated class "parser" */
%define parser_class_name "BisonParser"

/* generate needed location classes */
%locations

/* pass overlying parser to generated parser and yylex-wrapper */
%parse-param { Parser& parser_ }
%lex-param   { Parser& parser_ }

/* write tokens to parser-woflan.h for use by scanner */
%defines

/* more verbose error reporting */
%error-verbose


 /*****************************************************************************
  * C declarations
  ****************************************************************************/

%code requires {
  /* forward declarations */
  namespace pnapi { namespace parser { namespace woflan {
    class Parser;
  } } } /* pnapi::parser::woflan */
}

%{

#include "config.h"

#include "parser-woflan-wrapper.h"
#include "petrinet.h"

#include <sstream>

%}


 /*****************************************************************************
  * types, tokens, start symbol
  ****************************************************************************/

%token LCONTROL RCONTROL KEY_TRUE KEY_FALSE TILDE
%token KEY_TRANSITION KEY_PLACE KEY_IN KEY_OUT KEY_INIT
%token COLON SEMICOLON COMMA NUMBER NEGATIVE_NUMBER IDENT


%union 
{
  int yt_int;
  char * yt_str;
}

%type <yt_int> NUMBER NEGATIVE_NUMBER
%type <yt_str> IDENT

%start net


 /*****************************************************************************
  * grammar rules 
  ****************************************************************************/

%%

net:
  node 
| net node  
;

node:
  place SEMICOLON
| KEY_TRANSITION transition_name label ins outs SEMICOLON
;

place:
  KEY_PLACE IDENT 
  {
    try
    {
      parser_.places_[$2] = parser_.place_ = &(parser_.net_.createPlace($2, 0, 0));
    }
    catch(exception::Error & e)
    {
      parser_.rethrow(e);
    }

    free($2);
  }
  place_tail
;

place_tail:
  /* empty */
| KEY_INIT NUMBER
  {
    parser_.place_->setTokenCount($2);
  }
;

transition_name:
  /* empty */
  { parser_.needLabel = true; }
| IDENT 
  {
    try
    {
      parser_.transition_ = &(parser_.net_.createTransition($1));
    }
    catch(exception::Error & e)
    {
      parser_.rethrow(e);
    }

    parser_.transName = $1;
    free($1);
    parser_.needLabel = false;
  }
;

label:
  /*empty*/ 
  { parser_.check(!(parser_.needLabel), "transition must be labeled"); }
| TILDE IDENT
  {
    try
    {
      parser_.transition_->setName($2);
    }
    catch(exception::Error & e)
    {
      parser_.rethrow(e);
    }

    free($2);
  }
;

ins:
  /*empty*/
| KEY_IN
  { 
    parser_.target_ = reinterpret_cast<Node * *>(&(parser_.transition_));
    parser_.source_ = reinterpret_cast<Node * *>(&(parser_.place_));   
  }
  arcs
;

outs:
  /*empty*/
| KEY_OUT
  { 
    parser_.source_ = reinterpret_cast<Node * *>(&(parser_.transition_));
    parser_.target_ = reinterpret_cast<Node * *>(&(parser_.place_)); 
  }
  arcs
;

arcs:
  arc
| arcs arc
;

arc:
  IDENT
  {
    parser_.place_ = parser_.places_[$1];
    parser_.check(parser_.place_ != NULL, "unknown place");

    Arc * a = parser_.net_.findArc(**(parser_.source_), **(parser_.target_));
    if(a != NULL)
    {
      a->setWeight(a->getWeight() + 1);
    }
    else
    {
      parser_.net_.createArc(**(parser_.source_), **(parser_.target_), 1);
    }
    free($1);
  }
;

