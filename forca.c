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

char *EncontraPalavra(FILE *fp, int rand1, int rand2){
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

void ExibeForca(char *copia, int *erros){
    int i;

    DesenhaForca(erros);
    printf("    ");
    for (i = 0; copia[i] != '\0'; i++){ 
        printf("%c ", copia[i]);
    }
    printf("\n");
    printf("Erros: %d/7\n", *erros);
}

void ConverteMinusculo(char *p){
    int i;

    for (i = 0; p[i] != '\0'; i++){
        p[i] = tolower((unsigned char)p[i]);
    }
}

int ProcuraLetra(char *p, char letra){ 
    if (strchr(p, letra) == NULL){
        return 0;
    }else{
        return 1;
    }
}

int ChutaPalavra(char *p){
    int op;
    char chute[35];

    CHUTE:
        printf("\nDeseja chutar a palavra?\n"
               "1-sim 2-nao: ");
        scanf("%d", &op);
        getchar();
        
        if (op == 2){
            puts("");
            return 2;
        }

        if (op < 1 || op > 2){
            printf("Valor invalido!\n");
            goto CHUTE;
        }

        if (op == 1){
            printf("Adivinhe a palavra: ");
            scanf("%35[^\n]", chute);
            getchar();

            ConverteMinusculo(chute);

            for (int i = 0; chute[i] != '\0'; i++){ 
                if (chute[i] != p[i]){              
                    return 0;                 //foi mal a gambiarra!!! dps ajeito
                }
            }
            return 1;
        }else{
            return 0;
        }
}

int EncontraIndice(char *str, char *q, char letra){ //descobre indice da letra encontrada no array original 
    char *p = strchr(q, letra);  
    
    return p - str;  
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

int main(){
    int rand1, rand2; //para encontrar as palavras aleatorias
    int i, erros, tamCopia, op, tentativas, acertos;
    char str[35], letra;
    FILE *fp;

    printf("\t-----SEJA BEM VIADO AO JOGO DA FORCA-------\n\n");   
    printf("NENHUMA PALAVRA POSSUI CARACTERE ESPECIAL! ");
    sleep(2);
    system("cls");
    printf("Pressione ENTER para continuar ");
    scanf("%*c");
    system("cls");

INICIO: //tag para o goto
    erros = 0;
    tentativas = 0;
    acertos = 0;

    srand(time(NULL));
    rand1 = rand() % 26 + 1; //numero que gera a letra
    rand2 = rand() % 3 + 1;  //numero que gera a palavra

    fp = fopen("alfabeto_palavras.csv", "r");
    if (!fp){
        printf("Ja perdeu!");
        return 0;
    }
    
    strcpy(str, EncontraPalavra(fp, rand1, rand2)); //transfere a palavra encontrada para um array
    RemoveBarraN(str); //se for a ultima palavra da linha, remove o \n 

    char copia[35]; //transfere palavra encontrada para um array de copia
    strcpy(copia, str);

    for (i = 0; copia[i] != '\0'; i++){ //transforma o array em _ _ _ _
        copia[i] = '_';
    }

    printf("PALAVRA: %s\n", str);

    while(erros <= 7){ //enquanto array copia for diferente de array original 
        if (erros == 7){
            ExibeForca(str, &erros);
            sleep(1);
            printf("Voce perdeu!\n");
            printf("A palavra era: %s", str);
            break;
        }

        do{
            ExibeForca(copia, &erros);

            if (tentativas > 3  && acertos > 1){
                op = ChutaPalavra(str);
            
                if (op == 1){
                    break;
                }else if (op == 0){
                    printf("ERROU!\n");
                    erros++;
                    sleep(1);
                }

                ExibeForca(copia, &erros);
            }

            printf("Adivinhe uma letra: ");
            scanf("%c", &letra);
            getchar();
            printf("\n");

            letra = tolower((unsigned char)letra); //transforma a letra em minúscula
            
            if (strchr(copia, letra) != NULL){
                printf("A LETRA JA FOI DIGITADA!!\n");
                sleep(1);
            }
        }while(strchr(copia, letra) != NULL); //se letra já existe no array cópia, entao ja foi digitada
        
        if (op == 1 || strcmp(str, copia) == 0){
            ExibeForca(str, &erros);
            sleep(1);
            printf("Voce ganhou!\n");
            break;
        }

        tentativas++;
        
        if (!ProcuraLetra(str, letra)){ //retorna 1 quanto tem e 0 quando nao tem
            erros++;
        }else{
            acertos++;
            ArrumaString(copia, str, letra); //atualiza string apos o usuario acertar letra
        }

        if (strcmp(str, copia) == 0){
            ExibeForca(copia, &erros);
            printf("Voce ganhou!\n");
            break;
        }
    }
    sleep(1);

    printf("\nDeseja jogar novamente?\n"
           "1-sim 2-nao: ");
    scanf("%d", &op);

    if (op == 1)
        goto INICIO;

    fclose(fp);

    return 0;
}