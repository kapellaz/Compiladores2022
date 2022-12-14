#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantics.h"




void verifica(no raiz){
    if(raiz){
        if(strcmp(raiz->s_type,"Program")==0) nova_classe(raiz->filho->valor);
        else if(strcmp(raiz->s_type, "MethodDecl")==0) {
            verifica_method_decl(raiz->filho);
        
        }else if(strcmp(raiz->s_type, "FieldDecl")==0) insere(raiz->filho,NULL,NULL,"Class");
        
        for(no f = raiz->filho; f!=NULL; f=f->irmao) verifica(f);
        

    }else{
        return;
    }
}


void verifica_method_decl(no node){
    char * tipo = verifica_type(node->filho->s_type);
    char * valor = (char*)strdup(node->filho->irmao->valor);
    char * parametros = verifica_method_params(node->filho->irmao->irmao);
    char ** array_de_parametros = verifica_array_method_params(node->filho->irmao->irmao);

    char *n = malloc(strlen(valor)+strlen(parametros)+1);
    if(n){
        n[0]='\0';
        strcat(n,valor);
        strcat(n,parametros);
    }
    if(verifica_repetidos(n)>0){
        insere(node->filho,parametros,NULL,"Class");

        novo_metodo(n,valor,array_de_parametros,tipo);
        no h = NULL;
        if(node->filho->irmao->irmao->filho){
            h = node->filho->irmao->irmao->filho;
        }
        for(h; h!= NULL; h=h->irmao) {
            if(verifica_repetidos_parametros(n,h->filho->irmao->valor)==0) {
                insere(h->filho,NULL, "param", n);
            }
            else printf("Line %s, Col %s: Symbol %s already defined\n",h->filho->irmao->line ,h->filho->irmao->col ,h->filho->irmao->valor);
        }
    }else{
        node->filho->irmao->repetido=1;
        printf("Line %s, Col %s: Symbol %s already defined\n", node->filho->irmao->line, node->filho->irmao->col,n);
    }
};


char * verifica_method_params(no node){
    if(node->filho){
        char parametros[200] = "(";

        for(no aux = node->filho; aux != NULL; aux = aux->irmao){
            if(strcmp(aux->filho->s_type, "StringArray") == 0) strcat(parametros,"String[]");
            else if(strcmp(aux->filho->s_type, "Double")==0)strcat(parametros, "double");
            else if(strcmp(aux->filho->s_type, "Bool")==0)strcat(parametros, "boolean");
            else if(strcmp(aux->filho->s_type, "Int") == 0)strcat(parametros, "int");
            if(aux->irmao) strcat(parametros,",");
        }
        strcat(parametros,")");
        return strdup(parametros);
    }else{
        return strdup("()");
    }
}


char ** check_calls_method_params(no raiz) {
    char ** params = (char**)malloc(50*sizeof(char*));
    no aux = NULL;
    int i = 1;
    if (raiz->filho->irmao) {
        aux = raiz->filho->irmao;
    }
    while (aux != NULL) {
        if (aux->id != NULL) {
            params[i] = strdup(&aux->id[3]);
            i++;
        }
        aux = aux->irmao;
    }
    char string[10];
    sprintf(string, "%d", i-1);
    params[0] = strdup(string);
    return params;
}



//retorna array com strings a indicar o tipo de parametros
char ** verifica_array_method_params(no node){
    char ** parametros = (char**)malloc(30*sizeof(char*));
    
    int x = 1;

    if(node->filho){

        for(no aux = node->filho; aux != NULL; aux = aux->irmao){
            if(strcmp(aux->filho->s_type, "StringArray") == 0)parametros[x]=strdup("String[]");
            else if(strcmp(aux->filho->s_type, "Double")==0)parametros[x]=strdup("double");
            else if(strcmp(aux->filho->s_type, "Bool")==0)parametros[x]=strdup("boolean");
            else if(strcmp(aux->filho->s_type, "Int") == 0)parametros[x]=strdup("int");
            x++;
        }
    }
    char numero[20];
    sprintf(numero,"%d", x-1);
    parametros[0]=strdup(numero);
    return parametros;
}


char * transforma_type(char* tipo){       
    if (strcmp(tipo, " - boolean") == 0) return "boolean";
    if (strcmp(tipo, " - int") == 0) return "int";
    if (strcmp(tipo, " - String[]") == 0) return "String[]";
    if (strcmp(tipo, " - double") == 0) return "double";
    if (strcmp(tipo, " - void") == 0) return "void";
    if (strcmp(tipo, " - void") == 0) return "void";
    if (strcmp(tipo, "Add") == 0) return "+";
    if (strcmp(tipo, "Eq") == 0) return "==";
    if (strcmp(tipo, "Sub") == 0) return "-";
    if (strcmp(tipo, "Assign") == 0) return "=";
    if (strcmp(tipo, "Mul") == 0) return "*";
    if (strcmp(tipo, "Div") == 0) return "/";
    if (strcmp(tipo, "Mod") == 0) return "%";
    if (strcmp(tipo, " - undef") == 0) return "undef";
    if (strcmp(tipo, "none") == 0) return "none";

    return "none";
}



void verifica_method_body(char * tab, no node){
    if(node){
        
        if(strcmp(node->s_type,"VarDecl")==0) insere(node->filho, NULL,NULL,tab);
 

        if(strcmp(node->s_type,"Id")==0)node->id=procura_tabela(node,tab);

        if(strcmp(node->s_type,"DecLit")==0 || strcmp(node->s_type,"Length")==0 || strcmp(node->s_type,"ParseArgs")==0) node->id=(char*)strdup(" - int");

        if(strcmp(node->s_type, "Ne") == 0 || strcmp(node->s_type, "Eq") == 0 || strcmp(node->s_type, "Ge") == 0 || strcmp(node->s_type, "Gt") == 0 
        || strcmp(node->s_type, "Le") == 0 || strcmp(node->s_type, "Lt") == 0 || strcmp(node->s_type, "Not") == 0 || strcmp(node->s_type, "Or") == 0 
        || strcmp(node->s_type, "And") == 0 || strcmp(node->s_type, "BoolLit") == 0) node->id=(char*)strdup(" - boolean");

        if(strcmp(node->s_type,"StrLit")==0) node->id=(char*)strdup(" - String");

        if(strcmp(node->s_type,"RealLit")==0) node->id=(char*)strdup(" - double");
    
        if(strcmp(node->s_type,"VarDecl")!=0 && strcmp(node->s_type,"Lshift")!=0){
            no aux = node->filho;
            while (aux != NULL) {
                verifica_method_body(tab, aux);
                aux = aux->irmao;
            }
        }

        if(strcmp(node->s_type, "Assign")==0){
            //printf("%s %s\n",node->filho->id,node->filho->irmao->id);
            char * id1=malloc(sizeof(char) * 50);
            char * id2=malloc(sizeof(char) * 50);
            if(node->filho->id==NULL)id1 = "none";
            else id1=node->filho->id;
            if(node->filho->irmao->id==NULL)id2="none";
            else id2=node->filho->irmao->id;
            if(strcmp(id1,id2)!=0){
                printf("Line %s, col %s: Operator %s cannot be applied to types %s, %s\n",node->line,node->col,transforma_type(node->s_type),transforma_type(id1),transforma_type(id2));
            }
            node->id=node->filho->id;
        }


        if(strcmp(node->s_type, "Minus")==0 || strcmp(node->s_type, "Plus")==0)node->id=node->filho->id;

        if (strcmp(node->s_type,"Add")==0 || strcmp(node->s_type,"Sub")==0 
        || strcmp(node->s_type,"Div")==0 || strcmp(node->s_type,"Mul")==0 || strcmp(node->s_type,"Mod")==0)
        {
            if(strcmp(node->filho->id," - boolean")==0 ||strcmp(node->filho->irmao->id," - boolean")==0 || strcmp(node->filho->id," - undef")==0 ||strcmp(node->filho->irmao->id," - undef")==0
             || strcmp(node->filho->id," - String[]")==0 ||strcmp(node->filho->irmao->id," - String[]")==0){
                printf("Line %s, col %s: Operator %s cannot be applied to types %s, %s\n",node->line,node->col,transforma_type(node->s_type),transforma_type(node->filho->id),transforma_type(node->filho->irmao->id));
                node->id=(char*)strdup(" - undef");
            }else{
                //printf("entrou9\n");
            // printf("----%s %s \n", node->filho->id, node->filho->irmao->s_type);
                if(strcmp(node->filho->id, node->filho->irmao->id) != 0) {
                // printf("entrou7\n");
                    node->id=(char*)strdup(" - double");
                    //printf("saiu7\n");
                }
            // printf("nao??aqui\n");
                if(strcmp(node->filho->id, node->filho->irmao->id) == 0) {
                    //printf("entrou8\n");
                    node->id=(char*)strdup(node->filho->id);
                    //printf("saiu8\n");
                }
                //printf("saiu9\n");
            }
        
        }


        if(strcmp(node->s_type,"Call")==0){
            char ** params = check_calls_method_params(node);
            tabela anota = check_call(node->filho->valor, params, 0);
            if (anota != NULL) {
                char * n_string;
                if ((n_string = malloc(strlen(" - ")+strlen(anota->tab->s_type)+1)) != NULL) {
                    n_string[0] = '\0';
                    strcat(n_string, " - ");
                    strcat(n_string, anota->tab->s_type);
                }
                node->id = n_string;
                int i = strlen(anota->c_nome);
                char * n_string2;
                if ((n_string2 = malloc(strlen(" - ")+strlen(&anota->nome[i])+1)) != NULL) {
                    n_string2[0] = '\0';
                    strcat(n_string2, " - ");
                    strcat(n_string2, &anota->nome[i]);
                }
                node->filho->id = n_string2;
            }
            else {
                char * n_string;
                if ((n_string = malloc(strlen(" - undef")+1)) != NULL) {
                    n_string[0] = '\0';
                    strcat(n_string, " - undef");
                }
                node->id = n_string;
                node->filho->id = n_string;
            }
        }

    }else{
        return;
    }
}


void check_ast(no raiz){
    if(raiz){
        if(strcmp(raiz->s_type, "MethodDecl")==0){
            if(raiz->filho->filho->irmao->repetido==0){
                char * params = verifica_method_params(raiz->filho->filho->irmao->irmao);
                char * valor = (char*)strdup(raiz->filho->filho->irmao->valor);

                char * n = malloc(strlen(params)+strlen(valor)+1);
                if(n){
                    n[0]='\0';
                    strcat(n, valor);
                    strcat(n,params);
                }
                verifica_method_body(n,raiz->filho->irmao);
            }
        }

    }else return;
    for(no aux = raiz->filho; aux!=NULL; aux=aux->irmao) check_ast(aux);
}