#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> //para gerar numeros aleatorios
#include <unistd.h> //para congelar a tela em certo momentos
#include <ctype.h> //converter strings em minusculo

void RemoveBarraN(char *p){
    if (p[strlen(p) - 1] == '\n'){
        p[strlen(p) - 1] = '\0';
    }
}

void NumerosAleatorios(int *rand1, int *rand2){ //gera os números de referência para encontrar a palavra no CSV
    srand(time(NULL));
    *rand1 = rand() % 26 + 1; //numero que gera a letra
    *rand2 = rand() % 3 + 1;  //numero que gera a palavra
}

void TranformaEm__(char *copia){
    int i;

    for (i = 0; copia[i] != '\0'; i++){ //transforma o array em _ _ _ _
        copia[i] = '_';
    }
}

void LeLetra(char *letra){
    printf("Digite a letra: ");
    scanf("%c", letra);
    getchar();
}

void MenuInicial(){ //exibe menu inicial
    system("cls");
    printf("\t-----SEJA BEM VINDO AO JOGO DA FORCA-------\n\n");   
    printf("NENHUMA PALAVRA POSSUI CARACTERE ESPECIAL! ");
    sleep(2);
    system("cls");
    printf("Pressione ENTER para continuar ");
    scanf("%*c");
    system("cls");
}

char *EncontraPalavra(FILE *fp, int rand1, int rand2){ //encontra palavra no arquivo CSV
    int i;
    char linha[35], *str, *p;
   
    rewind(fp);

    while(1){
        fgets(linha, 35, fp);

        str = linha; //para encontrar o número
        p = strchr(linha, ',');
        *p = '\0';
        if (atoi(str) == rand1){ //se achar, encerra o loop
            str = p + 1;
            break;
        }
    }
    
    i = 1;

    while(i <= rand2){ //encontra a palavra escolhida
        p = strchr(str, ',');
        if (p == NULL){ //se for NULL, chegou na ultima palavra
            break;
        }

        *p = '\0';

        if (i != rand2){ //str aponta para a palavra apos a virgula se nao chegar na palavra desejada
            str = p + 1;
        }
        i++;
    }

    return str;
}

void DesenhaForca(int *erros){
    switch (*erros){
        case 0:
            printf("  --------|   \n"
                   " |         \n"
                   " |          \n"
                   " |          \n"
                   "_|_         ");
            break;
        case 1: 
            printf(" -------|   \n"
                   " |      O   \n"
                   " |          \n"
                   " |          \n"
                   "_|_          ");
            break;
        case 2:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |      |   \n"
                   " |          \n"
                   "_|_          ");
            break;
        case 3:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |    --|   \n"
                   " |          \n"
                   "_|_          ");
            break;
        case 4:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |    --|-- \n"
                   " |          \n"
                   "_|_          ");
            break;
        case 5:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |    --|-- \n"
                   " |      |   \n"
                   "_|_          ");
            break;
        case 6:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |    --|-- \n"
                   " |      |__ \n"
                   "_|_          ");
            break;
        case 7:
            printf(" -------|   \n"
                   " |      O   \n"
                   " |    --|-- \n"
                   " |    __|__ \n"
                   "_|_          ");
            break;
    }
}

void ExibeForca(char *copia, int *erros){ //exibe a palavra junto com a
    int i;

    DesenhaForca(erros);
    printf("    ");
    for (i = 0; copia[i] != '\0'; i++){ 
        printf("%c ", copia[i]);
    }
    printf("\n");
    printf("Erros: %d/5\n", *erros);
}

void ConverteMinusculo(char *p){
    int i;

    for (i = 0; p[i] != '\0'; i++){
        p[i] = tolower((unsigned char)p[i]);
    }
}

int ProcuraLetra(char *p, char letra){ //verifica se há a letra digitada na palavra original
    if (strchr(p, letra) == NULL){
        return 0;
    }else{
        return 1;
    }
}

int ChutaPalavra(char *str){
    char chute[35];

    printf("\nChute a palavra: ");
    fgets(chute, 35, stdin);
    RemoveBarraN(chute);
    ConverteMinusculo(chute);

    if (strcmp(chute, str) == 0){
        return 1;
    }else{
        return 0;
    }
}

int EncontraIndice(char *str, char *q, char letra){ //descobre indice da letra encontrada no array original 
    char *p = strchr(q, letra);  
    
    return p - str;  
}

int EncontraElemento(char *letrasDigitadas, char letra, int tamLetrasDigitadas){
    for (int i = 0; i < tamLetrasDigitadas; i++){
        if (letrasDigitadas[i] == letra){
            return 0;
        }
    }
    return 1;
}

void ArrumaString(char *copia, char *str, char letra){ //atualiza string cópia quando o usuario acerta letra
    char *p, *q;
    int i;

    q = str;
    while (strchr(q, letra) != NULL){
        p = strchr(q, letra);

        i = EncontraIndice(str, q, letra);
        copia[i] = letra;

        q = p + 1;
    }
}

int VerificaSeDigitou(char *letrasDigitadas, char letra, int *posDigitadas, int tamLetrasDigitadas){
    if (EncontraElemento(letrasDigitadas, letra, tamLetrasDigitadas) == 1) {
        letrasDigitadas[*posDigitadas] = letra;
        (*posDigitadas)++;
        return 0;
    }else{
        printf("\nLetra ja digitada!\n");
        return 1;
    }
}

int main(){
    int rand1, rand2, perdeu; 
    int i, erros, tamCopia, op, acertos, posDigitadas;
    char str[35], letra;
    FILE *fp;

    MenuInicial();

    fp = fopen("alfabeto_palavras.csv", "r"); //abre arquivo CSV
    if (!fp){
        printf("Ja perdeu!");
        return 0;
    }

INICIO: //tag para o goto
    erros = acertos = perdeu = posDigitadas = 0;
    NumerosAleatorios(&rand1, &rand2); 

    strcpy(str, EncontraPalavra(fp, rand1, rand2)); //transfere a palavra encontrada para um array
    RemoveBarraN(str); //se for a ultima palavra da linha, remove o \n 

    char copia[strlen(str)+1]; //cria um array de acordo com o tamanho da palavra encontrada
    strcpy(copia, str);
    TranformaEm__(copia);


    int tamLetrasDigitadas = strlen(str) + 5;
    char letrasDigitadas[tamLetrasDigitadas]; //para guardar todas as letras que o usuário digitou
    for (i = 0; i < strlen(str) + 5; i++){ //para iniciar letrasDigitadas como array vazio
        letrasDigitadas[i] = ' ';
    }

    while (1){
        ExibeForca(copia, &erros);
        
        if (erros > 5){ //verifica se errou mais que 5 vezes
            perdeu = 1; //variavel bool
            break;
        }

        if (strcmp(str, copia) == 0){
            perdeu = 0;
            break;
        }

        if (acertos > 3){
            char op;

            printf("\nVoce deseja chutar a palavra? [S/N]: ");
            scanf("%c", &op);
            getchar();

            if (tolower(op) == 's'){
                ChutaPalavra(str);
                break;
            }
        }

        do{
            printf("Digite a letra: ");
            scanf("%c", &letra);
            getchar();
        } while (VerificaSeDigitou(letrasDigitadas, letra, &posDigitadas, tamLetrasDigitadas) == 1); //retorna 1 se usuario ja digitou e 0 se nao


        if (ProcuraLetra(str, letra) == 1){
            ArrumaString(copia, str, letra);
            acertos++;
        }else{
            erros++;
        }
    }
    sleep(1);

    if (perdeu == 0){
        printf("\nVOCE GANHOU!\n");
    }else{
        printf("\nVOCE PERDEU!\n");
        printf("A palavra era: %s\n", str);
    }

    printf("\nDeseja jogar novamente?\n"
           "1-sim 2-nao: ");
    scanf("%d", &op);
    getchar();
    puts("");

    if (op == 1)
        goto INICIO;

    fclose(fp);

    return 0;
}
