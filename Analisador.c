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

void PegaToken (char c, FILE *file, Token *TabelaToken)
{
    if(c >= '0' && c <= '9')
    {
        TrataDigito(c,file, TabelaToken);
    }
    else if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        printf(" - Token: Identificador\n");
    }
    else
    {
        printf(" - Token: Simbolo\n");
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
        while (c == '{' || c == ' ' && c != EOF)
        {
            if (c == '{')
            {
                while (c != '}' && c != EOF)
                {
                    c = fgetc(file);
                }
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