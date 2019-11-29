#define BASE 10000
#define INF 999999999
#define ZERO 0.0000000001
#define OFFSET 48

#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int digit;
	struct node* next;
	struct node* prev;
}Node;

typedef struct poly
{
	float coeff;
	int deg;
	struct poly* next;
	struct poly* prev;
}Poly;

Node* createNode();

void clearList(Node** header);

void appendDigit(Node** header, int digit);

void prependDigit(Node** header, int digit);

void reverseList(Node** header);

void printNum(Node* header);

void readNum(Node** header);

Node* addLarge(Node* num1,Node* num2);

Node* mulLarge(Node* num1, Node* num2);

Node* evalLarge();

void printPoly(Poly* header);

void readPoly(Poly** header);

void prependPoly(Poly** header, int deg, float coeff);

void appendPoly(Poly** header, int deg, float coeff);

Poly* divPoly(Poly* dividend, Poly* divisor,Poly** quotient);

Poly* copyPoly(Poly* expr);

void delPolyNode(Poly** header);

void reversePoly(Poly** header);

Poly* createPoly();

void delPoly(Poly** header);

void insertPolyNode(Poly** header, int deg, float coeff);

float abs2(float num);