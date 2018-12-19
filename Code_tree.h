#ifndef MY_LANGUAGE_Code_tree
#define MY_LANGUAGE_Code_tree

#include "stdio.h"
#include <vector>

namespace Tree_of_code
{
int const CMD_MAXSIZE = 50;

enum Commands
{
    if_cmd, while_cmd, out_cmd, in_cmd, return_cmd, IFELSE
};

enum TYPE
{
    OP, name, VAR, CONST, command, punct, keyword, fcall,
    fdef, assignment, assign_type, GATHER, BLOCK, Other,
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

    sin = 's', cos = 'c', pow = '^',
};

struct Node
{
    Node* left = nullptr;
    Node* right = nullptr;

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
    std::vector<Node*> code;

    //char str[CMD_MAXSIZE] = "";

    int nline = 1;

    Node* get_Programm();
    Node* get_premain();
    Node* get_main();
    Node* func_defenition();

    bool end_of_stmt();
    Node* get_if();
    Node* get_output();
    Node* get_input();
    Node* get_return();
    Node* get_while();
    Node* get_statements();
    Node* get_statement();
    Node* func_call();
    Node* get_assign();
    Node* get_id();
    Node* get_E();
    Node* get_E2();
    Node* get_E3();
    Node* get_T();
    Node* get_SQRT();
    Node* get_atom();
    Node* get_arg();
    Node* get_param();
    int not_cmd();
    Node* get_P();
    Node* get_P2();
    double get_N();
    Node* Block_or_one();
    int Lexical_analyser();
    int read_num(Node* node);
    int read_name(Node* node);
    int skip_space();

    Node* Unite(TYPE type, Node* newNode, Node* left, Node* right);

    int Check(const char* str);
    bool if_check(Node* node, const char* str);

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
