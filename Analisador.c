#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS_EXPR 100

typedef struct {
    char simbolo[100];
    char lexema[100];
} Token;

typedef struct {
    char nome[100];
    char tipo [100];
    char escopo;
    int memoria;
}Tabsimb;

typedef struct {
    Token tokens[MAX_TOKENS_EXPR];
    int count;
} VetorTokens;

int numero_linha = 1;
int numero_rotulo = 1;
int alloc_atual = 0;
int memoria_atual = 0;

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
Token analisa_tipo(Token token, FILE *file, int *pc, Tabsimb TABSIMB[]);
Token analisa_variaveis(Token token, FILE *file,int *pc,Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_et_variaveis(Token token, FILE *file, int *pc,Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_atrib_chprocedimento(Token token, FILE *file, int *pc, Tabsimb TABSIMB[]);
Token analisa_leia(Token token, FILE *file, int *pc, Tabsimb TABSIMB[]);
Token analisa_escreva(Token token, FILE *file, int *pc, Tabsimb TABSIMB[]);
Token analisa_fator(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens);
Token analisa_termo(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens);
Token analisa_expressao_simples(Token token, FILE *file, int *pc, Tabsimb TABSIMB[], VetorTokens *vetorTokens);
Token analisa_expressao(Token token, FILE *file, int *pc, Tabsimb TABSIMB[], VetorTokens *vetorTokens);
Token analisa_enquanto(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_se(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_comando_simples(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_comandos(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_declaracao_procedimento(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_declaracao_funcao(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_subrotinas(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_bloco(FILE *file,int *pc,Tabsimb TABSIMB[],FILE *file_saida);
Token analisa_chamada_funcao(Token token,FILE *file);
Token chamada_procedimento(Token token, FILE *file);
void insere_tabela(const char *nome, const char *tipo, char escopo, int memoria, int *pc,Tabsimb TABSIMB[]);
int pesquisa_duplicvar_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[]);
void coloca_tipo_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[]);
int pesquisa_declvar_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[]);
void desempilha_nivel(const char nivel,int *pc,Tabsimb TABSIMB[]);
void imprime_tabela(Tabsimb TABSIMB[], int *pc);
Token analisa_atribuicao(Token token, FILE *file, Tabsimb TABSIMB[], Token token_nome, int *pc);
void trata_expressao_posfix(Token token, FILE *file, VetorTokens vetor_tokens, char saida[]);
int precedencia(char operador);
char acha_caractere_anterior(char saida[], int contador_saida);
int eh_operador_unario(char operador, char caractere_anterior);
void gera(char rotulo[], char instrucao[], char operando1[], char operando2[], FILE *file_saida);

void adiciona_token_vetor(VetorTokens *vetor, Token token) {
    if (vetor->count < MAX_TOKENS_EXPR) {
        vetor->tokens[vetor->count] = token;
        vetor->count++;
    }
}

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


Token analisa_tipo(Token token, FILE *file, int *pc, Tabsimb TABSIMB[])
{
    if(strcmp(token.simbolo,"sinteiro") != 0 && strcmp(token.simbolo,"sbooleano") != 0)
    {
        erro("esperado tipo");
    }else
    {
        coloca_tipo_tabela(token.lexema,pc,TABSIMB);
    }
    token = lexico(file);
    return token;
}

Token analisa_variaveis(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    int dup;
    int vars_count = 1;
    char string[50], string2[50];
    do
    {
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            dup = pesquisa_duplicvar_tabela(token.lexema,pc,TABSIMB);
            if(dup == 0)
            {
                insere_tabela(token.lexema,"variavel",' ',0,pc,TABSIMB);
                token = lexico(file);
                if(strcmp(token.simbolo,"sdoispontos") == 0 || strcmp(token.simbolo,"svirgula") == 0)
                {
                    if(strcmp(token.simbolo,"svirgula") == 0)
                    {
                        token = lexico(file);
                        vars_count++;
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
                erro("variavel ja declarada");
            }   
        }else
        {
            erro("esperado identificador");
        }
    } while (strcmp(token.simbolo,"sdoispontos") != 0);
    sprintf(string, "%d", vars_count);
    sprintf(string2, "%d", alloc_atual);
    gera(" ", "ALLOC", string2, string, file_saida);
    alloc_atual += vars_count;
    token = lexico(file);
    token = analisa_tipo(token,file,pc,TABSIMB);
    return token;
}

Token analisa_et_variaveis(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    if(strcmp(token.simbolo,"svar") == 0)
    {
       token = lexico(file);
       if(strcmp(token.simbolo,"sidentificador") == 0)
       {
        while(strcmp(token.simbolo,"sidentificador") == 0)
        {
            token = analisa_variaveis(token,file,pc,TABSIMB,file_saida);
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

int pega_tipo(const char *lexema,Tabsimb TABSIMB[],int *pc)
{
    char tipo[20];
    int aux = *pc;
    while(aux >= 0)
    {
        if(strcmp(lexema,TABSIMB[aux].nome) == 0 )
        {
            strcpy(tipo,TABSIMB[aux].tipo);
            if(strcmp(tipo,"inteiro") == 0)
            {
                return 1;
            }else
            {
                return 2;
            }
        }else
        {
            aux--;
        }
    }
}



int pega_tipo_expr(char *expressao, Tabsimb TABSIMB[], int *pc) {
    char pilha[100][50]; // Pilha para armazenar tipos ("I" ou "B")
    int topo = -1;
    char token[50];
    int token_index = 0;
    
    for (int i = 0; expressao[i] != '\0'; i++) {
        if (expressao[i] == ' ') {
            if (token_index > 0) {
                token[token_index] = '\0';
                
                // Verificar se é operando (dígito ou variável)
                if (isdigit(token[0])) {
                    // É constante numérica - inteiro
                    strcpy(pilha[++topo], "I");
                } else if (isalpha(token[0])) {
                    // É variável - buscar tipo na tabela de símbolos
                    int tipo = pega_tipo(token, TABSIMB, pc);
                    if (tipo == 1) {
                        strcpy(pilha[++topo], "I");
                    } else if (tipo == 2) {
                        strcpy(pilha[++topo], "B");
                    } else {
                        // Erro - variável não encontrada ou tipo inválido
                        return -1;
                    }
                }
                token_index = 0;
            }
            continue;
        }
        
        // Se for operador
        if (strchr("+-*/=<>eouMSP", expressao[i]) != NULL && token_index == 0) {
            char operador = expressao[i];
            
            switch (operador) {
                // Operadores aritméticos binários: + - * /
                case '+': case '-': case '*': case '/': {
                    if (topo < 1) return -1; // Pilha vazia
                    
                    // Verificar se ambos os operandos são inteiros
                    if (strcmp(pilha[topo], "I") == 0 && strcmp(pilha[topo-1], "I") == 0) {
                        topo--; // Remove dois operandos
                        strcpy(pilha[topo], "I"); // Substitui por I
                    } else {
                        return -1; // Erro de tipo
                    }
                    break;
                }
                
                // Operadores relacionais: = < >
                case '=': case '<': case '>': {
                    if (topo < 1) return -1;
                    
                    // Verificar se ambos os operandos são inteiros
                    if (strcmp(pilha[topo], "I") == 0 && strcmp(pilha[topo-1], "I") == 0) {
                        topo--; // Remove dois operandos
                        strcpy(pilha[topo], "B"); // Substitui por B
                    } else {
                        return -1; // Erro de tipo
                    }
                    break;
                }
                
                // Operadores lógicos binários: e, o, u
                case 'e': case 'o': case 'u': {
                    if (topo < 1) return -1;
                    
                    // Verificar se ambos os operandos são booleanos
                    if (strcmp(pilha[topo], "B") == 0 && strcmp(pilha[topo-1], "B") == 0) {
                        topo--; // Remove dois operandos
                        strcpy(pilha[topo], "B"); // Substitui por B
                    } else {
                        return -1; // Erro de tipo
                    }
                    break;
                }
                
                // Operador de negação: n
                case 'n': {
                    if (topo < 0) return -1;
                    
                    // Verificar se o operando é booleano
                    if (strcmp(pilha[topo], "B") == 0) {
                        strcpy(pilha[topo], "B"); // Mantém como B
                    } else {
                        return -1; // Erro de tipo
                    }
                    break;
                }
                
                // Operadores unários: M (positivo), S (negativo)
                case 'M': case 'S': {
                    if (topo < 0) return -1;
                    
                    // Verificar se o operando é inteiro
                    if (strcmp(pilha[topo], "I") == 0) {
                        strcpy(pilha[topo], "I"); // Mantém como I
                    } else {
                        return -1; // Erro de tipo
                    }
                    break;
                }
            }
        } else {
            // Acumular token (número ou variável)
            token[token_index++] = expressao[i];
        }
    }
    
    // Processar último token se houver
    if (token_index > 0) {
        token[token_index] = '\0';
        if (isdigit(token[0])) {
            strcpy(pilha[++topo], "I");
        } else if (isalpha(token[0])) {
            int tipo = pega_tipo(token, TABSIMB, pc);
            if (tipo == 1) {
                strcpy(pilha[++topo], "I");
            } else if (tipo == 2) {
                strcpy(pilha[++topo], "B");
            } else {
                return -1;
            }
        }
    }
    
    // Verificar resultado final
    if (topo == 0) {
        if (strcmp(pilha[0], "I") == 0) {
            return 1; // Inteiro
        } else if (strcmp(pilha[0], "B") == 0) {
            return 2; // Booleano
        }
    }
    
    return -1; // Erro - expressão mal formada
}

Token analisa_atribuicao(Token token, FILE *file, Tabsimb TABSIMB[], Token token_nome, int *pc)
{
    VetorTokens vetor_tokens;
    vetor_tokens.count = 0;
    int tipo_var,tipo_expr;
    char saida[300];
    /* for(int i = 0; token_nome.lexema[i] != 0 ; i++)
    {
        if(token_nome.lexema[i] == 0)
        {
            break;
        }
        printf("%c", token_nome.lexema[i]);
    }  */ // bloco de código para pegar o nome completo da variavel ou funcao/proc!!!!
    
    tipo_var = pega_tipo(token_nome.lexema,TABSIMB,pc);
    token = lexico(file);
    token = analisa_expressao(token,file,pc,TABSIMB,&vetor_tokens);

    trata_expressao_posfix(token, file,vetor_tokens,saida);
    tipo_expr = pega_tipo_expr(saida,TABSIMB,pc);
    if(tipo_expr == -1)
    {
        erro("expressao com tipos invalidos");
    }
    if(tipo_var != tipo_expr)
    {
        erro("tipos incompativeis");
    }
    return token;
}

void trata_expressao_posfix(Token token, FILE *file,VetorTokens vetorTokens,char *saida)
{
    char pilha[100], operador_atual = ' ', caractere_anterior;
    int contador_saida = 0, contador_pilha = 0, eh_unario;;
    Token tokens;
    int cont = 0;

    for(int i = 0; i < 300; i++) saida[i] = '\0';   //inicializa ambos os vetores com /0 pra evitar problemas com lixo
    for(int i = 0; i < 100; i++) pilha[i] = '\0';

    while(1) //so sai quando der erro ou parar no "break" (terminando de desempilhar a pilha) 
    {
        tokens = vetorTokens.tokens[cont++];

        if(strcmp(tokens.simbolo,"sidentificador") == 0 || strcmp(tokens.simbolo,"snumero") == 0) 
        {
            for(int i = 0; tokens.lexema[i] != '\0'; i++) //verifica se nao é o final da expressao
            {
                saida[contador_saida++] = tokens.lexema[i];  //adiciona o lexema a saida
            }
            saida[contador_saida++] = ' '; //coloca um espaço entre os elementos da "saida"
        }
        else if(strcmp(tokens.simbolo,"sabre_parenteses") == 0) 
        {
            pilha[contador_pilha++] = '('; //empilha se for um "("
        }
        else if(strcmp(tokens.simbolo,"sfecha_parenteses") == 0) 
        {
            while(contador_pilha > 0 && pilha[contador_pilha-1] != '(') //desempilha até achar um abre parenteses
            {
                saida[contador_saida++] = pilha[--contador_pilha];
                saida[contador_saida++] = ' ';  //troca oq havia na pilha por espaço " "
            }
            if(pilha[contador_pilha-1] == '(') 
            {
                contador_pilha--; //remove o abre parenteses da pilha
            } else 
                {
                    erro("Fecha parenteses sem um abre parenteses");
                }
        }
        else if(strcmp(tokens.simbolo,"smais") == 0 || //verifica qual o operador
                strcmp(tokens.simbolo,"smenos") == 0 || 
                strcmp(tokens.simbolo,"sdiv") == 0 ||
                strcmp(tokens.simbolo,"smult") == 0 ||  
                strcmp(tokens.simbolo,"smaior") == 0 ||
                strcmp(tokens.simbolo,"smenor") == 0 || 
                strcmp(tokens.simbolo,"smaiorig") == 0 || 
                strcmp(tokens.simbolo,"smenorig") == 0 || 
                strcmp(tokens.simbolo,"sigual") == 0 || 
                strcmp(tokens.simbolo,"sdif") == 0 ||
                strcmp(tokens.simbolo,"snao") == 0 ||
                strcmp(tokens.simbolo,"se") == 0 ||
                strcmp(tokens.simbolo,"sou") == 0) 
            {

                caractere_anterior = acha_caractere_anterior(saida, contador_saida);

                if(strcmp(tokens.simbolo,"smais") == 0)
                {
                    eh_unario = eh_operador_unario('+', caractere_anterior);

                    if(eh_unario == 1)
                    {
                        operador_atual = 'M'; //simbolo unitario do +
                    } 
                    else if(eh_unario == 0)
                    {
                        operador_atual = '+';
                    }
                }   
                else if(strcmp(tokens.simbolo,"smenos") == 0)
                {
                    eh_unario = eh_operador_unario('-', caractere_anterior);

                    if(eh_unario == 1)
                    {
                        operador_atual = 'S'; //simbolo unitario do -
                    } 
                    else if(eh_unario == 0)
                    {
                        operador_atual = '-';
                    }
                }
                else if(strcmp(tokens.simbolo,"smult") == 0) operador_atual= '*';
                else if(strcmp(tokens.simbolo,"sdiv") == 0) operador_atual= '/';
                else if(strcmp(tokens.simbolo,"smaior") == 0) operador_atual= '>';
                else if(strcmp(tokens.simbolo,"smenor") == 0) operador_atual= '<';
                else if(strcmp(tokens.simbolo,"smaiorig") == 0) operador_atual = 'G'; //simbolo de maior ou igual pq nao fiz pra poder mais de caracter pq operador_atual é um char
                else if(strcmp(tokens.simbolo,"smenorig") == 0) operador_atual= 'L'; //simbolo de menor ou igual
                else if(strcmp(tokens.simbolo,"sigual") == 0) operador_atual= '=';
                else if(strcmp(tokens.simbolo,"sdif") == 0) operador_atual= '!';
                else if(strcmp(tokens.simbolo,"snao") == 0) operador_atual= 'N';
                else if(strcmp(tokens.simbolo,"se") == 0) operador_atual= 'E';
                else if(strcmp(tokens.simbolo,"sou") == 0) operador_atual= '|';
                                
                while(contador_pilha > 0 && 
                    pilha[contador_pilha-1] != '(' &&
                    precedencia(pilha[contador_pilha-1]) >= precedencia(operador_atual))  //desempilha os operadores com MAIOR OU IGUAL precedencia
                {
                    saida[contador_saida++] = pilha[--contador_pilha];
                    saida[contador_saida++] = ' ';
                }

                pilha[contador_pilha++] = operador_atual; //empilha o operador atual
            }
        else //verifica o fim da expressao
        {
            while(contador_pilha > 0) 
            {
                saida[contador_saida++] = pilha[--contador_pilha];  //desempilha todos os operadores restantes
                saida[contador_saida++] = ' ';
            }

            break;
        }
    }

    printf("Expressão posfixa: %s\n", saida); //print teste da posfixa
}

int precedencia(char operador) {
    switch(operador) 
    {
        case 'M': case 'S':  
            return 8;
        case '*': case '/': 
            return 7;
        case '+': case '-':   
            return 6;
        case '>': case '<': case 'G': case 'L': case '=': case '!': 
            return 5;
        case 'N': 
            return 4;
        case 'E': 
            return 3;
        case '|':
            return 2;
        default:
            return 1;
    }
}

char acha_caractere_anterior(char saida[], int contador_saida)  //funcao feita pra achar o primeiro caractere na saida que NAO SEJA ESPAÇO
{
    for(int contador_auxiliar = contador_saida - 1; contador_auxiliar >= 0; contador_auxiliar--)
    {
        if(contador_saida == 0) 
        {
            return '\0';
        }
        else if(saida[contador_auxiliar] != ' ') //percorre todo o vetor a partir do topo decrementando ate achar um caracetere valido e retorna
        {
            return saida[contador_auxiliar];
        }
    }
    return '\0'; //retorna vazio se não encontrar nenhum vlido
}

int eh_operador_unario(char operador, char caractere_anterior) {
    if (operador == '+' || operador == '-') {
        if(caractere_anterior == '(' || caractere_anterior == '\0' || 
           caractere_anterior == '+' || caractere_anterior == '-' || 
           caractere_anterior == '*' || caractere_anterior == '/' ||
           caractere_anterior == '>' || caractere_anterior == '<' || 
           caractere_anterior == 'G' || caractere_anterior == 'L' || 
           caractere_anterior == '=' || caractere_anterior == '!' || 
           caractere_anterior == 'N' || caractere_anterior == 'E' || 
           caractere_anterior == '|' || caractere_anterior == 'M' || 
           caractere_anterior == 'S') {
            return 1; //retorna que EH UNARIOA 
        } else {
            return 0; //retorna que NAO EH UNARIO
        }
    }
}

Token analisa_atrib_chprocedimento(Token token, FILE *file, int *pc, Tabsimb TABSIMB[])
{
    Token token_nome = token;
    token = lexico(file);
    if(strcmp(token.simbolo,"satribuicao") == 0)
    {
        token = analisa_atribuicao(token, file, TABSIMB, token_nome,pc);
        return token;
    }else
    {
        token = chamada_procedimento(token,file);
        return token;
    }
}

Token analisa_leia(Token token, FILE *file, int *pc, Tabsimb TABSIMB[])
{
    int ex = 1,tip;
    token = lexico(file);
    if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            ex = pesquisa_declvar_tabela(token.lexema,pc,TABSIMB);
            tip = pega_tipo(token.lexema,TABSIMB,pc);
            if(ex == 1 && tip == 1)
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
                erro("variavel nao declarada ou tipo errado");
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

Token analisa_escreva(Token token, FILE *file, int *pc, Tabsimb TABSIMB[])
{
    int ex = 1, tip;
    token = lexico(file);
    if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            ex = pesquisa_declvar_tabela(token.lexema,pc,TABSIMB);
            tip = pega_tipo(token.lexema,TABSIMB,pc);
            if(ex == 1 && tip == 1)
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
                erro("variavel nao declarada ou tipo errado");
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

Token analisa_termo(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens)
{
    token = analisa_fator(token, file, pc, TABSIMB,vetorTokens);
    while(strcmp(token.simbolo,"smult") == 0 || strcmp(token.simbolo,"sdiv") == 0 || strcmp(token.simbolo,"se") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        token = analisa_fator(token, file, pc, TABSIMB,vetorTokens);
    }
    return token;
}

Token analisa_expressao_simples(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens)
{
    if(strcmp(token.simbolo,"smais") == 0 || strcmp(token.simbolo,"smenos") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
    }
    token = analisa_termo(token, file, pc, TABSIMB,vetorTokens);
    while(strcmp(token.simbolo,"smais") == 0 || strcmp(token.simbolo,"smenos") == 0 || strcmp(token.simbolo,"sou") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        token = analisa_termo(token, file, pc, TABSIMB,vetorTokens);
    }
    return token;
}
Token analisa_expressao(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens)
{
    
    token = analisa_expressao_simples(token, file, pc, TABSIMB,vetorTokens);
    if(
        strcmp(token.simbolo,"smaior") == 0 ||
        strcmp(token.simbolo,"smenor") == 0 || 
        strcmp(token.simbolo,"smaiorig") == 0 || 
        strcmp(token.simbolo,"smenorig") == 0 || 
        strcmp(token.simbolo,"sigual") == 0 || 
        strcmp(token.simbolo,"sdif") == 0
        )
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        token = analisa_expressao_simples(token, file, pc, TABSIMB,vetorTokens);
        return token;
    }
    return token;
}

int procura_ind(char lexema[], int *pc, Tabsimb TABSIMB[])
{
    int aux = *pc;
    while(aux >= 0)
    {
        if(strcmp(lexema,TABSIMB[aux].nome) == 0)
        {
            return aux;
        }
        aux--;
    }

}

Token analisa_fator(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],VetorTokens *vetorTokens)
{
    int ex;
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        if(pesquisa_declvar_tabela(token.lexema,pc,TABSIMB) == 1)
        {
            int ind = procura_ind(token.lexema,pc,TABSIMB);
            if(strcmp(TABSIMB[ind].tipo,"funcao inteiro") == 0 || strcmp(TABSIMB[ind].tipo,"funcao booleano") == 0)
            {
                token = analisa_chamada_funcao(token, file);
                return token;
            }else
            {
                adiciona_token_vetor(vetorTokens, token);
                token = lexico(file);
                return token;
            }
        }else
        {
            erro("variavel ou funcao nao declarada");
        }
    }else if(strcmp(token.simbolo,"snumero") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        return token;
    }else if(strcmp(token.simbolo,"snao") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        token = analisa_fator(token, file, pc, TABSIMB,vetorTokens);
    }else if(strcmp(token.simbolo,"sabre_parenteses") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        token = analisa_expressao(token, file, pc, TABSIMB,vetorTokens);
        if(strcmp(token.simbolo,"sfecha_parenteses") == 0)
        {
            adiciona_token_vetor(vetorTokens, token);
            token = lexico(file);
            return token;
        }else
        {
            erro("esperado ')'");
        }
    }else if(strcmp(token.simbolo,"sverdadeiro") == 0 || strcmp(token.simbolo,"sfalso") == 0)
    {
        adiciona_token_vetor(vetorTokens, token);
        token = lexico(file);
        return token;
    }else
    {
        erro("esperado fator");
    }
}

Token analisa_enquanto(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    VetorTokens vetor_tokens;
    vetor_tokens.count = 0;

    token = lexico(file);
    token = analisa_expressao(token, file, pc, TABSIMB,&vetor_tokens);
    if(strcmp(token.simbolo,"sfaca") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file,pc,TABSIMB,file_saida);
    }else
    {
        erro("esperado 'faca'");
    }
    return token;
}

Token analisa_se(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    VetorTokens vetor_tokens;
    vetor_tokens.count = 0;

    token = lexico(file);
    token = analisa_expressao(token, file, pc, TABSIMB,&vetor_tokens);
    if(strcmp(token.simbolo,"sentao") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file,pc,TABSIMB,file_saida);
        if(strcmp(token.simbolo,"ssenao") == 0)
        {
            token = lexico(file);
            token = analisa_comando_simples(token, file,pc,TABSIMB,file_saida);
        }
    }else
    {
        erro("esperado 'entao'");
    }
    return token;
}

Token analisa_comando_simples(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        token = analisa_atrib_chprocedimento(token, file, pc, TABSIMB);
    }else if(strcmp(token.simbolo,"sse") == 0)
    {
        token = analisa_se(token, file, pc, TABSIMB,file_saida);
    }else if(strcmp(token.simbolo,"senquanto") == 0)
    {
        token = analisa_enquanto(token, file, pc, TABSIMB,file_saida);
    }else if (strcmp(token.simbolo,"sleia") == 0)
    {
        token = analisa_leia(token, file, pc, TABSIMB);
    }else if (strcmp(token.simbolo,"sescreva") == 0)
    {
        token = analisa_escreva(token, file, pc, TABSIMB);
    }else
    {
        token = analisa_comandos(token,file,pc,TABSIMB,file_saida);
    }
    return token;
}


Token analisa_comandos(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    if(strcmp(token.simbolo,"sinicio") == 0)
    {
        token = lexico(file);
        token = analisa_comando_simples(token, file,pc,TABSIMB,file_saida);
        while(strcmp(token.simbolo,"sfim") != 0)
        {
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = lexico(file);
                if(strcmp(token.simbolo,"sfim") != 0)
                {
                    token = analisa_comando_simples(token,file,pc,TABSIMB,file_saida);
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

Token analisa_declaracao_procedimento(Token token, FILE *file,int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    char nivel = 'L';
    token = lexico(file);
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        int ex = pesquisa_declvar_tabela(token.lexema,pc,TABSIMB);
        if(ex == 0)
        {
            insere_tabela(token.lexema,"nomedeprocedimento",nivel,0,pc,TABSIMB);
            token = lexico(file);
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = analisa_bloco(file,pc,TABSIMB,file_saida);
            }else
            {
                erro("esperado ';'");
            }
        }else
        {
            erro("procedimento ja declarado");
        }
    }else
    {
        erro("esperado identificador");
    }
    desempilha_nivel(nivel,pc,TABSIMB);
    return token;
}

Token analisa_declaracao_funcao(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    char nivel = 'L';
    token = lexico(file);
    if(strcmp(token.simbolo,"sidentificador") == 0)
    {
        int ex = pesquisa_declvar_tabela(token.lexema,pc,TABSIMB);
        if(ex == 0)
        {
            int indice = *pc;
            insere_tabela(token.lexema,"",nivel,0,pc,TABSIMB);
            token = lexico(file);
            if(strcmp(token.simbolo,"sdoispontos") == 0)
            {
                token = lexico(file);
                if(strcmp(token.simbolo,"sinteiro") == 0 || strcmp(token.simbolo,"sbooleano") == 0)
                {
                    if(strcmp(token.simbolo,"sinteiro") == 0)
                    {
                        strcpy(TABSIMB[indice].tipo,"funcao inteiro");
                    }else
                    {
                        strcpy(TABSIMB[indice].tipo,"funcao booleano");
                    }
                    token = lexico(file);
                    if(strcmp(token.simbolo,"sponto_virgula") == 0)
                    {
                        token = analisa_bloco(file,pc,TABSIMB,file_saida);
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
            erro("funcao ja declarada");
        }
    }else
    {
        erro("esperado identificador");
    }

    desempilha_nivel(nivel,pc,TABSIMB);
    return token;
}

Token analisa_subrotinas(Token token, FILE *file, int *pc, Tabsimb TABSIMB[],FILE *file_saida)
{
    while(strcmp(token.simbolo,"sprocedimento") == 0 || strcmp(token.simbolo,"sfuncao") == 0)
    {
        if(strcmp(token.simbolo,"sprocedimento") == 0)
        {
            token = analisa_declaracao_procedimento(token, file, pc, TABSIMB,file_saida);
        }else
        {
            token = analisa_declaracao_funcao(token, file, pc, TABSIMB,file_saida);
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
Token analisa_bloco(FILE *file,int *pc,Tabsimb TABSIMB[],FILE *file_saida)
{
    Token token;
    token = lexico(file);
    token = analisa_et_variaveis(token,file, pc, TABSIMB,file_saida);
    token = analisa_subrotinas(token,file,pc,TABSIMB,file_saida);
    token = analisa_comandos(token,file,pc,TABSIMB,file_saida);
    return token;
}

void insere_tabela(const char *nome, const char *tipo, char escopo, int memoria, int *pc,Tabsimb TABSIMB[])
{
    strcpy(TABSIMB[*pc].nome,nome);
    strcpy(TABSIMB[*pc].tipo,tipo);
    TABSIMB[*pc].escopo = escopo;
    TABSIMB[*pc].memoria = memoria;
    (*pc)++;
}

int pesquisa_duplicvar_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[])
{
    int aux = *pc;
    
    while(TABSIMB[aux].escopo != 'L')
    {
        if(strcmp(lexema,TABSIMB[aux].nome) == 0 )
        {
            return 1;
        }else
        {
            aux--;
        }
    }

    if(strcmp(lexema, TABSIMB[aux].nome) == 0)
    {
        return 1;
    }

    return 0;
}

void coloca_tipo_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[])
{
    int aux = *pc;
    while(aux >= 0)
    {
        if(strcmp(TABSIMB[aux].tipo,"variavel") == 0 )
        {
           if(strcmp(lexema,"inteiro") == 0)
           {
                strcpy(TABSIMB[aux].tipo,"inteiro");
           }else
           {
                strcpy(TABSIMB[aux].tipo,"booleano");
                aux--;
           }
        }else
        {
            aux--;
        }
    }
}

int pesquisa_declvar_tabela(const char *lexema,int *pc,Tabsimb TABSIMB[])
{
    int aux = *pc;
    while(aux >= 0)
    {
        if(strcmp(lexema,TABSIMB[aux].nome) == 0 )
        {
            return 1;
        }else
        {
            aux--;
        }
    }
    return 0;
}

void desempilha_nivel(const char nivel,int *pc,Tabsimb TABSIMB[])
{
    while(TABSIMB[*pc].escopo != nivel)
    {
        (*pc)--;
    }
    TABSIMB[*pc].escopo = ' ';
    (*pc)++;
}


void imprime_tabela(Tabsimb TABSIMB[], int *pc) 
{
    printf("\n=== TABELA DE SÍMBOLOS ===\n");
    for (int i = 0; i < *pc; i++) 
    {
        printf("[%d] Nome: %-15s Tipo: %-15s Escopo: %c Memória: %d\n",i, TABSIMB[i].nome, TABSIMB[i].tipo, TABSIMB[i].escopo, TABSIMB[i].memoria);
    }
}

void gera(char rotulo[], char instrucao[], char operando1[], char operando2[], FILE *file_saida)
{
    fprintf(file_saida, "%s %s %s %s\n", rotulo, instrucao, operando1, operando2);
}

int main()
{
    Token token;
    Tabsimb TABSIMB[100];
    char c;
    FILE *file = fopen("arquivo.txt", "r");
    FILE *file_saida = fopen("saida.txt", "w");
    int pc = 0;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    token = lexico(file);
    if(strcmp(token.simbolo,"sprograma") == 0)
    {
        gera(" ","START"," "," ",file_saida);
        token = lexico(file);
        if(strcmp(token.simbolo,"sidentificador") == 0)
        {
            insere_tabela(token.lexema,"nomedeprograma",'L',0,&pc,TABSIMB);
            token = lexico(file);
            if(strcmp(token.simbolo,"sponto_virgula") == 0)
            {
                token = analisa_bloco(file,&pc,TABSIMB,file_saida);
                if(strcmp(token.simbolo,"sponto") == 0)
                {
                    printf("Análise sintatica concluída com sucesso!\n");
                    imprime_tabela(TABSIMB, &pc);

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