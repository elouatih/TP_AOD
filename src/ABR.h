#ifndef _ABR_H
#define _ABR_H

void creating_all_tabs(int n);
void initialization_sum_probabilities(int n);
void initialization_roots(int n);
void initialization_costs(int n);
void initialization_BST(int n);
void sub_BST_optimal(int i, int j, int n);
void BST_optimal(int n);
void recover_subABR(int start, int end, int n);
void recover_BST(int n);
int extract_probabilities(FILE *file);
void printBST(int n);
void computing_BST(FILE *file, int n);

#endif