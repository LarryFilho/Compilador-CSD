#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char simbolo[100];
    char lexema[100];
} Token;

int numero_linha = 1; 

// Declaração de protótipos de todas as funções
void TrataDigito(char c, FILE *file, Token *token);
int hash_identificadores(const char *str);
void TrataIdentificador(char c, FILE *file, Token *token);
void TrataAtribuicao(char c, FILE *file, Token *token);
void TrataOperadorAritmetico(char c, FILE *file, Token *token);
void TrataOperadorRelacional(char c, FILE *file, Token *token);
void TrataPontuacao(char c, FILE *file, Token *token);
Token PegaToken(char c, FILE *file, Token *token);
Token lexico(FILE *file);
Token analisa_tipo(Token token, FILE *file);
Token analisa_variaveis(Token token, FILE *file);
Token analisa_et_variaveis(Token token, FILE *file);
Token analisa_atrib_chprocedimento(Token token, FILE *file);
Token analisa_leia(Token token, FILE *file);
Token analisa_escreva(Token token, FILE *file);
Token analisa_fator(Token token, FILE *file);
Token analisa_termo(Token token, FILE *file);
Token analisa_expressao_simples(Token token, FILE *file);
Token analisa_expressao(Token token, FILE *file);
Token analisa_enquanto(Token token, FILE *file);
Token analisa_se(Token token, FILE *file);
Token analisa_comando_simples(Token token, FILE *file);
Token analisa_comandos(Token token, FILE *file);
Token analisa_declaracao_procedimento(Token token, FILE *file);
Token analisa_declaracao_funcao(Token token, FILE *file);
Token analisa_subrotinas(Token token, FILE *file);
Token analisa_bloco(FILE *file);
Token analisa_chamada_funcao(Token token,FILE *file);
Token chamada_procedimento(Token token, FILE *file);

void erro(const char *mensagem) {
    printf("Erro na linha %d: %s\n", numero_linha, mensagem);
    exit(1);
}

void TrataDigito(char c,FILE *file, Token *token)
{
    char num[20];
    int i = 0;
    num[i] = c;

    c = fgetc(file);

    while (c >= '0' && c <= '9')
    {
        num[++i] = c;
        c = fgetc(file);
    }

    if (c != EOF) {
        ungetc(c, file);
    }

    num[++i] = '\0';

    strcpy(token->simbolo, "snumero");
    strcpy(token->lexema, num);


}

int hash_identificadores(const char *str) {
    if (strcmp(str, "programa") == 0) return 1;
    if (strcmp(str, "se") == 0) return 2;
    if (strcmp(str, "entao") == 0) return 3;
    if (strcmp(str, "senao") == 0) return 4;
    if (strcmp(str, "enquanto") == 0) return 5;
    if (strcmp(str, "faca") == 0) return 6;
    if (strcmp(str, "inicio") == 0) return 7;
    if (strcmp(str, "fim") == 0) return 8;
    if (strcmp(str, "escreva") == 0) return 9;
    if (strcmp(str, "leia") == 0) return 10;
    if (strcmp(str, "var") == 0) return 11;
    if (strcmp(str, "inteiro") == 0) return 12;
    if (strcmp(str, "booleano") == 0) return 13;
    if (strcmp(str, "verdadeiro") == 0) return 14;
    if (strcmp(str, "falso") == 0) return 15;
    if (strcmp(str, "procedimento") == 0) return 16;
    if (strcmp(str, "funcao") == 0) return 17;
    if (strcmp(str, "div") == 0) return 18;
    if (strcmp(str, "e") == 0) return 19;
    if (strcmp(str, "ou") == 0) return 20;
    if (strcmp(str, "nao") == 0) return 21;
    return 0;
}

void TrataIdentificador(char c,FILE *file, Token *token)
{
    char id[50];
    int i = 0;
    id[i] = c;

    c = fgetc(file);

    while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))
    {
        id[++i] = c;
        c = fgetc(file);
    }

    if (c != EOF) {
        ungetc(c, file);
    }

    id[++i] = '\0';
    strcpy(token->lexema, id);

    switch (hash_identificadores(id)) {
        case 1:
            strcpy(token->simbolo, "sprograma");
            break;
        case 2:
            strcpy(token->simbolo, "sse");
            break;
        case 3:
            strcpy(token->simbolo, "sentao");
            break;
        case 4:
            strcpy(token->simbolo, "ssenao");
            break;
        case 5:
            strcpy(token->simbolo, "senquanto");
            break;
        case 6:
            strcpy(token->simbolo, "sfaca");
            break;
        case 7:
            strcpy(token->simbolo, "sinicio");
            break;
        case 8:
            strcpy(token->simbolo, "sfim");
            break;
        case 9:
            strcpy(token->simbolo, "sescreva");
            break;
        case 10:
            strcpy(token->simbolo, "sleia");
            break;
        case 11:
            strcpy(token->simbolo, "svar");
            break;
        case 12:
            strcpy(token->simbolo, "sinteiro");
            break;
        case 13:
            strcpy(token->simbolo, "sbooleano");
            break;
        case 14:
            strcpy(token->simbolo, "sverdadeiro");
            break;
        case 15:
            strcpy(token->simbolo, "sfalso");
            break;
        case 16:
            strcpy(token->simbolo, "sprocedimento");
            break;
        case 17:
            strcpy(token->simbolo, "sfuncao");
            break;
        case 18:
            strcpy(token->simbolo, "sdiv");
            break;
        case 19:
            strcpy(token->simbolo, "se");
            break;
        case 20:
            strcpy(token->simbolo, "sou");
            break;
        case 21:
            strcpy(token->simbolo, "snao");
            break;
        default:
            strcpy(token->simbolo, "sidentificador");
    }    
}

void TrataAtribuicao(char c,FILE *file, Token *token)
{
    c = fgetc(file);
    if (c == '=')
    {
        strcpy(token->simbolo, "satribuicao");
        strcpy(token->lexema, ":=");
        c = fgetc(file);
    }
    else
    {
        strcpy(token->simbolo, "sdoispontos");
        strcpy(token->lexema, ":");
    } 
    if (c != EOF) {
        ungetc(c, file);
    }
}

void TrataOperadorAritmetico(char c,FILE *file, Token *token)
{
    if (c == '+')
    {
        strcpy(token->simbolo, "smais");
        strcpy(token->lexema, "+");
    }
    else if (c == '-')
    {
        strcpy(token->simbolo, "smenos");
        strcpy(token->lexema, "-");
    }
    else if (c == '*')
    {
        strcpy(token->simbolo, "smult");
        strcpy(token->lexema, "*");
    }
}

void TrataOperadorRelacional(char c,FILE *file, Token *token)
{
    if (c == '>')
    {
        c = fgetc(file);
        if (c == '=')
        {
            strcpy(token->simbolo, "smaiorig");
            strcpy(token->lexema, ">=");
            c = fgetc(file);
        }else
        {
            strcpy(token->simbolo, "smaior");
            strcpy(token->lexema, ">");

        }
        if (c != EOF) {
        ungetc(c, file);
    }
    }
    else if (c == '<')
    {
        c = fgetc(file);
        if (c == '=')
        {
            strcpy(token->simbolo, "smenorig");
            strcpy(token->lexema, "<=");
            c = fgetc(file);
        }else
        {
            strcpy(token->simbolo, "smenor");
            strcpy(token->lexema, "<");
        }
        if (c != EOF) {
        ungetc(c, file);
    }
    }
    else if (c == '!')
    {
        c =fgetc(file);
        if (c == '=')
        {
            strcpy(token->simbolo, "sdif");
            strcpy(token->lexema, "!=");
            c = fgetc(file);
        }else
        {
            printf("Erro na linha %d: Caractere desconhecido: !\n", numero_linha);
            exit(1);
        }
        if (c != EOF) {
        ungetc(c, file);
    }
    }else if (c == '=')
    {
        strcpy(token->simbolo, "sigual");
        strcpy(token->lexema, "=");
    }
}

void TrataPontuacao(char c,FILE *file, Token *token)
{
    if (c == ';')
    {
        strcpy(token->simbolo, "sponto_virgula");
        strcpy(token->lexema, ";");
    }
    else if (c == ',')
    {
        strcpy(token->simbolo, "svirgula");
        strcpy(token->lexema, ",");
    }
    else if (c == '(')
    {
        strcpy(token->simbolo, "sabre_parenteses");
        strcpy(token->lexema, "(");
    }
    else if (c == ')')
    {
        strcpy(token->simbolo, "sfecha_parenteses");
        strcpy(token->lexema, ")");
    }
    else if (c == '.')
    {
        strcpy(token->simbolo, "sponto");
        strcpy(token->lexema, ".");
    }
}

Token PegaToken (char c, FILE *file, Token *token)
{
    if(c >= '0' && c <= '9')
    {
        TrataDigito(c,file, token);
    }
    else if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        TrataIdentificador(c,file, token);
    }
    else if(c == ':')
    {
        TrataAtribuicao(c,file, token);
    }
    else if(c == '+' || c == '-' || c == '*' )
    {
        TrataOperadorAritmetico(c,file, token);
    }
    else if(c == '=' || c == '<' || c == '>' || c == '!')
    {
        TrataOperadorRelacional(c,file, token);
    }
    else if(c == ';' || c == ',' || c == '(' || c == ')' || c == '.')
    {
        TrataPontuacao(c,file, token);
    }
    else
    {
        printf("Erro na linha %d: Caractere desconhecido: %c\n", numero_linha, c);
        exit(1);
    }
   
}

Token lexico(FILE *file)
{
    char c = fgetc(file);

    Token token;
    strcpy(token.simbolo, "");
    strcpy(token.lexema, "");

    int fgetc(FILE * file);

    
    while (c == '{' || c == ' ' || c == '\n' || c == '\t' && c != EOF)
    {
        if (c == '{')
        {
            while (c != '}' && c != EOF)
            {
                c = fgetc(file);
                if (c == '\n') numero_linha++;
            }
            c = fgetc(file);
        }
        if (c == '\n')
        {
            numero_linha++;
            c = fgetc(file);
        }
        if (c == '\t')
        {
            c = fgetc(file);
        }
        while (c == ' ' && c != EOF)
        {
            c = fgetc(file);
        }
    }

    if (c != EOF)
    {
        PegaToken(c, file, &token);
    }
    
    return token;
}


Token analisa_tipo(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sinteiro") != 0 && strcmp(token.simbolo,"sbooleano") != 0)
    {
        erro("esperado tipo");
    }
    token = lexico(file);
    return token;
}

Token analisa_variaveis(Token token, FILE *file)
{
    do
    {
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = lexico(file);
            if(strcmp(token.simbolo,"sdoispontos") == 0 || strcmp(token.simbolo,"svirgula") == 0)
            {
                if(strcmp(token.simbolo,"svirgula") == 0)
                {
                    token = lexico(file);
                    if(strcmp(token.simbolo,"sdoispontos") == 0)
                    {
                        erro("dois pontos errado");
                    }
                }
            }else
            {
                erro("esperado ';' ou ','");
            }
        }else
        {
            erro("esperado identificador");
        }
    } while (strcmp(token.simbolo,"sdoispontos") != 0);
    token = lexico(file);
    token = analisa_tipo(token,file);
    return token;
}

Token analisa_et_variaveis(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"svar") == 0)
    {
       token = lexico(file);
       if(strcmp(token.simbolo,"sidentificador") == 0)
       {
        while(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = analisa_variaveis(token,file);
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = lexico(file);
            }else
            {
                erro("esperado ';'");
            }
        } 
       }else
       {
        erro("esperado identificador");
       }
    }

    return token;
}

Token analisa_chamada_funcao(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sidentificador") != 0)
    {
        erro("esperado identificador");
    }
    token = lexico(file);
    return token;
}

Token chamada_procedimento(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sponto_virgula") != 0)
    {
        erro("esperado ';'");
    }
    return token;
}
Token analisa_atrib_chprocedimento(Token token, FILE *file)
{
    token = lexico(file);
    if(strcmp(token.simbolo,"satribuicao") == 0)
    {
        //analisa_atribuicao(); perguntar pro freitas
        token = lexico(file);
        token = analisa_expressao(token,file);
        return token;
    }else
    {
        token = chamada_procedimento(token,file);
        return token;
    }
}

Token analisa_leia(Token token, FILE *file)
{
    token = lexico(file);
    if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = lexico(file);
            if(strcmp(token.simbolo,"sfecha_parenteses") == 0)
            {
                token = lexico(file);
                return token;
            }else
            {
                erro("esperado ')'");
            }
        }else
        {
            erro("esperado identificador");
        }
    }else
    {
        erro("esperado '('");
    }
}

Token analisa_escreva(Token token, FILE *file)
{
    token = lexico(file);
    if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = lexico(file);
            if(strcmp(token.simbolo,"sfecha_parenteses") == 0)
            {
                token = lexico(file);
                return token;
            }else
            {
                erro("esperado ')'");
            }
        }else
        {
            erro("esperado identificador");
        }
    }else
    {
        erro("esperado '('");
    }
}

Token analisa_termo(Token token, FILE *file)
{
    token = analisa_fator(token, file);
    while(strcmp(token.simbolo,"smult") == 0 || strcmp(token.simbolo,"sdiv") == 0 || strcmp(token.simbolo,"se") == 0)
    {
        token = lexico(file);
        token = analisa_fator(token, file);
    }
    return token;
}

Token analisa_expressao_simples(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"smais") == 0 || strcmp(token.simbolo,"smenos") == 0)
    {
        token = lexico(file);
    }
    token = analisa_termo(token, file);
    while(strcmp(token.simbolo,"smais") == 0 || strcmp(token.simbolo,"smenos") == 0 || strcmp(token.simbolo,"sou") == 0)
    {
        token = lexico(file);
        token = analisa_termo(token, file);
    }
    return token;
}
Token analisa_expressao(Token token, FILE *file)
{
    token = analisa_expressao_simples(token, file);
    if(
        strcmp(token.simbolo,"smaior") == 0 ||
        strcmp(token.simbolo,"smenor") == 0 || 
        strcmp(token.simbolo,"smaiorig") == 0 || 
        strcmp(token.simbolo,"smenorig") == 0 || 
        strcmp(token.simbolo,"sigual") == 0 || 
        strcmp(token.simbolo,"sdif") == 0
        )
    {
        token = lexico(file);
        token = analisa_expressao_simples(token, file);
        return token;
    }
    return token;
}

Token analisa_fator(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        token = analisa_chamada_funcao(token, file);
        return token;
    }else if(strcmp(token.simbolo,"snumero") == 0)
    {
        token = lexico(file);
        return token;
    }else if(strcmp(token.simbolo,"snao") == 0)
    {
        token = lexico(file);
        token = analisa_fator(token, file);
    }else if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        token = lexico(file);
        token = analisa_expressao(token, file);
        if(strcmp(token.simbolo,"sfecha_parenteses") == 0)
        {
            token = lexico(file);
            return token;
        }else
        {
            erro("esperado ')'");
        }
    }else if(strcmp(token.simbolo,"sverdadeiro") == 0 || strcmp(token.simbolo,"sfalso") == 0)
    {
        token = lexico(file);
        return token;
    }else
    {
        erro("esperado fator");
    }
}

Token analisa_enquanto(Token token, FILE *file)
{
    token = lexico(file);
    token = analisa_expressao(token, file);
    if(strcmp(token.simbolo,"sfaca") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file);
    }else
    {
        erro("esperado 'faca'");
    }
    return token;
}

Token analisa_se(Token token, FILE *file)
{
    token = lexico(file);
    token = analisa_expressao(token, file);
    if(strcmp(token.simbolo,"sentao") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file);
        if(strcmp(token.simbolo,"ssenao") == 0)
        {
            token = lexico(file);
            token = analisa_comando_simples(token, file);
        }
    }else
    {
        erro("esperado 'entao'");
    }
    return token;
}

Token analisa_comando_simples(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        token = analisa_atrib_chprocedimento(token, file);
    }else if(strcmp(token.simbolo,"sse") == 0)
    {
        token = analisa_se(token, file);
    }else if(strcmp(token.simbolo,"senquanto") == 0)
    {
        token = analisa_enquanto(token, file);
    }else if (strcmp(token.simbolo,"sleia") == 0)
    {
        token = analisa_leia(token, file);
    }else if (strcmp(token.simbolo,"sescreva") == 0)
    {
        token = analisa_escreva(token, file);
    }else
    {
        token = analisa_comandos(token,file);
    }
    return token;
}


Token analisa_comandos(Token token, FILE *file)
{
    if(strcmp(token.simbolo,"sinicio") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file);
        while(strcmp(token.simbolo,"sfim") != 0)
        {
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = lexico(file);
                if(strcmp(token.simbolo,"sfim") != 0)
                {
                    token = analisa_comando_simples(token,file);
                }
            }else
            {
                erro("esperado ';'");
            }
        }
        token = lexico(file);
    }else
        {
            erro("esperado 'inicio'");
        }
    return token;
}

Token analisa_declaracao_procedimento(Token token, FILE *file)
{
    token = lexico(file);
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sponto_virgula") == 0)
        {
            token = analisa_bloco(file);
        }else
        {
            erro("esperado ';'");
        }
    }else
    {
        erro("esperado identificador");
    }
    return token;
}

Token analisa_declaracao_funcao(Token token, FILE *file)
{
    token = lexico(file);
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sdoispontos") == 0)
        {
            token = lexico(file);
            if(strcmp(token.simbolo,"sinteiro") == 0 || strcmp(token.simbolo,"sbooleano") == 0)
            {
                token = lexico(file);
                if(strcmp(token.simbolo,"sponto_virgula") == 0)
                {
                    token = analisa_bloco(file);
                }
            }else
            {
                erro("esperado tipo");
            }
        }else
        {
            erro("esperado ':'");
        }
    }else
    {
        erro("esperado identificador");
    }
    return token;
}

Token analisa_subrotinas(Token token, FILE *file)
{
    while(strcmp(token.simbolo,"sprocedimento") == 0 || strcmp(token.simbolo,"sfuncao") == 0)
    {
        if(strcmp(token.simbolo,"sprocedimento") == 0)
        {
            token = analisa_declaracao_procedimento(token, file);
        }else
        {
            token = analisa_declaracao_funcao(token, file);
        }
        if(strcmp(token.simbolo,"sponto_virgula") == 0)
        {
            token = lexico(file);
        }else
        {
            erro("esperado ';'");
        }
    }
    return token;
}
Token analisa_bloco(FILE *file)
{
    Token token;
    token = lexico(file);
    token = analisa_et_variaveis(token,file);
    token = analisa_subrotinas(token,file);
    token = analisa_comandos(token,file);
    return token;
}

int main()
{
    Token token;
    char c;
    FILE *file = fopen("arquivo.txt", "r");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    token = lexico(file);
    if(strcmp(token.simbolo,"sprograma") == 0)
    {
        
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = lexico(file);
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = analisa_bloco(file);
                if(strcmp(token.simbolo,"sponto") == 0)
                {
                    printf("Análise sintatica concluída com sucesso!\n");
                }else
                {
                    erro("esperado '.'");
                }
            }else
            {
                erro("esperado ';'");
            }
        }else
        {
            erro("esperado identificador");
        }
    }else
    {
        erro("esperado 'programa'");
    }

    
    fclose(file);

    return 0;
}