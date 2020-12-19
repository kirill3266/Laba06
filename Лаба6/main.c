#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* value;
	int count;
	size_t* prev;
	size_t* next;
} node_t;

typedef struct {
	node_t* head;
	int size;
} list_t;

int strlength(char* str) {
	int i = 0;
	for (i = 0; str[i] != 0; i++);
	return i;
}

void init(list_t* l) {
	l->head = NULL;
	l->size = 0;
}

void push_back(list_t* l, char* val, int chislo) {
	node_t* n, * cur;
	n = (node_t*)malloc(sizeof(node_t));
	n->value = val;
	n->count = chislo;
	n->next = NULL;
	if (l->head == NULL) {
		n->prev = NULL;
		l->head = n;
	}
	else {
		cur = l->head;
		while (cur->next != NULL)
			cur = cur->next;
		cur->next = n;
		n->prev = cur;
	}
	l->size++;
	//sort
	node_t* cur1;
	node_t* prev;
	char* tmp;
	for (int i = 0; i < l->size - 1; i++) {
		cur1 = l->head;
		prev = NULL;

		while (cur1 != NULL) {
			prev = cur1;
			cur1 = cur1->next;
			if (cur1 != NULL) {
				if (strlength(prev->value) > strlength(cur1->value) || (strcmp(prev->value, cur1->value) > 0) && strlength(prev->value) == strlength(cur1->value)) {
					tmp = prev->value;
					prev->value = cur1->value;
					cur1->value = tmp;
				}
			}
		}
	}
}

void destroy(list_t* l) {
	node_t* cur = l->head;
	node_t* prev = NULL;
	while (cur != NULL) {
		prev = cur;
		cur = cur->next;
		free(prev);
	}
}

char* finput(FILE* fin, list_t* l, char* word, int* chislo) {
	char ch = 0;
	int v, n;
	char* str;
	do {
		v = 0, n = 5;
		str = (char*)malloc(n * sizeof(char));
		do {
			fscanf(fin, "%c", &ch);
			str[v] = ch;
			v++;
			if (v == n - 2) {
				n *= 2;
				str = (char*)realloc(str, n * sizeof(char));
				if (!str) {
					printf("Allocation error!\n");
					destroy(l);
					return 0;
				}
			}
		} while (ch != ' ' && !feof(fin) && ch != '\t' && ch != '\n');
		if (ch == '\n' && word == NULL) {
			str[v - 1] = 0;
			word = str;
		}
		else {
			str[v - 1] = 0;
			if (strlength(str) == 0) {
				free(str);
			}
			else {
				push_back(l, str, *chislo);
				(*chislo)++;
			}
		}
	} while (!feof(fin));
	return word;
}

void print(list_t* l) {
	node_t* cur = l->head;
	printf("Searching string: ");
	while (cur != NULL) {
		if (cur->next != NULL) {
			printf("%s<", cur->value);
			cur = cur->next;
		}
		else {
			printf("%s", cur->value);
			cur = cur->next;
		}
	}
	printf("\n");
	cur = l->head;
	printf("Searching count: ");
	while (cur != NULL) {
		if (cur->next != NULL) {
			printf("%d<", cur->count);
			cur = cur->next;
		}
		else {
			printf("%d", cur->count);
			cur = cur->next;
		}
	}
	printf("\n");
}

int strcmp(const char* s1, const char* s2)
{
	for (; *s1 == *s2; ++s1, ++s2)
		if (*s2 == '\0')
			return 0;
	return (unsigned char)*s1 - (unsigned char)*s2;
}

int binarysearch(list_t* l, char* word) {
	node_t* cur;
	int low, high, middle;
	low = 0;
	high = (l->size) - 1;
	while (low <= high) {
		cur = l->head;
		middle = (low + high) / 2;
		while (cur->next != NULL && cur->count != middle)
			cur = cur->next;
		if ((strcmp(word, cur->value) < 0 && strlength(word) == strlength(cur->value)) || strlength(word) < strlength(cur->value))
			high = middle - 1;
		else if ((strcmp(word, cur->value) > 0 && strlength(word) == strlength(cur->value)) || strlength(word) > strlength(cur->value))
			low = middle + 1;
		else return middle;
	}
	return -1;
}

int main() {
	FILE* fin;

	if ((fin = fopen("Fin.txt", "r")) == NULL)
	{
		printf("Error\n");
		return 0;
	}
	else printf("Success\n");

	list_t l;
	init(&l);
	char* word = NULL;
	int chislo = 0;
	int result = 0;

	word = finput(fin, &l, word, &chislo);

	char* ch;
	int v, n;
	char* str;
	printf("Vvodite slovo ili stroku.\\n to end:\n");
	do {
		ch = 0;
		v = 0, n = 5;
		str = (char*)malloc(n * sizeof(char));
		do {
			scanf("%c", &ch);
			str[v] = ch;
			v++;
			if (v == n - 2) {
				n *= 2;
				str = (char*)realloc(str, n * sizeof(char));
				if (!str) {
					printf("Allocation error!\n");
					destroy(&l);
					return 0;
				}
			}
		} while (ch != '\n' && ch != ' ' && ch != '\t');
		if (*str != '\n') {
			str[v - 1] = 0;
			if (strlength(str) != 0) {
				push_back(&l, str, chislo);
				chislo++;
			}
		}
	} while (*str != '\n');

	if (word == NULL)
	{
		printf("No searching word!\n");
		free(word);
		return 0;
	}
	else printf("Looking word: %s\n", word);

	print(&l);

	result = binarysearch(&l, word);

	if(result !=-1) printf("Index of searching word: %d\n", result);
	else {
		printf("Word not found!\n");
		destroy(&l);
		return -1;
	}


	destroy(&l);

	return 0;
}