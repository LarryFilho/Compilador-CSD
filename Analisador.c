#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char simbolo[100];
    char lexema[100];
} Token;

void TrataDigito(char c,FILE *file, Token *TabelaToken)
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

    strcpy(TabelaToken->simbolo, "snumero");
    strcpy(TabelaToken->lexema, num);


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

void TrataIdentificador(char c,FILE *file, Token *TabelaToken)
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
    strcpy(TabelaToken->lexema, id);

    switch (hash_identificadores(id)) {
        case 1:
            strcpy(TabelaToken->simbolo, "sprograma");
            break;
        case 2:
            strcpy(TabelaToken->simbolo, "sse");
            break;
        case 3:
            strcpy(TabelaToken->simbolo, "sentao");
            break;
        case 4:
            strcpy(TabelaToken->simbolo, "ssenao");
            break;
        case 5:
            strcpy(TabelaToken->simbolo, "senquanto");
            break;
        case 6:
            strcpy(TabelaToken->simbolo, "sfaca");
            break;
        case 7:
            strcpy(TabelaToken->simbolo, "sinicio");
            break;
        case 8:
            strcpy(TabelaToken->simbolo, "sfim");
            break;
        case 9:
            strcpy(TabelaToken->simbolo, "sescreva");
            break;
        case 10:
            strcpy(TabelaToken->simbolo, "sleia");
            break;
        case 11:
            strcpy(TabelaToken->simbolo, "svar");
            break;
        case 12:
            strcpy(TabelaToken->simbolo, "sinteiro");
            break;
        case 13:
            strcpy(TabelaToken->simbolo, "sbooleano");
            break;
        case 14:
            strcpy(TabelaToken->simbolo, "sverdadeiro");
            break;
        case 15:
            strcpy(TabelaToken->simbolo, "sfalso");
            break;
        case 16:
            strcpy(TabelaToken->simbolo, "sprocedimento");
            break;
        case 17:
            strcpy(TabelaToken->simbolo, "sfuncao");
            break;
        case 18:
            strcpy(TabelaToken->simbolo, "sdiv");
            break;
        case 19:
            strcpy(TabelaToken->simbolo, "se");
            break;
        case 20:
            strcpy(TabelaToken->simbolo, "sou");
            break;
        case 21:
            strcpy(TabelaToken->simbolo, "snao");
            break;
        default:
            strcpy(TabelaToken->simbolo, "sidentificador");
    }    
}

void TrataAtribuicao(char c,FILE *file, Token *TabelaToken)
{
    c = fgetc(file);
    if (c == '=')
    {
        strcpy(TabelaToken->simbolo, "satribuicao");
        strcpy(TabelaToken->lexema, ":=");
        c = fgetc(file);
    }
    else
    {
        strcpy(TabelaToken->simbolo, "sdoispontos");
        strcpy(TabelaToken->lexema, ":");
    } 
    if (c != EOF) {
        ungetc(c, file);
    }
}

void TrataOperadorAritmetico(char c,FILE *file, Token *TabelaToken)
{
    if (c == '+')
    {
        strcpy(TabelaToken->simbolo, "smais");
        strcpy(TabelaToken->lexema, "+");
    }
    else if (c == '-')
    {
        strcpy(TabelaToken->simbolo, "smenos");
        strcpy(TabelaToken->lexema, "-");
    }
    else if (c == '*')
    {
        strcpy(TabelaToken->simbolo, "smult");
        strcpy(TabelaToken->lexema, "*");
    }
}

void TrataOperadorRelacional(char c,FILE *file, Token *TabelaToken)
{
    if (c == '>')
    {
        c = fgetc(file);
        if (c == '=')
        {
            strcpy(TabelaToken->simbolo, "smaiorig");
            strcpy(TabelaToken->lexema, ">=");
            c = fgetc(file);
        }else
        {
            strcpy(TabelaToken->simbolo, "smaior");
            strcpy(TabelaToken->lexema, ">");

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
            strcpy(TabelaToken->simbolo, "smenorig");
            strcpy(TabelaToken->lexema, "<=");
            c = fgetc(file);
        }else
        {
            strcpy(TabelaToken->simbolo, "smenor");
            strcpy(TabelaToken->lexema, "<");
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
            strcpy(TabelaToken->simbolo, "sdif");
            strcpy(TabelaToken->lexema, "!=");
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
        strcpy(TabelaToken->simbolo, "sigual");
        strcpy(TabelaToken->lexema, "=");
    }
}

void TrataPontuacao(char c,FILE *file, Token *TabelaToken)
{
    if (c == ';')
    {
        strcpy(TabelaToken->simbolo, "sponto_virgula");
        strcpy(TabelaToken->lexema, ";");
    }
    else if (c == ',')
    {
        strcpy(TabelaToken->simbolo, "svirgula");
        strcpy(TabelaToken->lexema, ",");
    }
    else if (c == '(')
    {
        strcpy(TabelaToken->simbolo, "sabre_parenteses");
        strcpy(TabelaToken->lexema, "(");
    }
    else if (c == ')')
    {
        strcpy(TabelaToken->simbolo, "sfecha_parenteses");
        strcpy(TabelaToken->lexema, ")");
    }
    else if (c == '.')
    {
        strcpy(TabelaToken->simbolo, "sponto");
        strcpy(TabelaToken->lexema, ".");
    }
}

void PegaToken (char c, FILE *file, Token *TabelaToken)
{
    if(c >= '0' && c <= '9')
    {
        TrataDigito(c,file, TabelaToken);
    }
    else if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        TrataIdentificador(c,file, TabelaToken);
    }
    else if(c == ':')
    {
        TrataAtribuicao(c,file, TabelaToken);
    }
    else if(c == '+' || c == '-' || c == '*' )
    {
        TrataOperadorAritmetico(c,file, TabelaToken);
    }
    else if(c == '=' || c == '<' || c == '>' || c == '!')
    {
        TrataOperadorRelacional(c,file, TabelaToken);
    }
    else if(c == ';' || c == ',' || c == '(' || c == ')' || c == '.')
    {
        TrataPontuacao(c,file, TabelaToken);
    }
    else
    {
        printf("Caractere desconhecido: %c\n", c);
        exit(1);
    }
   
}


int main()
{
    Token TabelaToken[100];
    int count = 0;
    char c;
    FILE *file = fopen("arquivo.txt", "r");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int fgetc(FILE * file);

    while ((c = fgetc(file)) != EOF)
    {
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
            PegaToken(c, file, &TabelaToken[count]);
            count++;
        }
    }

    for (int i = 0; i < count; i++) {
        printf("Token %d: simbolo='%s', lexema='%s'\n", i + 1, TabelaToken[i].simbolo, TabelaToken[i].lexema);
    }


    fclose(file);

    return 0;
}