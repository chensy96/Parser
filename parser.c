#include <stdio.h>
#include <ctype.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int line = 0;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
void skipcomment();

void expr();
void term();
void factor();
void error(int i);
void program();
void coumpoundStmt();
void stmt();
void assignmentStmt();
void simpleStmt();
void readStmt();
void writeStmt();
void structStmt();
void ifstmt();
void whilestmt();
void simpleexpr();
void relaop();
void sign();

int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define PROGRAM 100
#define PROGNAME 2
#define UNKNOWN 99

/* Token codes program begin ; end := read ( , ) write if then else
while do + - * / = <> < <= >= > #
*/
#define ASSIG 97
#define SIGN 96
#define BEGIN 3
#define SEMI 4
#define END 5
#define COLON 6
#define EQUAL 7
#define READ 8
#define COMMA 9
#define WRITE 10
#define IF 11
#define THEN 12
#define ELSE 13
#define WHILE 14
#define DO_OP 15
#define INQUAL 16
#define EXPRSS 17
#define LESS 18
#define MORE 19
#define LESSE 20
#define MOREE 21
#define HASH 22
#define EXPRR 33

#define INT_LIT 23 //constant
#define IDENT 24  //id = variable
#define ASSIGN_OP 25
#define ADD_OP 26
#define SUB_OP 27
#define MULT_OP 28
#define DIV_OP 29
#define LEFT_PAREN 30
#define RIGHT_PAREN 31


/* main driver */
main() {
	/* Open the input data file and process its contents */
	if ((in_fp = fopen("testcase-p3.txt", "r")) == NULL)
		printf("ERROR - cannot open front.in \n");
	else {
		getChar();
		do {
			lex();
			program();
		} while (nextToken != EOF);
	}

}
/*****************************************************/
/* lookup - a function to lookup program begin ; end := read ( , ) write if then else
while do + - * / = <> < <= >= > #
and return the token */

int lookup(char ch) {
	switch (ch) {
	case ';':
		addChar();
		nextToken = SEMI;
		break;
	case ':':
		addChar();
		getChar();
		nextToken = EXPRR;
		if (nextChar == '=') {
			addChar();
			nextToken = ASSIG;
		}
		break;

	case ',':
		addChar();
		nextToken = COMMA;
		break;
	case '=':
		addChar();
		nextToken = EQUAL;
		break;
	case '<':
		addChar();
		nextToken = LESS;
		getChar();
		if (nextChar == '>') {
			addChar();
			nextToken = INQUAL;
		}
		if (nextChar == '=') {
			addChar();
			nextToken = LESSE;
		}
		break;
	case '>':
		addChar();
		nextToken = MORE;
		getChar();
		if (nextChar == '=') {
			addChar();
			nextToken = MOREE;
		}
		break;
	case '#':
		addChar();
		nextToken = HASH;
		break;
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;

	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
	
	if ((nextChar = getc(in_fp)) != EOF) {
		if (nextChar == '\n') {
			line++;
		}
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

int lex() {
	nextToken = initial_lex();
	if (nextToken == IDENT) { 
		if (strcmp(lexeme, "program") == 0) {
			nextToken = PROGRAM;
		}
		else if (strcmp(lexeme, "begin") == 0) {
			nextToken = BEGIN;
		}
		else if (strcmp(lexeme, "end") == 0) {
			nextToken = END;
		}
		else if (strcmp(lexeme, "read") == 0) {
			nextToken = READ;
		}
		else if (strcmp(lexeme, "write") == 0) {
			nextToken = WRITE;
		}
		else if (strcmp(lexeme, "if") == 0) {
			nextToken = IF;
		}
		else if (strcmp(lexeme, "then") == 0) {
			nextToken = THEN;
		}
		else if (strcmp(lexeme, "else") == 0) {
			nextToken = ELSE;
		}
		else if (strcmp(lexeme, "while") == 0) {
			nextToken = WHILE;
		}
		else if (strcmp(lexeme, "do") == 0) {
			nextToken = DO_OP;
		}
		if (lexeme[0] >= 'A' && lexeme[0] <= 'Z') {
			nextToken = PROGNAME;
		}
	}
	printf("Next token is: %d, Next lexeme is %s\n",
		nextToken, lexeme);

	return nextToken;
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar)) {
		getChar();
	}
}

void skipcomment() {
	while (nextChar != '\n') {
		getChar();
	}
	getChar();
}

int initial_lex() {
	lexLen = 0;

	getNonBlank();
	if (nextChar == '#') {
		skipcomment();
	}
	getNonBlank();

	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;
		/* Parse constants */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators (This includes #comment) */
	case UNKNOWN:
		lookup(nextChar);
		if (nextChar != '<' && nextChar != '>' && nextChar != ':') {
			getChar();
		}
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */

	return nextToken;
} /* End of function lex */


void error(int i)
{
	int errorcode = i;
	printf("-----!!!--------ERROR: %i --------!!!------\n", errorcode);
	printf("the error is at line: %i \n", line);
}


//funtions for every non-terminals 

/*<program> -> program <progname> <compound stmt>*/
void program() {
	printf("Enter <program>\n");

	if (nextToken == PROGRAM) {
		lex();
	}
	else {
		error(1);
	}

	if (nextToken == PROGNAME) //check if program name is in the right format
	{
		lex();
	}
	else {
		error(2);
	}

	coumpoundStmt();

	printf("Exit <program>\n");
}

/* <compound stmt> -> begin <stmt> {; <stmt>} end */
void coumpoundStmt() {
	printf("Enter <compound stmt>\n");
	if (nextToken == BEGIN) {
		lex();
	}
	else {
		error(3);
	}

	stmt();

	while (nextToken == SEMI) {
		lex();
		stmt();
	}

	if (nextToken == END) {
		lex();
	}
	else {
		error(4);
	}

	printf("Exit <compound stmt>\n");
}

/*<stmt> -> <simple stmt> | <structured stmt> */
void stmt() {
	printf("Enter <stmt>\n");

	if (nextToken == READ || nextToken == WRITE || nextToken == IDENT || nextToken == PROGNAME) {
		simpleStmt();

	}
	else if (nextToken == BEGIN || nextToken == IF || nextToken == WHILE) {
		structStmt();

	}
	else
	{
		error(5);
	}

	printf("Exit <stmt>\n");
}

/* must start with Var/# <simple stmt> -> <assignment stmt> | <read stmt> | <write stmt> | <comment> */
void simpleStmt() {
	printf("Enter <simpleStmt>\n");

	if (nextToken == READ) {
		readStmt();
	}
	else if (nextToken == WRITE) {
		writeStmt();
	}
	else if (nextToken == IDENT || nextToken == PROGNAME) {
		assignmentStmt();
	}

	printf("Exit <simpleStmt>\n");
}

/* <assignment stmt> -> <variable> := <expression> */
void assignmentStmt() {
	printf("Enter <assignmentStmt>\n");
	if (nextToken == IDENT || nextToken == PROGNAME) {
		lex();
		if (nextToken == ASSIG) {
			lex();
		}
		else {
			error(6);
		}
		expr();
	}
	else
	{
		error(7);
	}
	printf("Exit <assignmentStmt>\n");
}

/* <read stmt> -> read ( <variable> { , <variable> } ) */
void readStmt() {
	printf("Enter <read Stmt>\n");

	if (nextToken == READ) {
		lex();

		if (nextToken == LEFT_PAREN) {
			lex();
			if (nextToken == IDENT || nextToken == PROGNAME) {
				lex();
			}
			else
			{
				error(8);
			}
			while (nextToken == COMMA)
			{
				lex();
				if (nextToken == IDENT || nextToken == PROGNAME) {
					lex();
				}
				else {
					error(9);
				}
			}

			if (nextToken == RIGHT_PAREN) {
				lex();
			}
			else {
				error(10);
			}
		}
		else
		{
			error(11);
		}
	}
	else {
		error(12);
	}
	printf("Exit <read Stmt>\n");

}

/* <write stmt> -> write ( <expression> { , <expression> } ) */
void writeStmt() {
	printf("Enter <write Stmt>\n");

	if (nextToken == WRITE) {
		lex();

		if (nextToken == LEFT_PAREN) {
			lex();
			expr();

			while (nextToken == COMMA)
			{
				lex();
				expr();  
			}

			if (nextToken == RIGHT_PAREN) {
				lex();
			}
			else {
				error(13);
			}

		}
		else
		{
			error(14);
		}
	}
	else {
		error(15);
	}
	printf("Exit <write Stmt>\n");

}

/* <structured stmt> -> <compound stmt> | <if stmt> | <while stmt> */
void structStmt() {
	printf("Enter <struct Stmt>\n");

	if (nextToken == BEGIN) {
		coumpoundStmt();
	}

	if (nextToken == IF) {
		ifstmt();
	}

	if (nextToken == WHILE) {
		whilestmt();
	}

	printf("Exit <struct Stmt>\n");

}

/* <if stmt> ::= if <expression> then <stmt> |
if <expression> then <stmt> else <stmt> */
void ifstmt() {
	printf("Enter <if Stmt>\n");


	if (nextToken == IF) {
		lex();
	}
	else {
		error(16);
	}

	expr(); 

	if (nextToken == THEN)
	{
		lex();
	}
	else {
		error(17);
	}

	stmt(); 


	if (nextToken == ELSE) {
		lex();
		stmt();
	}

	printf("Exit <if Stmt>\n");
}

/* <while stmt> ::= while <expression> do <stmt> */
void whilestmt() {
	printf("Enter <while Stmt>\n");

	if (nextToken == WHILE) {
		lex();
	}
	else {
		error(18);
	}
	expr();
	if (nextToken == DO_OP) {
		lex();
	}
	else {
		error(19);
	}
	stmt();

	printf("Exit <while Stmt>\n");
}


/* <expression> ::= <simple expr> |
<simple expr> <relational_operator> <simple expr> */
void expr() {
	printf("Enter <expr>\n");

	simpleexpr(); 

	if (nextToken == EQUAL || nextToken == INQUAL || nextToken == LESS || nextToken == LESSE || nextToken == MORE || nextToken == MOREE) {
		relaop(); 
		simpleexpr(); 
	}

	printf("Exit <expr>\n");
}

/* <simple expr> ::= [ <sign> ] <term> { <adding_operator> <term> } */
void simpleexpr() {
	printf("Enter <simple expr>\n");
	if (nextToken == ADD_OP || nextToken == SUB_OP) { 
		sign(); 
	}
	/* Parse the first term */
	term(); 

	/* As long as the next token is + or -, get
			the next token and parse the next term */
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
	printf("Exit <simple expr>\n");
} /* End of function expr */


  /* term
  Parses strings in the language generated by the rule:
  <term> -> <factor> {(* | /) <factor>}
  */
void term() {
	printf("Enter <term>\n");
	/* Parse the first factor */
	factor();
	/* As long as the next token is * or /, get the
	next token and parse the next factor */
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		lex();
		factor();
	}
	printf("Exit <term>\n");
} /* End of function term */


  /* factor
  Parses strings in the language generated by the rule:
  <factor> -> variable | int_constant | ( <expr> )
  */
void factor() {
	printf("Enter <factor>\n");
	/* Determine which RHS */
	if (nextToken == IDENT || nextToken == PROGNAME || nextToken == INT_LIT)
		/* Get the next token */
		lex();
	/* If the RHS is ( <expr>), call lex to pass over the
	left parenthesis, call expr, and check for the right
	parenthesis */
	else  if (nextToken == LEFT_PAREN) {
		lex();
		expr();
		if (nextToken == RIGHT_PAREN) {
			lex();
		}
		else
		{
			error(20);
		}
	} /* End of if (nextToken == ... */
	  /* It was not an id, an integer literal, or a left
	  parenthesis */
	else {
		error(21);
	}

	printf("Exit <factor>\n");
} /* End of function factor */

  /* <relational_operator> ::= = | <> | < | <= | >= | > */
void relaop() {
	printf("Enter <relational operator>\n");
	if (nextToken == EQUAL || nextToken == INQUAL || nextToken == LESS || nextToken == LESSE || nextToken == MORE || nextToken == MOREE)
	{
		lex();
	}
	else {
		error(22);
	}
	printf("Exit <relational operator>\n");
}

/* <sign> ::= + | - */
void sign()
{
	printf("Enter <sign>\n");
	if (nextToken == ADD_OP || nextToken == SUB_OP)
	{
		lex();
	}
	else {
		error(23);
	}
	printf("Exit <sign>\n");
}

