/*
 * CP 264 Huffman Coding
 *
 * This file contains the main program,the structure of the Huffman node
 * and user interface for running your Huffman Encoder/Decoder.
 *
 * This code is completed by Aozhou Hao, Xiaohan Li, Baizhou Wang, Wei Yuen Edmund Ru, Brayden Mckay
 *
 * The main function of this code is to encoded and decoded the text file using Huffman structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "project.h"

int main(void) {
	setbuf(stdout, NULL);
	node *root = (node*) malloc(sizeof(node));

	char message[30];
	printf("Enter the filename you want to encode: ");
	scanf("%s", message);
	//The result of encode function will be in the encoded.txt file
	//It also returns the root of the tree to decode the binary bits
	root = encode(message, "encoded.txt");

	//Use the root of the tree to decode the binary bits from encoded.txt;
	//Then write it to the file decoded_message.txt
	decode(root, "encoded.txt", "decoded_message.txt");
	return 0;
}

//Read the txt file from *message, then write the encoded
//binary bits into the file *encoded, return root of the tree
node* encode(char *message, char *encoded) {

	//Initialize an array of nodes, set all elements in it to NULL
	node *list[MAX_SIZE];
	for (int i = 0; i < MAX_SIZE; i++) {
		list[i] = NULL;
	}

	char file[TXT_SIZE] = "";
	int length_of_file = read_file_to_string(message, file);
	//string file now contains all characters in the file named message

	//Store the number of occurrence for each character in list,
	//length is the length of the list
	int length = read_file_to_nodes(message, list);
	//print out the frequency of each character in the file
	printf("Frequency of each character in the file:\n");
	for (int i = 0; i < length; i++) {
		if (list[i]->c == '\n')
			printf("\\n: %d\n", list[i]->n);
		else
			printf("%c : %d\n", list[i]->c, list[i]->n);
	}

	node *root = (node*) malloc(sizeof(node));
	root = read_ary_to_tree(list, length);
	//Store the list in a tree

	int ary[ASCII_length], index = 0;
	node *l[length];
	for (int j = 0; j < length; j++) {
		l[j] = NULL;
	}
	ary_binary_code(root, ary, index, l);
	//l contains list of nodes that with binary bits for each character

	printf("Binary bits for each character:\n");
	for (int j = 0; j < length; j++) {
		if (l[j]->c == '\n') {
			printf("\\n: %s\n", l[j]->code);
		} else
			printf("%c: %s\n", l[j]->c, l[j]->code);
	}

	//write the binary bits to file encoded
	FILE *f = fopen(encoded, "w");
	for (int i = 0; i < length_of_file; i++) {
		for (int j = 0; j < length; j++) {
			if (file[i] == l[j]->c) {
				if (l[j]->c == '\n') {
					fprintf(f, "\n");
				}
				fprintf(f, "%s", l[j]->code);
			}
		}
	}
	fclose(f);

	return root;
}
void decode(node *root, char *encoded, char *message) {
	char string[TXT_SIZE] = "";
	//First, we need to get encoded message from file.
	int length = read_file_to_string(encoded, string);

	//Then we use encoded message to traverse the tree.
	search_in_tree(length, root, string, message);

	return;
}

//search_in_tree is used to traverse the tree and return symbol according to encoded message,
//and write the symbol into decoded file.
void search_in_tree(int length, node *root, char string[], char *message) {
	node *temp = (node*) malloc(sizeof(node));
	temp = root;
	char decoded;
	FILE *file = fopen(message, "w");
	int i = 0;
	while (i < length) {
		char action = string[i];	//Read one character in string.
		if (action == '0') {
			temp = temp->left;//If the character is 0, we need to go left of current node.

			if (isLeaf(temp)) {	//If we the node is leaf, then we've already found the symbol.
				decoded = temp->c;
				fputc(decoded, file);	//Write the symbol into decoded file.

				temp = root;//We need to back to root again to find the next symbol.
			}
		} else if (action == '1') {	//If the character is 1, we need to go right of current node.
			temp = temp->right;
			if (isLeaf(temp)) {
				decoded = temp->c;
				fputc(decoded, file);

				temp = root;//We need to back to root again to find the next symbol.
			}
		}
		i++;
	}

	return;
}

//read from file *encoded to string[]
int read_file_to_string(char *encoded, char string[]) {
	char c;
	int length = 0;
	FILE *file = fopen(encoded, "r");
	c = fgetc(file);
	while (c != EOF) {
		if (c != '\0') {
			strncat(string, &c, 1);	//Add the character we get to string.
			length++;	//Each time we add a character to string, its length + 1
		}
		c = fgetc(file);
	}
	fclose(file);
	return length;
}

//Form a list of nodes into a tree structure
node* read_ary_to_tree(node *list[], int length) {
	int i = 0;
	while (length != 1) {

		node *parent = (node*) malloc(sizeof(node));
		parent->left = list[0];
		parent->right = list[1];
		parent->n = list[0]->n + list[1]->n;
		parent->c = '*';
		for (i = 0; i < length - 1; i++)
			list[i] = list[i + 1];
		list[0] = parent;
		length--;
		node_sort(list, length);

	}
	return list[0];
}

//Get the binary bits of each character in the tree, then store them in
//the list of nodes l
void ary_binary_code(node *root, int array[], int index, node *l[]) {
	if (root->left) {
		array[index] = 0;
		ary_binary_code(root->left, array, index + 1, l);
	}
	if (root->right) {
		array[index] = 1;
		ary_binary_code(root->right, array, index + 1, l);
	}
	if (isLeaf(root)) {
		node *n = (node*) malloc(sizeof(node));
		n->c = root->c;
		convert_int_to_char(array, index, n->code);
		append_to_list(l, n);
	}
	return;
}

//append the node n in the list of nodes l[]
void append_to_list(node *l[], node *n) {
	int length = get_length(l);

	l[length] = n;
}

//get the length of a list of nodes
int get_length(node *l[]) {
	int i = 0;
	while (l[i] != NULL) {
		i++;
	}
	return i;
}

//convert an array of integers arr[] into an array of char c[]
void convert_int_to_char(int arr[], int n, char c[]) {
	int i, k = 0;
	for (i = 0; i < n; ++i)
		sprintf(&c[k++], "%d", arr[i]);

	return;
}

//check if a node is a leaf or not
int isLeaf(node *root) {

	return root->c != '*';
}

/*read_file_to_nodes read the file named message, then store each character
 *and it's number occurrence into a node, then sort them from low number of
 *occurrence to high number of occurrence.
 Return: the length of the list
 */
int read_file_to_nodes(char *message, node *list[]) {
	char c, string[MAX_SIZE] = "";

	FILE *file = fopen(message, "r");
	c = fgetc(file);

	while (c != EOF) {
		strncat(string, &c, 1);
		c = fgetc(file);
	}

	int l = strlen(string), l2 = 0, count, checker = FALSE, index = 0;

	char collection[MAX_SIZE];

	for (int i = 0; i < l; i++) {
		count = 0;
		checker = FALSE;
		//check if the character is in the collection
		for (int j = 0; j < l2; j++) {
			if (string[i] == collection[j])
				checker = TRUE;
		}
		if (checker == FALSE) {
			collection[l2++] = string[i];
			for (int k = 0; k < l; k++) {
				if (string[i] == string[k])
					count++;
			}
			node_insert(list, index++, string[i], count);
		}
	}

	node_sort(list, index);
	fclose(file);
	return index;
}

//Insert a node in the list of nodes at a given index
void node_insert(node *list[], int index, char c, int count) {
	node *node1 = (node*) malloc(sizeof(node));
	node1->left = NULL;
	node1->right = NULL;
	node1->c = c;
	node1->n = count;
	list[index] = node1;
	return;
}

//Sort the list of nodes in descending order
void node_sort(node *list[], int length) {
	for (int i = 0; i < length - 1; i++) {
		for (int j = i; j < length; j++) {
			if (list[i]->n > list[j]->n) {
				node_switch(list, i, j);
			}
		}
	}
}

//Switch the position of 2 nodes in a list of nodes
void node_switch(node *list[], int i, int j) {
	node *n1 = (node*) malloc(sizeof(node));
	n1 = list[i];
	list[i] = list[j];
	list[j] = n1;
	return;
}

