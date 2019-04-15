#include"code_gen.h"
#include"hash.h"
#include"parserDef.h"
#include"symbolTable.h"
#include"st_entries.h"
#include"st_utils.h"
void operation(void* tv)
{   
    Tree t = (Tree)tv;
    Tree id1 = t->child[0];
    Tree id2 = t->child[1];
    char* operation = (char*)malloc(sizeof(char)*10);
    if(t->tk->state==TK_PLUS)
    {
        operation = "add";
    }
    else if(t->tk->state==TK_MINUS)
    {
        operation = "sub";
    }
    else if(t->tk->state==TK_MUL)
    {
        operation = "mul";
    }
    else
    {
        operation = "div";
    }
    t->code=(char*)malloc(snprintf(NULL,0,"\n\t mov ax, [%s] \n\t \n\t %s ax,[%s] \n\t \n\t mov [%s],ax \n\t",id1->addr,operation,id2->addr,t->addr));
    sprintf(t->code,"\n\t mov ax, [%s] \n\t \n\t %s ax,[%s] \n\t \n\t mov [%s],ax \n\t");
}
void assignmentStmt(void* tv)
{
    Tree t = (Tree)tv;
    Tree id = t->child[0];
    Tree arith = t->child[1];
    char* code;
    code = (char*)malloc(snprintf(NULL,0,"\n\t mov [%s],[%s] \n\t",id->addr,arith->addr));
    t->code = arith->code;
    strcat(t->code,code);
}
void handle_io_stmt(void* tv,SymbolTable gst)
{
    Tree t = (Tree)tv;
    Tree id = t->child[1];
    Tree readOrwrite = t->child[0];
    if(readOrwrite->tk->val==TK_READ)
    {
        if(id->tk->state==TK_RECORDID)
        {
            record_def_entry rec = findST(id->tk->val,gst);
            int size = rec->size;
            char code[300];
            while(size--)
            {
                
            }
        }
        else
        {
            t->code = (char*)malloc(snprintf(NULL,0,"\n\t mov eax,3 \n\t mov ebx,0 \n\t mov ecx,input_buf \n\t int 80h \n\t mov [%s], ecx",id->addr));
            sprintf(t->code,"\n\t mov eax,3 \n\t mov ebx,0 \n\t mov ecx,input_buf \n\t int 80h \n\t mov [%s], ecx",id->addr);
        }
    }
    else
    {
        if(id->tk->state==TK_RECORDID)
        {
            record_def_entry rec = findST(id->tk->val,gst);
        }
        else
        {

        }
    }
}

void iteration(void* tv)
{
    Tree t = (Tree)tv;

    Tree boolean = t->child[0]->child[1];
    Tree counter = t->child[0]->child[0];
    Tree checker = t->child[0]->child[2];

    t->code = (char*)malloc(snprintf(0,NULL,"\n\t while: \n\t mov cx,[%s] \n\t",counter->addr));
    sprintf(t->code,"\n\t while: \n\t mov cx,[%s] \n\t",counter->addr);
    strcat(t->code,t->child[1]->code);
    strcat(t->code,t->child[2]->code);
    char jmp_line[100];
    if(boolean->tk->state==TK_LE)
    {
        sprintf(jmp_line,"\n\t cmp cx,[%s] \n\t jle while \n\t",checker->addr);
        strcat(t->code,jmp_line);
    }
    else if(boolean->tk->state==TK_GE)
    {
        sprintf(jmp_line,"\n\t cmp cx,[%s] \n\t jge while \n\t",checker->addr);
        strcat(t->code,jmp_line);
    }else if(boolean->tk->state==TK_NE)
    {
        sprintf(jmp_line,"\n\t cmp cx,[%s] \n\t jne while \n\t",checker->addr);
        strcat(t->code,jmp_line);
    }
}
