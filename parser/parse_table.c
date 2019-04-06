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

        Terminal** follows2 = NULL;
        int len_follows2 = 0;

        int len_follows = nt_current->follows_size;
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
            grammerRule eps_rule;
            eps_rule.lhs = nt_current;
            eps_rule.rhs = malloc(sizeof(TerminalNonTerminal*));
            eps_rule.num_of_rhs=1;
            eps_rule.id=-2;
            eps_rule.isError=false;
            eps_rule.rhs[0]=malloc(sizeof(TerminalNonTerminal));
            eps_rule.rhs[0]->type='t';
            eps_rule.rhs[0]->s.t=(Terminal *)eps;
            
            for(int j=0;j<len_follows;j++)
            {
                table[nt_current->key][follows[j]->StateId] = eps_rule;
                debug_msg("adding rule for %s:%d %s:%d\n",nt_current->name,nt_current->key,follows[j]->name,follows[j]->StateId);
                print_grammer_rule(eps_rule);
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
// gen_parse_table(NonTerminals *nt, grammerRule *r, no_of_rules, no_of_nt, nt_start_key, rule_start_key){
//     firsts=nt[i].firsts
//     nt[i].follows
//     nt[i].key==i

//     parse_tabel[nt.key][firsts[idx].stateID]=rule[1]
//     r[0]=<program>--> TK_MAIN
//     //nt_start_key points to index of nterminal <program>
// }