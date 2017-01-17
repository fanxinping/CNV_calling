#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "singly_linked_list_node.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define LEN 22

void ReadFromActualResult(char * filename,Node * X[]);
void ReadFromAccurityResult(char * filename, Node * Y[]);
void ReadFromControlFreeCResult(char * filename,Node * Y[]);
void ReadFromSequenzaResult(char * filename, Node * Y[]);

double SumOfArea(Node * X[],int length);
void IntersectGenRanges(Node * X[], Node * Y[], Node * XandY[],int length);
void InitializeGenRanges(Node * X[], int length);
void DeleteGenRanges(Node * X[], int length);
double JaccardIndex(Node * X[], char * filename, void(*fun)(char * filename, Node * Y[]));

int main(int argc,char * argv[])
{
	Node * X[LEN];
	char purity[50];
	char file1[250];
	char file2[250];
	char file3[250];
	FILE * f_in;
	FILE * f_out;
	double jaccard_index;
	char softwarename[3][20];
	void(*funcOrder[3])(char *,Node **);
	char * t = "Accurity       ControlFreeC   sequenza       ";
	int i,pos;

	InitializeGenRanges(X, LEN);
	ReadFromActualResult(argv[1], X);

	f_in = fopen(argv[2],"r");
	if (NULL == f_in)
	{
		printf("Cannot open the file: %s\n",argv[2]);
		exit(0);
	}
	fscanf(f_in,"%s%s%s",softwarename[0],softwarename[1],softwarename[2]);
	for(i = 0;i < 3; i++)
	{
		pos = (strstr(t,softwarename[i]) - t)/sizeof(char)/15;
		switch (pos)
		{
			case 0:funcOrder[i]=ReadFromAccurityResult;
				   break;
			case 1:funcOrder[i]=ReadFromControlFreeCResult;
			       break;
			case 2:funcOrder[i]=ReadFromSequenzaResult;
				   break;
			default:break;
		}
	}
	f_out = fopen(argv[3], "w");
	fprintf(f_out, "\t%s\t%s\t%s\n",softwarename[0],softwarename[1],softwarename[2]);

	while (4 == fscanf(f_in, "%s%s%s%s", purity, file1, file2, file3))
	{
		fprintf(f_out, "%s\t", purity);
		jaccard_index = JaccardIndex(X, file1, funcOrder[0]);
		fprintf(f_out, "%.15f\t", jaccard_index);
		jaccard_index = JaccardIndex(X, file2, funcOrder[1]);
		fprintf(f_out, "%.15f\t", jaccard_index);
		jaccard_index = JaccardIndex(X, file3, funcOrder[2]);
		fprintf(f_out, "%.15f\n", jaccard_index);
	}
	fclose(f_in);
	fclose(f_out);
	DeleteGenRanges(X, LEN);
	return 0;
}

void ReadFromActualResult(char * filename,Node * X[])
{
	FILE * f_in;
	char temp[100];
	char chr[10];
	long start;
	long end;
	int copynumber;

	f_in = fopen(filename, "r");
	if (NULL == f_in)
	{
		printf("Cannot open the file: %s\n", filename);
		exit(0);
	}
	fgets(temp, 100, f_in);   //skip header line
	while (4 == fscanf(f_in, "%s%ld%ld%d", chr, &start, &end, &copynumber))
	{
		Item item = { start, end, copynumber+1 };
		sll_insert(&X[atoi(chr + 3)-1], item);

	}
	fclose(f_in);
}

void ReadFromControlFreeCResult(char * filename, Node *Y[])
{
	FILE * f_in;
	char chr[10];
	char start[50];
	char end[50];
	char copynumber[50];
	char state[10];
	f_in = fopen(filename, "r");
	if (NULL == f_in)
	{
		printf("Cannot open the file: %s\n", filename);
		exit(0);
	}
	while (5 == fscanf(f_in, "%s%s%s%s%s", chr, start, end, copynumber, state))
	{
		if (strcmp("chrX", chr) == 0 || strcmp("chrY", chr) == 0)
			continue;
		Item item = { atol(start), atol(end), atof(copynumber) };
		sll_insert(&Y[atoi(chr) - 1], item);
	}
	fclose(f_in);
}

void ReadFromAccurityResult(char * filename, Node * Y[])
{
	FILE * f_in;
	char chr[10];
	char start[50];
	char end[50];
	char copynumber[50];
//	char temp[500];
	f_in = fopen(filename, "r");
	if (NULL == f_in)
	{
		printf("Cannot open the file: %s\n", filename);
		exit(0);
	}
//	fgets(temp,500,f_in);
	while (4 == fscanf(f_in, "%s%*s%*s%s%*s%*s%*s%*s%*s%*s%s%s", chr, copynumber, start, end))
	{
		if (atoi(start) == 0 || atoi(end) == 0 || atof(copynumber) == 2)
			continue;
		Item item = { atol(start), atol(end), atof(copynumber) };
		sll_insert(&Y[atoi(chr) - 1], item);
	}
	fclose(f_in);
}

void ReadFromSequenzaResult(char * filename, Node * Y[])
{
	FILE * f_in;
	char chr[10];
	char start[50];
	char end[50];
	char copynumber[50];
	char temp[500];
	f_in = fopen(filename,"r");
	if (NULL == f_in)
	{
		printf("Cannot open the file: %s\n",filename);
		exit(0);
	}
	fgets(temp,500,f_in);   //skip header line
	while (4 == fscanf(f_in,"%s%s%s%*s%*s%*s%*s%*s%*s%s%*s%*s%*s",chr,start,end,copynumber))
	{
		if (atoi(copynumber) == 2)
			continue;
		Item item = {atol(start),atol(end),atof(copynumber)};
		sll_insert(&Y[atoi(chr+4)-1],item);
	}
	fclose(f_in);
}
double SumOfArea(Node * X[], int length)
{
	int i;
	double sum = 0;
	Node * current;
	for (i = 0; i < length; i++)
	{
		if (NULL == X[i])
			continue;
		current = X[i];
		while (NULL != current)
		{
			sum += (current->item.end - current->item.start + 1) * current->item.copynumber;
			current = current->link;
		}
	}
	return sum;
}

void IntersectGenRanges(Node * X[], Node * Y[], Node * XandY[], int length)
{
	int i;
	long start, end;
	double copynumber;
	Node * X_current;
	Node * Y_current;

	for (i = 0; i < length; i++)
	{
		if (NULL == X[i] || NULL == Y[i])
			continue;
		Y_current = Y[i];
		while (NULL != Y_current)
		{
			X_current = X[i];
			while (NULL != X_current)
			{
				start = max(Y_current->item.start, X_current->item.start);
				end = min(Y_current->item.end, X_current->item.end);
				if (start < end + 1)
				{
					Item item = { start, end, min(X_current->item.copynumber, Y_current->item.copynumber) };
					sll_insert(&XandY[i], item);
				}
				X_current = X_current->link;
			}
			Y_current = Y_current->link;
		}
	}
}

void InitializeGenRanges(Node * X[], int length)
{
	int i;
	for (i = 0; i < length; i++)
		X[i] = NULL;
}

void DeleteGenRanges(Node * X[], int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		sll_delete(&X[i]);
	}
}

double JaccardIndex(Node * X[], char * filename, void(*fun)(char * filename, Node * Y[]))
{
	double intersectarea, unionarea, jaccardIndex;
	Node * XandY[LEN];
	Node * Y[LEN];
	int i;
	InitializeGenRanges(Y, LEN);
	InitializeGenRanges(XandY, LEN);
	fun(filename, Y);
	IntersectGenRanges(X, Y, XandY, LEN);
	/*for (i = 0; i < LEN; i++)
	{
		printf("chr%d\n", i + 1);
		sll_shown(&XandY[i]);
	}*/
	intersectarea = SumOfArea(XandY, LEN);
	//printf("intersectarea: %.15f\n", intersectarea);
	unionarea = SumOfArea(X, LEN) + SumOfArea(Y, LEN) - intersectarea;
	/*printf("X: %.15f\n", SumOfArea(X, LEN));
	printf("Y: %.15f\n", SumOfArea(Y, LEN));*/
	jaccardIndex = intersectarea / unionarea;
	DeleteGenRanges(Y, LEN);
	DeleteGenRanges(XandY, LEN);
	return jaccardIndex;
}
