#include "Code_tree.h"

#include "assert.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"
//#define Check(a) strcpy(str,#a); Check1();
//#define if_check(a, b) strcpy(str, #b), if_check1(a)


namespace Tree_of_code
{

    Node* code_tree::get_Programm()
    {
        root        = new Node;
        root->type  = keyword;
        root->value = PROGRAMM;
        strcpy(root->name, "PROGRAMM");


        Check("Рецепт"); Check("\"");

        while (!if_check(code[index], "\""))
            index++;

        Check("\"");

        root->right = get_premain();
        root->left  = get_main();
        
        return root;
    }

    bool code_tree::if_check(Node* node, const char* str)
    {
        if (node != nullptr && !strcmp(node->name, str))
            return true;

        return false;
    }

    Node *code_tree::get_premain()
    {
        Node* premain  = new Node;
        premain->type  = keyword;
        premain->value = PREMAIN;
        strcpy(premain->name, "PREMAIN");
        bool empty = true;
        
        if (if_check(code[index], "ингридиенты"))
        {
            premain->left = get_assign();
            empty = false;
        }


        Check("Порядок");

        Check("приготовления");
        Check(":");


        if (if_check(code[index], "инструкция"))
        {
           premain->right = func_defenition();
           empty = false;

           while(if_check(code[index], "инструкция"))
           {
               premain->right = CreateNode(GATHER, GATHER, premain->right, func_defenition());
               strcpy(premain->right->name, "GATHER");
           }
        }

        if (!empty)
            return premain;

        delete premain;
        return nullptr;
    }

    Node *code_tree::get_main()
    {
        Check("ВРУБИТЬ"); Check("МУЗОН"); Check ("!");

        Node* main = get_statements();

        Check("ВЫРУБИТЬ"); Check("МУЗОН"); Check("!");

        Check("Время");  Check("приготовления");
        index++;
        Check("мин");
        return main;
    }

    Node *code_tree::get_statements()
    {

        Node* stmt = nullptr;
        if (!end_of_stmt())
            stmt = get_statement();

        while(!end_of_stmt())
        {

            stmt = CreateNode(GATHER, GATHER, stmt, get_statement());

        }


        return stmt;
    }

    Node *code_tree::get_statement()
    {

        Node* stmt = nullptr;

        if (if_check(code[index], "дегустация"))
            stmt = get_if();
        else
        if (code[index] != nullptr && code[index + 1] != nullptr &&
            if_check(code[index], "взбалтывать") &&
            if_check(code[index + 1], "пока"))
                stmt = get_while();
        else
        if (if_check(code[index], "наСтол"))
            stmt = get_output();
        else
        if (if_check(code[index], "затариться"))
            stmt = get_input();
        else
        if (if_check(code[index], "наТарелку"))
            stmt = get_return();
        else
         if (if_check(code[index], "ингридиенты"))
            stmt = get_assign();
         else
        {

            stmt = get_E();
            Check(";");
        }


        return stmt;
    }

    Node *code_tree::get_return()
    {

        Node* ret = code[index];
        index++;

        ret->type = command;
        ret->value = return_cmd;
        strcpy(ret->name, "return");

        Node* term = get_E();

        Check(";");
        ret->right = term;


        return ret;
    }

    Node *code_tree::get_input()
    {
        Node* in = code[index];
        index++;
        in->type = command;
        in->value = in_cmd;
        strcpy(in->name, "input");

        Node* term = get_id();

        Check(";");
        in->right = term;

        return in;
    }

    Node *code_tree::get_output()
    {
        Node* out = code[index];
        index++;
        out->type = command;
        out->value = out_cmd;
        strcpy(out->name, "output");

        Node* term = get_atom();

        Check(";");
        out->right = term;

        return out;
    }

    Node *code_tree::get_while()
    {
        Node* while_stmt = code[index];
        index += 2;

        while_stmt->type = command;
        while_stmt->value = while_cmd;
        strcpy(while_stmt->name, "while");

        Check("(");

        Node* condit = get_E();

        Check(")");

        while_stmt->right = condit;
        while_stmt->left = Block_or_one();

        return while_stmt;
    }

    Node *code_tree::Block_or_one()
    {
        Node* body = nullptr;

        if (code[index] != nullptr && code[index +1 ] != nullptr &&
            if_check(code[index], "позвать") &&
            if_check(code[index + 1], "повара"))
        {
            Node* block = code[index];
            block->type = BLOCK;
            strcpy(block->name, "BLOCK");

            index += 2;

            body = get_statements();

            Check("отослать");
            Check("повара");

            block->right = body;
            body = block;
        }
        else
            {

            body = get_statement();}


        return body;
    }

    Node *code_tree::get_if()
    {

        Node* ifstmt = code[index];
        index++;

        ifstmt->type = command;
        ifstmt->value = if_cmd;
        strcpy(ifstmt->name, "IF");

        Check("(");

        Node* condit = get_E();

        Check(")");

        ifstmt->right = condit;
        ifstmt->left = Block_or_one();


        if (if_check(code[index], "невкусно"))
        {

            index++;
            ifstmt = CreateNode(command, IFELSE, ifstmt, Block_or_one());
            strcpy(ifstmt->name, "IFELSE");

        }

        return ifstmt;
    }

    bool code_tree::end_of_stmt()
    {


        if (code[index] != nullptr && code[index + 1] != nullptr)
            if (if_check(code[index], "отослать") &&
                if_check(code[index + 1], "повара"))
            {

                //index += 2;
                return true;
            }
            else
            if (code[index + 2] != nullptr &&
                if_check(code[index], "ВЫРУБИТЬ") &&
                if_check(code[index + 1], "МУЗОН") &&
                if_check(code[index + 2], "!"))
            {
               // index += 3;
                return true;
            }

        return false;
    }


    Node *code_tree::func_defenition()
    {
        Check("инструкция"); Check("к");

        if (code[index] == nullptr)
        {
            printf("syntax mistake in the %d line!\n"
                   "function defenition expected.", code[index - 1]->line);
            assert(code[index] != nullptr);
        }

        Node* def = code[index];
        def->type = fdef;
        index++;

        Check("(");

        if (!if_check(code[index],")"))
            def->right = get_arg();

        Check(")");

        Check("позвать");
        Check("повара");

        def->left = get_statements();

        Check("отослать");
        Check("повара");

        return def;
    }

    Node *code_tree::get_assign()
    {
        Check("ингридиенты"); Check(":");

        Node* NEWvar = code[index - 1];
        NEWvar->type = assignment;
        strcpy(NEWvar->name, "ASSIGNMENT");

        Node* assign = get_id();

        if (if_check(code[index], "это"))
        {
            Node* unode = code[index];
            index++;

            unode->value = op_assign;

            assign = Unite(OP, unode, assign, get_E());
        }

        while(if_check(code[index], ","))
        {
            index++;

            Node* newvar = get_id();

            if (if_check(code[index], "это"))
            {
                Node* unode = code[index];

                index++;

                unode->value = op_assign;
                newvar = Unite(OP, unode, newvar, get_E());
            }

            assign = CreateNode(GATHER, GATHER, assign, newvar);
        }

        Check(";");

        NEWvar->right = assign;

        return NEWvar;
    }

    Node *code_tree::get_id()
    {
        if (code[index] == nullptr)
        {
            printf("syntax mistake in the %d line!\n"
                   "variable expected.", code[index - 1]->line);
            assert(code[index] != nullptr);
        }

        Node *newvar =  nullptr;
        if (not_cmd())
        {
            newvar = code[index];
            newvar->type = VAR;
        }
        else
        {
            printf("command word '%s'was found!", code[index]->name);
            assert(0);
        }


        index++;

        return newvar;
    }


    Node* code_tree::get_E()
    {

        Node* node = get_E2();

        while (if_check(code[index], "это") || if_check(code[index], "следует"))
        {
            if (node->type != VAR)
            {
                printf("\nSyntax mistake in the %d line! Not assignable expression.", code[index]->line);
                assert(node->type == VAR);
            }

            Node* unode = code[index];
            index++;

            Node* node2 = get_E2();

            strcpy(unode->name, "=");
            unode->value = op_assign;

            node = Unite(OP, unode, node, node2);
        }
        return node;
    }

    Node *code_tree::get_E2()
    {
        Node* node = get_E3();

        bool more = if_check(code[index], "вкуснее");
        bool less = if_check(code[index], "горче");
        bool equal = if_check(code[index], "на вкус как");
        bool notequal = if_check(code[index], "не сочетается с");

        while (more || less || equal || notequal)
        {
            Node* unode = code[index];


            index ++;

            Node* node2 = get_E3();

            if (more)
            {
                unode->value = op_more;
                node = Unite(OP, unode, node, node2);
            }
            else
            if (less)
            {
                unode->value = op_less;
                node = Unite(OP, unode, node, node2);
            }
            else
            if (equal)
            {
                unode->value = op_equal;
                node = Unite(OP, unode, node, node2);
            }
            else
            if (notequal)
            {
                unode->value = op_noteq;
                node = Unite(OP, unode, node, node2);
            }

            more = if_check(code[index], "вкуснее");
            less = if_check(code[index], "горче");
            equal = if_check(code[index], "на вкус как");
            notequal = if_check(code[index], "не сочетается с");
        }


        return node;
    }

    Node *code_tree::get_E3()
    {
        Node* node = get_T();

        bool ADD = if_check(code[index], "смешать") && if_check(code[index + 1], "с");

        bool SUB = if_check(code[index], "очистить") && if_check(code[index + 1], "от");

        while (ADD || SUB)
        {

            Node* unode = code[index];

            index += 2;

            Node* node2 = get_T();
            if (ADD)
            {
                unode->value = add;
                node = Unite(OP, unode, node, node2);
            }
            else
            {
                unode->value = sub;
                node = Unite(OP, unode, node, node2);
            }

            ADD = if_check(code[index], "смешать") && if_check(code[index + 1], "c");
            SUB = if_check(code[index], "очистить") && if_check(code[index + 1], "от");
        }
        return node;
    }

    Node* code_tree::get_T()
    {
        Node* node = get_SQRT();

        bool MUL = if_check(code[index], "запечь") && if_check(code[index + 1], "с");
        bool DIV = if_check(code[index], "пожарить") && if_check(code[index + 1], "с");

        while (MUL || DIV)
        {
            Node* unode = code[index];
            index += 2;

            Node* node2 = get_SQRT();
            if (MUL)
            {
                unode->value = mul;
                node = Unite(OP, unode, node, node2);
            }
            else
            {
                unode->value = div;
                node = Unite(OP, unode, node, node2);
            }

            MUL = if_check(code[index], "запечь") && if_check(code[index + 1], "с");
            DIV = if_check(code[index], "пожарить") && if_check(code[index + 1], "с");
        }

        return node;
    }

    Node* code_tree::get_SQRT()
    {
        Node* node = nullptr;

        if (if_check(code[index], "посолить"))
        {
            node = code[index];
            node->type = OP;
            node->value = op_sqrt;
            index++;

            Node* node2 = get_SQRT();

            node->right = node2;
        }
        else
            node = get_P();

        return node;
    }

    Node *code_tree::get_P()
    {
        Node* node = nullptr;

        if (if_check(code[index], "очистить") && if_check(code[index + 1], "от"))
        {

                index += 2;

                Node* minus = new Node;
                minus->type = CONST;
                minus->value = -1;

                node = get_SQRT();
                node = CreateNode(OP, mul, minus, node);

            }
        else
            node = get_P2();

        return node;
    }

    Node* code_tree::get_P2()
    {

        if (if_check(code[index], "("))
        {
            index++;

            Node* node = get_E();

            Check(")");

            return node;
        }

        Node* node = get_atom();

        return node;
    }

    Node *code_tree::get_atom()
    {

        Node* atom = nullptr;

        if (code[index] == nullptr)
        {
            printf("syntax mistake in the %d line!\n"
                   "term expected.", code[index - 1]->line);
            assert(code[index] != nullptr);
        }

        if (code[index]->type == name)
        {
            if (!strcmp(code[index]->name, "из"))
                atom = func_call();

            else
            if (not_cmd())
            {
                atom = code[index];
                atom->type = VAR;
                index++;
            }
        }

        else
        if (code[index]->type == CONST)
        {

            atom = code[index];
            index++;
        }
        else
        {
            printf("syntax mistake! in the %d line!\n"
                   "Expected term, found '%s'.\n", code[index]->line, code[index]->name);
            assert(0);
        }

        return atom;
    }

    int code_tree::not_cmd()
    {
        FILE* cmds = fopen("commands.txt", "r");

        assert(cmds != NULL);

        char CMD[CMD_MAXSIZE] = "";

        bool is_cmd = false;

        while(fscanf(cmds, " %s ", CMD) != EOF)
        {
            if (if_check(code[index], CMD))
            {
                is_cmd = true;
                break;
            }
        }
        fclose(cmds);

        if (is_cmd) return 0;

        return 1;
    }

    Node *code_tree::func_call()
    {
        Check("из");

        if (code[index] == nullptr)
        {
            printf("syntax mistake in the %d line!\n"
                   "function name expected.", code[index - 1]->line);
            assert(code[index] != nullptr);
        }

        Node* call = code[index];
        call->type = fcall;

        index++;

        Check("(");

        if (!if_check(code[index],")"))
        {
            Node *arguments = get_E();


            while(if_check(code[index], ","))
            {
                Node* unode = code[index];
                index++;

                unode->type = GATHER;
                unode->value= GATHER;
                strcpy(unode->name, "GATHER");

                arguments = Unite(GATHER, unode, arguments, get_E());
            }

            call->right = arguments;
        }

        Check(")");

        return call;
    }

    Node *code_tree::get_arg()
    {
        Node* arg = nullptr;

        if (if_check(code[index], "из"))
            arg = func_call();
        else
            arg = get_id();

        while (if_check(code[index], ","))
        {
            index++;

            Node* arg2 = nullptr;

            if (if_check(code[index], "из"))
                arg2 = func_call();
            else
                arg2 = get_id();

            arg = CreateNode(GATHER, GATHER, arg, arg2);
        }

        return arg;
    }

    int code_tree::Check(const char* str)
    {
        if (code[index] == nullptr)
        {
            printf("syntax mistake! in the %d line!\n", code[index - 1]->line);
            assert(code[index] != nullptr);
        }
        
        if (strcmp(code[index]->name, str))
        {
            printf("syntax mistake in the %d line!\n", code[index]->line);
            if (code[index]->type == CONST)
                printf("'%s' is expected, %lg was found.\n", str, code[index]->value);
            else
                printf("'%s' is expected, '%s' was found.\n", str, code[index]->name);

            assert(!strcmp(code[index]->name, str));
        }
        
        index++;
        return 0;
    }

    Node *code_tree::Unite(TYPE type, Node *newNode, Node *left, Node *right)
    {
        newNode->type  = type;
        newNode->left  = left;
        newNode->right = right;
        return newNode;
    }

    Node* CreateNode(TYPE type, double value, Node* left, Node* right)
    {
        Node* newNode = new Node;

        strcpy(newNode->name, "GATHER");
        newNode->type  = type;
        newNode->value = value;
        newNode->left  = left;
        newNode->right = right;

        return newNode;
    }

    Node* code_tree::Read(FILE* input)
    {
        assert(input != NULL);

        fseek(input, 0, SEEK_END);
        size_t buf_size = ftell(input);
        rewind(input);

        char* buf = new char[buf_size + 1];
        memset(buf, '\0', buf_size);

        fread(buf, sizeof(buf[0]), buf_size + 1, input);

        ptr = buf;

        Lexical_analyser();

//        int sz = code.size();
//        printf("vec size = %d", code.size());
//
//        for (int i = 0; i < sz - 1; i++)
//        {
//            if (code[i]->type == CONST)
//                printf("%lg\n", code[i]->value);
//            else
//                printf("%s\n", code[i]->name);
//        }

        root = get_Programm();

        delete[] buf;
        return root;
    }

    bool is_RUSalpha(char* ptr)
    {
        if (*ptr == '\0') return false;

        unsigned char s1 = 0, s2 = 0;

        s1 = *ptr;
        s2 = *(ptr + 1);

        bool rus1 = (s1 == 0xD0 && 0x90 <= s2 && s2 <= 0xBF);
        bool rus2 = (s1 == 0xD1 && 0x80 <= s2 && s2 <= 0x8F);

        return rus1 || rus2;
    }

    int code_tree::skip_space()
    {
        while (isspace(*ptr) && *ptr != '\0')
        {
            if (*ptr == '\n') nline++;
            ptr++;
        }

        return 0;
    }

    int code_tree::read_name(Node* node)
    {
        int i = 0;

        bool rus = false;

        while((rus = is_RUSalpha(ptr)) || isalnum(*ptr))
        {
            assert(i < CMD_MAXSIZE);

            if (rus)
            {
                node->name[i] = *ptr;
                node->name[i + 1] = *(ptr + 1);
                ptr += 2; i += 2;
            }
            else
            {
                node->name[i] =*ptr;
                ptr = ptr + 1; i++;
            }
        }

        node->name[i] = '\0';

        //printf("name-> %s\n", node->name);
        char str1[CMD_MAXSIZE] = "";
        char str2[CMD_MAXSIZE] = "";

        int move = 0;
        sscanf(ptr, " %s %s %n", str1, str2, &move);

        if (!strcmp(node->name, "на") && !strcmp(str1, "вкус") && !strcmp(str2, "как"))
        {
            strcpy(node->name, "на вкус как");
            ptr += move;
        }
        else
        if (!strcmp(node->name, "не") && !strcmp(str1, "сочетается") && !strcmp(str2, "с"))
        {
            strcpy(node->name, "не сочетается с");
            ptr += move;
        }

        assert(i < CMD_MAXSIZE);

        return 0;
    }

    int code_tree::read_num(Node *node)
    {
        int i =0;
        node->value = 0;

        while ('0' <= *ptr && *ptr <= '9')
        {
            node->value = node->value * 10 + *ptr - '0';
            ptr++;
        }

        return 0;
    }

    int code_tree::Lexical_analyser()
    {

        while(*ptr != '\0')
        {

            skip_space();
            //if name

            if (*ptr != '\0')
            {
                if (is_RUSalpha(ptr) || isalpha(*ptr))
                {

                    Node *newNode = new Node;
                    newNode->type = name;

                    newNode->line = nline;

                    read_name(newNode);
                    code.push_back(newNode);
                } else
                    //if const
                if ('0' <= *ptr && *ptr <= '9')
                {
                    Node *newNode = new Node;
                    newNode->type = CONST;

                    newNode->line = nline;

                    read_num(newNode);
                    code.push_back(newNode);
                } else
                    //if punct
                {

                    Node *newNode = new Node;
                    newNode->type = punct;

                    newNode->line = nline;

                    newNode->name[0] = *ptr;
                    code.push_back(newNode);
                    ptr++;
                }
            }
        }

        code.push_back(nullptr);

        return 0;
    }

    code_tree::~code_tree()
    {
        if (root != nullptr)
            Delete_rec(root);

        root = nullptr;
    }

    int Delete_rec(Node* ptr)
    {
        if (ptr == nullptr) return 0;

        if (ptr->left != nullptr)
            Delete_rec(ptr->left);

        if (ptr->right != ptr->left)
            if (ptr->right != nullptr)
                Delete_rec(ptr->right);

        delete[] ptr;
        return 0;
    }

    int code_tree::Write_DOT(FILE* out)
    {
        assert(out != NULL);
        assert(root!=nullptr);

#define fillcolor "#FFEBCD"
        fprintf(out,"digraph G {\nnode[shape=ellipse fontsize =15 style=%cfilled%c fillcolor=%c" fillcolor "%c]\n",
                '"', '"', '"', '"');

        int num = 0;
        Dotwrite_elems(root, &num, out);

        fprintf(out, "}");
        return 0;
    }

    int Dotwrite_elems(Node* elem, int* num, FILE* out)
    {
        assert(elem !=nullptr);

        int cur_num = *num;
        if (elem->type == CONST)
            fprintf(out, "node%d[label=%c%lg%c];\n", cur_num, '"', elem->value, '"');
        else
        if(elem->type == OP)
            fprintf(out, "node%d[label=%c%c%c];\n", cur_num, '"', (char)elem->value, '"');
        else
            fprintf(out, "node%d[label=%c%s%c];\n", cur_num, '"', elem->name, '"');

        if (elem->left != nullptr)
        {
            ++*num;
            fprintf(out, "node%d->node%d;\n", cur_num, *num);

            Dotwrite_elems(elem->left, num, out);
        }

        if (elem->right != nullptr)
        {
            ++*num;
            fprintf(out, "node%d->node%d;\n", cur_num, *num);
            Dotwrite_elems(elem->right, num, out);
        }
        return 0;
    }
}


