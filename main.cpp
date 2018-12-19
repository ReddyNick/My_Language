#include "stdio.h"
#include "assert.h"
#include "Code_tree.h"
#include "ASM_Generator.h"
#include "ctype.h"
#include "string.h"


int main()
{

    FILE* in = fopen("fibo.txt", "r");
    FILE* out_asm = fopen("/home/nick/CLionProjects/MY_CPU2/prog.txt", "w");
    assert(in != nullptr);
    assert(out_asm != nullptr);

    Tree_of_code::code_tree tree;
    tree.Read(in);
    fclose(in);

    ASM_Gen::Generator gen;
    gen.Generate_asm(tree.Root(), out_asm);

    fclose(out_asm);

    return 0;
}
