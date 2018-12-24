#include "ASM_Generator.h"
#include "assert.h"
#include "string.h"

namespace ASM_Gen
{
    int Generator::Generate_asm(Tree_of_code::Node *root, FILE *output)
    {
        assert(output != NULL);
        assert(root != nullptr);
        assert(root->type == Tree_of_code::keyword &&
               root->value == Tree_of_code::PROGRAMM);

        Tree_of_code::Node *premain = root->right;
        Tree_of_code::Node *main = root->left;


        if (premain != nullptr && premain->left != nullptr)
        {
            fprintf(output, "//cx is an address which RAM is free after"
                            "\npush 0\npop cx\n\n");

            G = count_globals(premain->left->right);

            gener_declar(premain->left->right, output);

            local_lvl = G;
        }

        if (main != nullptr)
        {
            fprintf(output, "\npush %d\npop cx\n"
                            "\n//___main___//\n", G);

            gener_code(main, output);

            fprintf(output, "end\n"
                            "\n//___end_of_main___//\n");
            TID.resize(G);
        }

        if (premain != nullptr && premain->right != nullptr)
            gener_code(premain->right, output);

        return 0;
    }

    int count_globals(Tree_of_code::Node *node)
    {
        assert(node != nullptr);
        if (node->type == Tree_of_code::GATHER)
            return count_globals(node->left) + count_globals(node->right);
        else
            return 1;
    }

    int Generator::gener_declar(Tree_of_code::Node *node, FILE *out)
    {
        assert(node != nullptr);

        if (node->type == Tree_of_code::GATHER)
        {
            gener_declar(node->left, out);
            gener_declar(node->right, out);
        }
        else
        if (node->type == Tree_of_code::VAR)
            add_id(node);

        else // there is '='
        {
            Tree_of_code::Node* var = node->left;

            add_id(var);

            count_expr(node->right, out);

            if (TID.size() - 1 < G)
                fprintf(out, "push ax\npop [%d]//%s\n", (int)TID.size() - 1, var->name);
            else
                fprintf(out, "push ax\npop [cx + %d]//%s\n", (int)TID.size() - 1 - G, var->name);
        }

        return 0;
    }

    int Generator::add_id(Tree_of_code::Node *node)
    {
        if (find_id(node, local_lvl) != -1)
        {
            printf("Redefenition of '%s' in the %d line!\n", node->name, node->line);
            assert(0);
        }

        node->value = 0;
        TID.push_back(node);

        return 0;
    }

    int Generator::find_id(Tree_of_code::Node *node, int lvl)
    {
        for(int i = TID.size() - 1; i >= lvl; i--)
            if (!strcmp(TID[i]->name, node->name))
            return i;

        return -1;
    }

    int Generator::count_expr(Tree_of_code::Node *node, FILE *out)
    {
        assert(node != nullptr);
#define gen_arg count_expr(node->left, out); fprintf(out, "push ax//to stack\n\n");\
                count_expr(node->right, out); fprintf(out, "push ax//to stack\n\n");

        switch(node->type)
        {
            case Tree_of_code::OP:
                switch ((int) node->value)
                {
                    case Tree_of_code::add:
                    gen_arg;
                        fprintf(out, "add\npop ax\n\n");
                        break;

                    case Tree_of_code::sub:
                    gen_arg;
                        fprintf(out, "sub\npop ax\n\n");
                        break;

                    case Tree_of_code::mul:
                    gen_arg;
                        fprintf(out, "mul\npop ax\n\n");
                        break;

                    case Tree_of_code::div:
                    gen_arg;
                        fprintf(out, "div\npop ax\n\n");
                        break;

                    case Tree_of_code::op_sqrt:
                        count_expr(node->right, out);
                        fprintf(out, "push ax\nsqrt\npop ax\n\n");
                        break;

                    case Tree_of_code::op_assign:
                        fprintf(out, "\n//assignment//\n");
                        gener_assign(node, out);
                        fprintf(out, "//end of assignment//\n\n");
                        break;

                    default:
                        printf("mistake in the %d line!\n"
                               "impossible to use condition expression in this place\n", node->line);
                        assert(0);
                }
                break;

            case Tree_of_code::VAR:
                write_var(node, out);
                break;

            case Tree_of_code::CONST:
                fprintf(out, "push %d//const\npop ax\n\n", (int)node->value);
                break;

            case Tree_of_code::fcall:
                gener_fcall(node, out);
        }

        return 0;
    }

    int Generator::gener_assign(Tree_of_code::Node *node, FILE *out)
    {
        Tree_of_code::Node *var = node->left;

        int address = find_id(var, 0);
        if (address == -1)
        {
            printf("use of undeclared identifier '%s' in the %d line!\n", var->name, var->line);
            assert(0);
        }

        count_expr(node->right, out);

        if (address < G)
            fprintf(out, "push ax\npop [%d]//%s\n", address, var->name);
        else
            fprintf(out, "push ax\npop [cx + %d]//%s\n", address - G, var->name);

        return 0;
    }

    int Generator::write_var(Tree_of_code::Node *node, FILE *out)
    {
        int add = find_id(node, 0);

        if (add == -1)
        {
            printf("use of undeclared identifier '%s' in the %d line!\n", node->name, node->line);
            assert(0);
        }

        if (add < G)
            fprintf(out, "push [%d]//%s\npop ax\n\n", add, node->name);
        else
            fprintf(out, "push [cx + %d]//%s\npop ax\n\n", add - G, node->name);

        return 0;
    }

    int Generator::gener_cmd(Tree_of_code::Node *node, FILE *out)
    {
        switch((int)node->value)
        {
            case Tree_of_code::if_cmd:
                fprintf(out, "\n//if-expression//\n");

                gener_if_cmd(node, out);

                fprintf(out, "//end of if-expression//\n");
                break;

            case Tree_of_code::IFELSE:
                fprintf(out, "\n//if/else-expression//\n");

                gener_ifelse_cmd(node, out);

                fprintf(out, "//end of if/else-expression//\n");
                break;

            case Tree_of_code::while_cmd:
                fprintf(out, "\n//while-expression//\n");

                gener_while_cmd(node, out);

                fprintf(out, "//end of while-expression//\n");
                break;

            case Tree_of_code::out_cmd:
                gener_code(node->right, out);

                fprintf(out, "push ax\n");
                fprintf(out,"out\n");
                break;

            case Tree_of_code::in_cmd:
                gener_in_cmd(node, out);
                break;

            case Tree_of_code::return_cmd:
                gener_code(node->right, out);

                fprintf(out,"\nret\n");
                break;

            default:
                printf("invalid command! in the %d line!", node->line);
        }

        return 0;
    }

    int Generator::gener_condit(Tree_of_code::Node *node, FILE *out)
    {
        gener_code(node->right, out); // <-- b
        fprintf(out, "push ax//to stack\n\n");

        gener_code(node->left, out);  // <-- a
        fprintf(out, "push ax//to stack\n\n");

        switch((int)node->value)
        {
            case Tree_of_code::op_more:
                fprintf(out, "jbe label_%d:\n", label);
                break;
            case Tree_of_code::op_equal:
                fprintf(out, "jne label_%d:\n", label);
                break;
            case Tree_of_code::op_less:
                fprintf(out, "jae label_%d:\n", label);
                break;
            case Tree_of_code::op_noteq:
                fprintf(out, "je label_%d:\n", label);
                break;
            default:
                printf("It is not a logic expression in the %d line!\n", node->line);
                assert(0);
        }
        return 0;
    }

    int Generator::gener_while_cmd(Tree_of_code::Node *node, FILE *out)
    {
        fprintf(out, "label_%d:\n", label);

        int cur_label = label++;

        gener_code(node->left, out);

        int nextl = label;

        fprintf(out, "\n//(condition)//\n");

        label = cur_label;

        gener_condit(node->right, out);

        fprintf(out, "//(condition)//\n");

        label = nextl;

        return 0;
    }

    int Generator::gener_ifelse_cmd(Tree_of_code::Node *node, FILE *out)
    {
        Tree_of_code::Node* if_node = node->left;

        fprintf(out, "\n//(condition)//\n");

        gener_condit(if_node->right, out);

        int cur_label1 = label++;

        fprintf(out, "//(condition)//\n");

        gener_code(if_node->left, out);

        int cur_label2 = label++;

        fprintf(out, "jmp label_%d:\n", cur_label2);

        fprintf(out, "label_%d:\n", cur_label1);

        gener_code(node->right, out);

        fprintf(out, "label_%d:\n", cur_label2);

        return 0;
    }

    int Generator::gener_if_cmd(Tree_of_code::Node *node, FILE *out)
    {
        fprintf(out, "\n//(condition)//\n");

        gener_condit(node->right, out);

        int cur_label = label++;

        fprintf(out, "//(condition)//\n");

        gener_code(node->left, out);

        fprintf(out, "label_%d:\n", cur_label);

        return 0;
    }

    int Generator::gener_in_cmd(Tree_of_code::Node *node, FILE *out)
    {

        Tree_of_code::Node *var = node->right;

        int address = find_id(var, 0);
        if (address == -1)
        {
            printf("use of undeclared identifier '%s' in the %d line!\n", var->name, var->line);
            assert(0);
        }

        if (address < G)
            fprintf(out, "in \npop [%d]//%s\n", address, var->name);
        else
            fprintf(out, "in \npop [cx + %d]//%s\n", address - G, var->name);

        return 0;
    }

    int Generator::gener_code(Tree_of_code::Node *node, FILE *out)
    {
        assert(node != nullptr);

        switch(node->type)
        {
            case Tree_of_code::assignment:
                gener_declar(node->right, out);
                break;

            case Tree_of_code::OP:
                count_expr(node, out);
                break;

            case Tree_of_code::CONST:
                fprintf(out, "push %d//const \npop ax\n\n", (int)node->value);
                break;

            case Tree_of_code::VAR:
                write_var(node, out);
                break;

            case Tree_of_code::command:
                gener_cmd(node, out);
                break;

            case Tree_of_code::GATHER:
                gener_code(node->left, out);
                gener_code(node->right, out);
                break;

            case Tree_of_code::BLOCK:
            {
                int old_local_lvl = local_lvl;
                local_lvl = TID.size();

                gener_code(node->right, out);

                local_lvl = old_local_lvl;
                break;
            }

            case Tree_of_code::fcall:
                gener_fcall(node, out);
                break;
            case Tree_of_code::fdef:
                gener_fdef(node, out);
                break;

            default:

                printf("generating error! node->name = %s, node->value = %lg\n"
                       "node->type = %d in the %d line.", node->name, node->value,
                       node->type, node->line);
                assert(0);
            }

            return 0;
    }

    int Generator::gener_fcall(Tree_of_code::Node *node, FILE *out)
    {
        int narg = 0;

        fprintf(out, "\n//function_call\n\n push cx\n");

        put_in_stack(node->right, out);

        fprintf(out, "push cx \npush %d \nadd\n pop cx\n\n", (int)TID.size());
        fprintf(out, "\n//initialize arguments\n");

        init_arg(node->right, out, &narg);

        fprintf(out, "call %s:\n pop cx\n\n", node->name);

        return 0;
    }

    int Generator::init_arg(Tree_of_code::Node *node, FILE *out, int* narg)
    {
        if (node->type == Tree_of_code::GATHER)
        {
            init_arg(node->left, out, narg);
            init_arg(node->right, out, narg);
        }
        else
        {
            fprintf(out, "pop [cx + %d]\n", *narg);
            (*narg)++;
        }

        return 0;
    }

    int Generator::put_in_stack(Tree_of_code::Node *node, FILE* out)
    {
        switch(node->type)
        {
            case Tree_of_code::GATHER:
            {
                put_in_stack(node->right, out);
                put_in_stack(node->left, out);
            }
            break;

            case Tree_of_code::VAR:
                write_var(node, out);
                fprintf(out, "push ax\n");
                break;

            case Tree_of_code::CONST:
                fprintf(out, "push %d\n", (int)node->value);
                break;

            case Tree_of_code::fcall:
                gener_fcall(node, out);
                fprintf(out, "push ax\n");
                break;

            case Tree_of_code::OP:
                count_expr(node, out);
                fprintf(out,"push ax\n");
                break;

            default:
                printf("generating of arguments error! node->name = %s, node->value = %lg\n"
                       "node->type = %d in the %d line.", node->name, node->value,
                       node->type, node->line);
                assert(0);
        }
        return 0;
    }

    int Generator::gener_fdef(Tree_of_code::Node *node, FILE *out)
    {
        fprintf(out, "\n\n//___function %s___//\n", node->name);

        add_arg(node->right);

        fprintf(out, "%s:\n", node->name);

        gener_code(node->left, out);

        fprintf(out, "ret\n\n//___end of function %s___//\n", node->name);
        TID.resize(G);
        return 0;
    }

    int Generator::add_arg(Tree_of_code::Node *node)
    {
        if (node->type == Tree_of_code::GATHER)
        {
            add_arg(node->left);
            add_arg(node->right);
        }
        else
            TID.push_back(node);

        return 0;
    }
}