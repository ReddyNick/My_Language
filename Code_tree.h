#ifndef MY_LANGUAGE_Code_tree
#define MY_LANGUAGE_Code_tree

#include "stdio.h"
#include <vector>

namespace Tree_of_code
{

int const CMD_MAXSIZE = 50;

//Commands, TYPE, KEY_WORDS enums are assigned
//to parse tokens and works with them

//out_cmd - output command
//in_cmd  - input command
//IFESLSE - condition with else statement

enum Commands
{
    if_cmd, while_cmd, out_cmd, in_cmd, return_cmd, IFELSE
};

//OP    - operator (e.g +, -, *)
//name  - variable name / function name / command
//VAR   - variable
//punct - punctuation
//fcall - function call
//fdef  - function defenition
//keyword is assigned to built a tree of a programm
//GATHER keeps the tree of a programm binary
//BLOCK - block of statements
enum TYPE
{
    OP, name, VAR, CONST, command, punct, keyword, fcall,
    fdef, assignment, GATHER, BLOCK, Other,
};

enum KEY_WORDS
{
    PROGRAMM, PREMAIN,
};

enum Operations
{
    add = '+', sub = '-', mul = '*', div = '/',
    op_assign = '=', op_more = '>', op_less = '<',
    op_equal = 'e', op_noteq = 'n', op_sqrt = 'S',
};

struct Node
{
    Node* left = nullptr;
    Node* right = nullptr;

    //line - the number of line where a token is located
    int line = 0;
    TYPE type = Other;

    double value = 0;
    char name[CMD_MAXSIZE] = "";
};

class code_tree
{

    char* ptr = nullptr;
    Node* root = nullptr;

    int index = 0;
    int nline = 1;
    std::vector<Node*> code;

    Node* get_Programm();
    Node* get_premain();
    Node* get_main();
    Node* func_defenition();

    Node* get_if();
    Node* get_output();
    Node* get_input();
    Node* get_return();
    Node* get_while();
    Node* get_statements();
    Node* get_statement();
    Node* func_call();
    Node* get_arg();
    Node* get_assign();
    Node* get_id();
    Node* get_E();
    Node* get_E2();
    Node* get_E3();
    Node* get_T();
    Node* get_SQRT();
    Node* get_P();
    Node* get_P2();
    Node* get_atom();
    Node* Block_or_one();
    Node* Unite(TYPE type, Node* newNode, Node* left, Node* right);

    int not_cmd();
    bool end_of_stmt();
    int Check(const char* str);
    bool if_check(Node* node, const char* str);

    int Lexical_analyser();
    int read_num(Node* node);
    int read_name(Node* node);
    int skip_space();

public:

    ~code_tree();

    Node* Read(FILE* input);
    Node* Root() { return root;}

    int Write_DOT(FILE* out);

};

int Dotwrite_elems(Node* elem, int* num, FILE* out);
int Delete_rec(Node* ptr);
Node* CreateNode(TYPE type, double value, Node* left, Node* right);

}

#endif //MY_LANGUAGE_Code_tree
