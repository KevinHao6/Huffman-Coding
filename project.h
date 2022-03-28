/*
 -------------------------------------
 File:    project.h
 Project: p
 file description
 -------------------------------------
 Author:  your name here
 ID:      your ID here
 Email:   your Laurier email here
 Version  2020-11-28
 -------------------------------------
 */
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 50000
#define TXT_SIZE 500000
#define ASCII_length 127
//
typedef struct node {
	struct node *left;
	struct node *right;
	char c; 		//to store characters from the file
	char code[MAX_SIZE];
	int n; 			//number of occurences

} node;

void ary_binary_code(node *root, int array[], int index, node *l[]);
void node_insert(node *list[], int index, char c, int count);
int read_file_to_nodes(char *message, node *list[]);
node* encode(char *message, char *encoded);
void node_sort(node *list[], int length);
void node_switch(node *list[], int i, int j);
node* read_ary_to_tree(node *list[], int length);
int isLeaf(node *root);
void convert_int_to_char(int arr[], int n, char c[]);
int read_file_to_string(char *encoded, char string[]);
void append_to_list(node *l[], node *n);
int get_length(node *l[]);
void decode(node *root, char *encoded, char *message);
void search_in_tree(int length, node *root, char string[], char *message);
