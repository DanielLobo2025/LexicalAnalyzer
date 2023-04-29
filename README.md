# LexicalAnalyzer
I built a lexical analyzer for a small programming language called Simple Perl-Like (SPL) using C++. Here are the definitons of the SPL language, they are given in EBNF notation.

Take a look at the README file to see a readable version of the definitions

Prog ::= StmtList
StmtList ::= Stmt ;{ Stmt; }
Stmt ::= AssignStme | WriteLnStmt | IfStmt
WriteLnStmt ::= WRITELN (ExprList)
IfStmt ::= IF (Expr) ‘{‘ StmtList ‘}’ [ ELSE ‘{‘ StmtList ‘}’ ]
AssignStmt ::= Var = Expr
Var ::= NIDENT | SIDENT
ExprList ::= Expr { , Expr }
Expr ::= RelExpr [(-eq|==) RelExpr ]
RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]
AddExpr :: MultExpr { ( + | - | .) MultExpr }
MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
ExponExpr ::= UnaryExpr { ^ UnaryExpr }
UnaryExpr ::= [( - | + )] PrimaryExpr
PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | (Expr)
