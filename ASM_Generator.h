
#ifndef MY_LANGUAGE_ASM_GENERATOR_H
#define MY_LANGUAGE_ASM_GENERATOR_H

#include "Code_tree.h"

namespace ASM_Gen
{
    int const BEGIN_OF_RAM = 0;
    int const RAM_SIZE = 100;

    class Generator
    {

        std::vector<Tree_of_code::Node*> TID;

        int G = 0;

        int local_lvl = 0;
        int label = 0;

        int gener_code(Tree_of_code::Node* node, FILE* out);
        int gener_cmd(Tree_of_code::Node* node, FILE* out);
        int gener_declar(Tree_of_code::Node* node, FILE* out);

        int gener_fdef(Tree_of_code::Node* node, FILE* out);
        int put_in_stack(Tree_of_code::Node *node, FILE* out);
        int add_arg(Tree_of_code::Node* node);
        int init_arg(Tree_of_code::Node *node, FILE* out, int* narg);
        int gener_fcall(Tree_of_code::Node* node, FILE* out);
        int gener_arg(Tree_of_code::Node* node, FILE* out, int* narg);

        int count_expr(Tree_of_code::Node* node, FILE* out);
        int gener_assign(Tree_of_code::Node* node, FILE* out);

        int add_id(Tree_of_code::Node* node);
        int find_id(Tree_of_code::Node* node, int lvl);
        int write_var(Tree_of_code::Node* node, FILE* out);

        int gener_condit(Tree_of_code::Node* node, FILE* out);
        int gener_if_cmd(Tree_of_code::Node* node, FILE* out);
        int gener_ifelse_cmd(Tree_of_code::Node* node, FILE* out);
        int gener_while_cmd(Tree_of_code::Node* node, FILE* out);
        int gener_in_cmd(Tree_of_code::Node* node, FILE* out);

    public:

        int Generate_asm(Tree_of_code::Node* root, FILE* output);




    };

    int count_globals(Tree_of_code::Node* node);
}
#endif //MY_LANGUAGE_ASM_GENERATOR_H
