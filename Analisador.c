#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char simbolo[100];
    char lexema[100];
} Token;

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
            printf("Caractere desconhecido: !\n");
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
        printf("Caractere desconhecido: %c\n", c);
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
            }
            c = fgetc(file);
        }
        if (c == '\n')
        {
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
                //analisa_bloco();
                token = lexico(file);
                if(strcmp(token.simbolo,"sponto") == 0)
                {
                    printf("Análise sintatica concluída com sucesso!\n");
                }else
                {
                    printf("Erro: esperado '.'\n");
                    return 1;
                }
            }else
            {
                printf("Erro: esperado ';'\n");
                return 1;
            }
        }else
        {
            printf("Erro: esperado identificador\n");
            return 1;
        }
    }else
    {
        printf("Erro: esperado 'programa'\n");
        return 1;
    }

   

    /*for(int i = 0; i < 10; i++) {
        token =lexico(file);
        printf("Simbolo: %s, Lexema: %s\n", token.simbolo, token.lexema);
    }*/
    
    fclose(file);

    return 0;
}