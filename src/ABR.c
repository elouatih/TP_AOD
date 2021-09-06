/*!\file ABR.c
 * \brief       To compute an optimal BST
 * \author      Hossam ELOUATI
 * \author      Youness HAMOUMI
 * \version     1.0
 * \date        21/10/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ABR.h"


static int *probability;
static int **roots;
static int **sum_probabilities;
static int **costs;
static int **BSTtree;
static int BSTroot;

/**
 * creating_all_tabs
 * \brief   Alloue les espaces memoires pour les tableaux
 * @param n Le nombre d'éléments du dictionnaire
 */
void creating_all_tabs(int n){
        // Tab with one dimension
        probability = malloc(n*sizeof(int));
        // Tabs with 2 dimensions
        roots = malloc(n*sizeof(int*));
        costs = malloc(n*sizeof(int*));
        sum_probabilities = malloc(n*sizeof(int*));
        BSTtree = malloc(n*sizeof(int*));
        for (int i=0; i<n; i++){
                roots[i] = malloc((n+1)*sizeof(int));
                costs[i] = malloc((n+1)*sizeof(int));
                sum_probabilities[i] = malloc((n+1)*sizeof(int));
                BSTtree[i] = malloc(2*sizeof(n));
        }
}

/**
 * initialization_sum_probabilities
 * \brief Initialise le tableau bidimensionnel comportant les sommes de probabilites
 * @param n Le nombre d'elements du dictionnaire
 */
void initialization_sum_probabilities(int n){
        for (int i=0; i<n; i++){
                for (int j=0; j<=i; j++){
                        sum_probabilities[i][j] = 0;
                }
                for (int j=i+1; j<=n; j++){
                        if (j==i+1){
                                sum_probabilities[i][j] = probability[j-1];
                        }else{
                                sum_probabilities[i][j] = sum_probabilities[i][j-1] + probability[j-1];
                        }
                }
        }
}

/**
 * initialization_roots
 * \brief Initialise le tableau bidimensionnel comportant les racines des sous-arbres optimaux
 * @param n Le nombre d'elements du dictionnaire
 */
void initialization_roots(int n){
        for (int i=0; i<n; i++){
                for (int j=0; j<=n; j++){
                        if (j==i+1){
                                roots[i][j] = i;
                        }else{
                                roots[i][j] = -1;
                        }
                }
        }
}

/**
 * initialization_costs
 * \brief Initialise le tableau bidimensionnel comportant les couts optimaux
 * @param n Le nombre d'elements du dictionnaire
 */
void initialization_costs(int n){
        for (int i=0; i<n; i++){
                for (int j=0; j<=i; j++){
                        costs[i][j] = 0;
                }
                for (int j=i+1; j<=n; j++){
                        if (j==i+1){
                                costs[i][j] = probability[j-1];
                        }else{
                                costs[i][j] = INT_MAX;
                        }
                }
        }
}

/**
 * initialization_BST
 * \brief Initialise le tableau representant l'ABR optimal
 * @param n Le nombre d'elements du dictionnaire
 */
void initialization_BST(int n){
        for (int i=0; i<n; i++){
                BSTtree[i][0] = -1;
                BSTtree[i][1] = -1;
        }
}

/**
 * sub_BST_optimal
 * \brief Remplit la case (i,j) du tableau couts et roots
 * @param i l'indice du premier element de la sequence
 * @param j l'indice du dernier element de la sequence + 1
 * @param n le nombre des elements du dictionnaire
 */
void sub_BST_optimal(int i, int j, int n){
        int t;
        int r_1 = roots[i][j-1];
        int r_2 = roots[i+1][j];
        for (int r=r_1; r<=r_2; r++){
                if (r==n-1){
                        t = costs[i][r];
                }else{
                        t = costs[i][r] + costs[r+1][j];
                }
                t += sum_probabilities[i][j];
                if (t < costs[i][j]){
                        roots[i][j] = r;
                        costs[i][j] = t;
                }
        }
}

/**
 * BST_optimal
 * \brief calcule le cout optimal et la racine de l'ABR optimal
 * @param n le nombre des elements du dictionnaire
 */
void BST_optimal(int n){
        for (int i=n-1; i>=0; i--){
                for (int j=i+2; j<=n; j++){
                        sub_BST_optimal(i,j, n);
                }
        }
}

/**
 * recover_subABR
 * \brief remplit le tableau BSTtree
 * @param start l'indice du premier element de la sequence
 * @param end l'indice du dernier element de la sequence + 1
 * @param n le nombre des elements du dictionnaire
 */
void recover_subABR(int start, int end, int n){
        int r = roots[start][end];
        if (start >= end){
                return;
        }
        if ( r == 0) {
                recover_subABR(r+1, end, n);
                BSTtree[r][1] = roots[r+1][end];
                return ;
        }
        if ( r == n-1){
                recover_subABR(start, r-1, n);
                BSTtree[r][0] = roots[start][r];
                return ;
        }
        recover_subABR(start, r, n);
        recover_subABR(r+1, end, n);
        BSTtree[r][0] = roots[start][r];
        BSTtree[r][1] = roots[r+1][end];
}

/**
 * recover_BST
 * \brief donne la totalite de BSTtree et donne la valeur de BSTroot
 * @param n le nombre des elements du dictionnaire
 */
void recover_BST(int n){
        recover_subABR(0, n, n);
        BSTroot = roots[0][n];
}

/**
 * extract_probabilities
 * \brief Extrait les probabilites d'un fichier et les place dans un tableau
 * @param file fichier comportant les probabilites
 * \return nombre d'élémnts du fichir comportant les probabilités
 */
int extract_probabilities(FILE *file){
        int value;
        int ind = 0;
        while ((fscanf(file, "%d", &value)) != EOF) {
                probability[ind] = value;
                ind++;
        }
        return ind;
};

/**
 * printBST
 * \brief Ecrit en sortie standard le tableau BSTtree
 * @param n le nombre des elements du dictionnaire
 */
void printBST(int n){
        fprintf(stdout, "static int BSTroot = %d;\n", BSTroot);
        fprintf(stdout, "static int BSTtree[%i][2] = {", n);
        for(int i=0; i<n; i++){
                fprintf(stdout, "\n{%d, %d}",BSTtree[i][0], BSTtree[i][1]);
                if(i < n-1) {
                        fprintf(stdout, ", ");
                }
        }
        fprintf(stdout, " };\n");
}

/**
 * computing_BST
 * \brief Extrait les probabilites et calcule l'ABR optimal correspondant
 * @param file
 * @param n
 */
void computing_BST(FILE *file, int n){
        creating_all_tabs(n);
        int number;
        number = extract_probabilities(file);
        if (number != n){
                printf("Number of elements is not correct. Please enter : %d \n", number);
                return;
        }
        //extract_probabilities(file, n);
        initialization_sum_probabilities(n);
        initialization_roots(n);
        initialization_costs(n);
        initialization_BST(n);
        BST_optimal(n);
        recover_BST(n);
        printBST(n);
}




