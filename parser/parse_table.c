//populate the table entries not defined as ErrorRule
#include"parserDef.h"
#include"populate_grammer.h"

//LHS for error rule remains
void init(grammerRule **table)
{
    grammerRule ErrorRule;
    ErrorRule.lhs=NULL;
    ErrorRule.rhs = NULL;
    ErrorRule.num_of_rhs = 0;
    ErrorRule.isError = 1;
    ErrorRule.id=-1;
    for(int i=0;i<NO_OF_NON_TERMINALS;i++)
    {
        //
        for(int j=0;j<NO_OF_TERMINALS;j++)
        {
            //
            table[i][j] = ErrorRule;
        }
    }
}
grammerRule** gen_parse_table(const grammerRule *r, int no_of_rules, const Terminal *eps)
{    
    grammerRule **table=malloc(sizeof(grammerRule*)*NO_OF_NON_TERMINALS);
    for(int i=0;i<NO_OF_NON_TERMINALS;i++)
    {
        table[i]=malloc(sizeof(grammerRule)*NO_OF_TERMINALS);
    }
    init(table);
    int i=0;
    while(i<no_of_rules)
    {
        debug_msg("\n\nrules from ");
        print_grammer_rule(r[i]);
        debug_msg("with lhs firsts size %d\t",r[i].lhs->firsts_size);
        debug_msg("with lhs follow size %d\n",r[i].lhs->follows_size);

        // int isEps = 0;
        int isEpsL = 0;
        NonTerminal *nt_current = r[i].lhs;
        TerminalNonTerminal** tnt_current = r[i].rhs;

        Terminal** first = NULL;
        Terminal** follows = nt_current->follows;
        int len_follows = nt_current->follows_size;
    /* ################ ERROR RECVOERY ############################# */
        for(int i=0;i<len_follows;i++)
        {
            table[nt_current->key][follows[i]->StateId].isSyn=1;
            
        }

        Terminal** firsts_for_error = nt_current->firsts;
        int first_size_for_error = nt_current->firsts_size;
        for(int i=0;i<first_size_for_error;i++)
        {
            table[nt_current->key][firsts_for_error[i]->StateId].isSyn=1;
            table[nt_current->key][firsts_for_error[i]->StateId].part_of_first=1; 
            if(firsts_for_error[i]->StateId==TK_EPS)
            {
                table[nt_current->key][firsts_for_error[i]->StateId].can_be_eps=1;
            }
        }
    /* ################ ERROR RECVOERY ############################# */
        Terminal** follows2 = NULL;
        int len_follows2 = 0;
        if(tnt_current[0]->type=='t')
        {   
            if(tnt_current[0]->s.t->StateId==TK_EPS)
            {
                isEpsL = 1;
            }
            table[nt_current->key][tnt_current[0]->s.t->StateId] = r[i];
            grammerRule temp=table[nt_current->key][tnt_current[0]->s.t->StateId];
            debug_msg("adding rule for %s:%d %s:%d\n",nt_current->name,nt_current->key,tnt_current[0]->s.t->name,tnt_current[0]->s.t->StateId);
            print_grammer_rule(temp);
            debug_msg("\n");
        }
        else
        {
            first = tnt_current[0]->s.nt->firsts;
            int len_first = tnt_current[0]->s.nt->firsts_size;
            int j=0;
            for(;j<len_first;j++)
            {
                if(first[j]->StateId!=TK_EPS)
                {
                    table[nt_current->key][first[j]->StateId] = r[i]; 
                    debug_msg("adding rule for %s:%d %s:%d\n",nt_current->name,nt_current->key,first[j]->name,first[j]->StateId);
                    print_grammer_rule(r[i]);
                    debug_msg("\n");
                }
                else
                {
                    follows2 = tnt_current[0]->s.nt->follows;
                    len_follows2 = tnt_current[0]->s.nt->follows_size;
                    // isEps = 1;
                }
            }
        }
        if(isEpsL)
        {
            for(int j=0;j<len_follows;j++)
            {
                table[nt_current->key][follows[j]->StateId] = r[i];
                debug_msg("adding rule for %s:%d %s:%d\n",nt_current->name,nt_current->key,follows[j]->name,follows[j]->StateId);
                print_grammer_rule(r[i]);
            }
        }
        if(follows2!=NULL)
        {
            for(int j=0;j<len_follows2;j++)
            {
                table[nt_current->key][follows2[j]->StateId] = r[i];
            }
        }
        i++;
    }
    return table;
}