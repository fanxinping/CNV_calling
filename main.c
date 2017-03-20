#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "singly_linked_list_node.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define LEN 22

long chr_len[22] = {249250621, 243199373, 198022430, 191154276, 180915260,
                    171115067, 159138663, 146364022, 141213431, 135534747,
                    135006516, 133851895, 115169878, 107349540, 102531392,
                    90354753,  81195210,  78077248,  59128983,  63025520,
                    48129895,  51304566};

void ReadFromActualResult(char *filename, Node *X[]);
void ReadFromAccurityResult(char *filename, Node *Y[]);
void ReadFromControlFreeCResult(char *filename, Node *Y[]);
void ReadFromSequenzaResult(char *filename, Node *Y[]);

double SumOfArea(Node *X[], int length);
void InitializeGenRanges(Node *X[], int length);
void DeleteGenRanges(Node *X[], int length);
void CreateGenomicSegments(Node *X[]);
double SimilarityOfXtoY(Node *XonY[], int length);

void CalRecallAndPrecision(Node *X[], char *filename,
                           void (*fun)(char *filename, Node *Y[]),
                           double *recall, double *precision);
/*map genomic segments of Y to X,and the copynumber is abs(X.copynumber -
 * Y.copynumber) */
void YSegmentsMapOnX(Node *X[], Node *Y[], Node *XandY[], int length);

int main(int argc, char *argv[])
{
    Node *X[LEN];
    char purity[50];
    char file1[250];
    char file2[250];
    char file3[250];
    FILE *f_in;
    FILE *f_out;
    double recall, precision;
    char softwarename[3][20];
    void (*funcOrder[3])(char *, Node **);
    char *t = "Accurity       ControlFreeC   sequenza       ";
    int i, pos;

    InitializeGenRanges(X, LEN);
    ReadFromActualResult(argv[1], X);

    f_in = fopen(argv[2], "r");
    if (NULL == f_in)
    {
        printf("Cannot open the file: %s\n", argv[2]);
        exit(0);
    }
    fscanf(f_in, "%s%s%s", softwarename[0], softwarename[1], softwarename[2]);
    for (i = 0; i < 3; i++)
    {
        pos = (strstr(t, softwarename[i]) - t) / sizeof(char) / 15;
        switch (pos)
        {
            case 0:
                funcOrder[i] = ReadFromAccurityResult;
                break;
            case 1:
                funcOrder[i] = ReadFromControlFreeCResult;
                break;
            case 2:
                funcOrder[i] = ReadFromSequenzaResult;
                break;
            default:
                break;
        }
    }
    f_out = fopen(argv[3], "w");
    fprintf(f_out, "\t%s_r\t%s_p\t%s_r\t%s_p\t%s_r\t%s_p\n", softwarename[0],
            softwarename[0], softwarename[1], softwarename[1], softwarename[2],
            softwarename[2]);

    while (4 == fscanf(f_in, "%s%s%s%s", purity, file1, file2, file3))
    {
        fprintf(f_out, "%s\t", purity);
        CalRecallAndPrecision(X, file1, funcOrder[0], &recall, &precision);
        fprintf(f_out, "%.15f\t%.15f\t", recall, precision);
        CalRecallAndPrecision(X, file2, funcOrder[1], &recall, &precision);
        fprintf(f_out, "%.15f\t%.15f\t", recall, precision);
        CalRecallAndPrecision(X, file3, funcOrder[2], &recall, &precision);
        fprintf(f_out, "%.15f\t%.15f\n", recall, precision);
    }
    fclose(f_in);
    fclose(f_out);
    DeleteGenRanges(X, LEN);
    return 0;
}

void ReadFromActualResult(char *filename, Node *X[])
{
    FILE *f_in;
    char temp[100];
    char chr[10];
    long start;
    long end;
    float copynumber;

    f_in = fopen(filename, "r");
    if (NULL == f_in)
    {
        printf("Cannot open the file: %s\n", filename);
        exit(0);
    }
    fgets(temp, 100, f_in);  // skip header line
    while (4 == fscanf(f_in, "%s%ld%ld%f%*s", chr, &start, &end, &copynumber))
    {
        Item item = {start, end, copynumber};
        sll_insert(&X[atoi(chr + 3) - 1], item);
    }
    fclose(f_in);
}

void ReadFromControlFreeCResult(char *filename, Node *Y[])
{
    FILE *f_in;
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
        if (strcmp("X", chr) == 0 || strcmp("Y", chr) == 0) continue;
        Item item = {atol(start), atol(end), atof(copynumber)};
        sll_insert(&Y[atoi(chr) - 1], item);
    }
    fclose(f_in);
}

void ReadFromAccurityResult(char *filename, Node *Y[])
{
    FILE *f_in;
    char chr[10];
    char start[50];
    char end[50];
    char copynumber[50];
    char temp[500];
    f_in = fopen(filename, "r");
    if (NULL == f_in)
    {
        printf("Cannot open the file: %s\n", filename);
        exit(0);
    }
    fgets(temp, 500, f_in);  // skip header
    while (4 == fscanf(f_in, "%s%*s%*s%s%*s%*s%*s%*s%*s%*s%s%s", chr,
                       copynumber, start, end))
    {
        if (atoi(start) == 0 || atoi(end) == 0 || atof(copynumber) == 2)
            continue;
        Item item = {atol(start), atol(end), atof(copynumber)};
        sll_insert(&Y[atoi(chr) - 1], item);
    }
    fclose(f_in);
}

void ReadFromSequenzaResult(char *filename, Node *Y[])
{
    FILE *f_in;
    char chr[10];
    char start[50];
    char end[50];
    char copynumber[50];
    char temp[500];
    f_in = fopen(filename, "r");
    if (NULL == f_in)
    {
        printf("Cannot open the file: %s\n", filename);
        exit(0);
    }
    fgets(temp, 500, f_in);  // skip header line
    while (4 == fscanf(f_in, "%s%s%s%*s%*s%*s%*s%*s%*s%s%*s%*s%*s", chr, start,
                       end, copynumber))
    {
        if (atoi(copynumber) == 2) continue;
        Item item = {atol(start), atol(end), atof(copynumber)};
        sll_insert(&Y[atoi(chr + 4) - 1], item);
    }
    fclose(f_in);
}

double SumOfArea(Node *X[], int length)
{
    int i;
    double sum = 0;
    Node *current;
    for (i = 0; i < length; i++)
    {
        if (NULL == X[i]) continue;
        current = X[i];
        while (NULL != current)
        {
            sum += current->item.end - current->item.start + 1;
            current = current->link;
        }
    }
    return sum;
}

void InitializeGenRanges(Node *X[], int length)
{
    int i;
    for (i = 0; i < length; i++) X[i] = NULL;
}

void DeleteGenRanges(Node *X[], int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        sll_delete(&X[i]);
    }
}

void CreateGenomicSegments(Node *X[])
{
    int i;
    long start, end;
    Node *current;

    for (i = 0; i < LEN; i++)
    {
        start = 1;
        end = chr_len[i];
        current = X[i];
        while (current != NULL)
        {
            if (start < current->item.start)
            {
                Item item = {start, current->item.start - 1, 2};
                sll_insert(&X[i], item);
            }
            start = current->item.end + 1;
            current = current->link;
        }
        if (start <= end)
        {
            Item item = {start, end, 2};
            sll_insert(&X[i], item);
        }
    }
}

void YSegmentsMapOnX(Node *X[], Node *Y[], Node *XandY[], int length)
{
    int i;
    long start, end;
    Node *X_current;
    Node *Y_current;

    for (i = 0; i < length; i++)
    {
        if (NULL == X[i] || NULL == Y[i]) continue;
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
                    Item item = {start, end, fabs(X_current->item.copynumber -
                                                  Y_current->item.copynumber)};
                    sll_insert(&XandY[i], item);
                }
                X_current = X_current->link;
            }
            Y_current = Y_current->link;
        }
    }
}

double SimilarityOfXtoY(Node *XonY[], int length)
{
    int i;
    double similarity = 0;
    Node *current;
    for (i = 0; i < length; i++)
    {
        if (NULL == XonY[i]) continue;
        current = XonY[i];
        while (NULL != current)
        {
            similarity += (current->item.end - current->item.start + 1) *
                          exp(-(current->item.copynumber));
            current = current->link;
        }
    }
    return similarity;
}

void CalRecallAndPrecision(Node *X[], char *filename,
                           void (*fun)(char *filename, Node *Y[]),
                           double *recall, double *precision)
{
    Node *Y[LEN];
    Node *XonY[LEN];
    double similarity;

    InitializeGenRanges(Y, LEN);
    InitializeGenRanges(XonY, LEN);
    fun(filename, Y);
    YSegmentsMapOnX(X, Y, XonY, LEN);
    similarity = SimilarityOfXtoY(XonY, LEN);
    *recall = similarity / SumOfArea(X, LEN);
    *precision = similarity / SumOfArea(Y, LEN);
    DeleteGenRanges(Y, LEN);
    DeleteGenRanges(XonY, LEN);
}
