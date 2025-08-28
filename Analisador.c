#include <stdio.h>
#include <stdlib.h>

int main()
{
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
        while (c == 123 || c == 32 && c != EOF)
        {
            if (c == 123)
            {
                while (c != 125 && c != EOF)
                {
                    c = fgetc(file);
                }
                c = fgetc(file);
                ;
                printf("%c\n", c);
            }
        }
    }

    fclose(file);

    return 0;
}