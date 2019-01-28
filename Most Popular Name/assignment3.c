/*
 * assignment3.c
 *
 *  Created on: Apr 15, 2018
 *      Author: Sally Ho
 */

#include <string.h>
#include <stdio.h>
#define TOTAL 1000
#define NAME_LENGTH 20
#define RANK_LENGTH 10
#define TOTAL_NAMES 366
/*
 * PROTOTYPE: Function that will read all 10 input files.
 */
void scanTen(FILE*[], char[][NAME_LENGTH], int[][RANK_LENGTH], int*);
/*
 * PROTOTYPE: Function that will read in 1 input file.
 */
void scanIndFile(FILE*, char[][NAME_LENGTH],
		int[][RANK_LENGTH], int, int*);
/*
 * PROTOTYPE: Function that will sort out names and stuff.
 */
void processName(FILE*, char[][NAME_LENGTH], int[][RANK_LENGTH],
		int, char[], int*, int*, int);
/*
 * PROTOTYPE: Function that will sort the arrays
 */
void mergeSortString(char[][20], int[][RANK_LENGTH], int);
/*
 * PROTOTYPE: Function that helps merge sort with sorting.
 */
void merge(char[][20], int[][RANK_LENGTH], char[][20], char[][20],
		   int[][10], int[][10], int, int, int);
/*
 * PROTOTYPE: Function that will create the output file.
 */
void printToCSV(char[][NAME_LENGTH], int[][RANK_LENGTH], FILE*);
/*
 * Main function to execute everything.
 */
int main(void) {
	FILE *file1920 = fopen("yob1920.txt", "r");
	FILE *file1930 = fopen("yob1930.txt", "r");
	FILE *file1940 = fopen("yob1940.txt", "r");
	FILE *file1950 = fopen("yob1950.txt", "r");
	FILE *file1960 = fopen("yob1960.txt", "r");
	FILE *file1970 = fopen("yob1970.txt", "r");
	FILE *file1980 = fopen("yob1980.txt", "r");
	FILE *file1990 = fopen("yob1990.txt", "r");
	FILE *file2000 = fopen("yob2000.txt", "r");
	FILE *file2010 = fopen("yob2010.txt", "r");
	FILE *fileCSV = fopen("summary.csv", "w");
	char names[TOTAL_NAMES + 2][NAME_LENGTH] = {'\0'};//100 female names
	int rank[TOTAL][RANK_LENGTH]; //1000 total ranks across 10 years.
	FILE *arrayFile[RANK_LENGTH] = {file1920, file1930, file1940,
			file1950,file1960, file1970, file1980, file1990,
			file2000, file2010};
	int nameCount = 0;
	int i = 0;
	scanTen(arrayFile, names, rank, &nameCount);
	mergeSortString(names, rank, nameCount);
	printToCSV(names, rank, fileCSV);
	for(i = 0; i < RANK_LENGTH; i++) {
		fclose(arrayFile[i]);
	}
	fclose(fileCSV);
	return 0;
}
/*
 * Function that will read all 10 input files.
 */
void scanTen(FILE *file[], char name[][NAME_LENGTH],
		int rank[][RANK_LENGTH], int *nameCount) {
	int i = 0;
	for(i = 0; i < RANK_LENGTH; i++) {
		scanIndFile(*(file + i), name, rank, i, nameCount);
	}
//	printf("Total names processed: %d\n", *nameCount);
}

/*
 * This function iterates through the top 100 female names in each file
 * add name and update rank
 */
void scanIndFile(FILE *file, char nameArray[][NAME_LENGTH],
		int rank[][RANK_LENGTH], int index, int *nameCount) {
	int i = 0;
	int hasName = 1;
	//iterates through the 100 names in each file.
	for(i = 0; i < 100; i++) { //when i is 0, rank is 1.
		char names[NAME_LENGTH];
		fscanf(file, " %[^,],%*c,%*d", names);
		processName(file, nameArray, rank, index, names,
				nameCount, &hasName, i);
		hasName = 1;
	}
}

/*
 * This function processes each individual line.
 * TOTAL: total number of ranks
 * NAME_LENGTH: 20
 * RANK_LENGTH: 10
 */
void processName(FILE *file, char nameArray[][NAME_LENGTH],
		int rank[][RANK_LENGTH], int index, char names[],
		int *nameCount, int *hasName, int rankIndex) {
	int i = 0;
	int j = 0;
	for(i = 0; i < TOTAL && nameArray[i][0] != '\0'; i++) {
		if(strcmp(names, nameArray[i]) == 0) { //if name is same
			*hasName = 0; //update to this
			rank[i][index] = rankIndex + 1; //update rank.
		}
	}
	if(*hasName == 1) { //if it doesn't have name
		for(j = 0; j < 20; j++) { //print name to compare array
			nameArray[i][j] = names[j];
		}
		rank[i][index] = rankIndex + 1; //update rank.
		*nameCount = *nameCount + 1; // update the count.
		//don't really need the count, just for reference
	}

}
/*
 * merges back into the name and rank arrays.
 */
void merge(char name[][NAME_LENGTH], int rank[][RANK_LENGTH],
		char left[][NAME_LENGTH], char right[][NAME_LENGTH],
		int leftRank[][RANK_LENGTH], int rightRank[][RANK_LENGTH],
		   int length, int leftLen, int rightLen) {
	int i = 0;
	int i1 = 0;
	int i2 = 0;
	int j = 0;
	for(i = 0; i < length; i++) {
		if(i2 >= rightLen
				|| (i1 < leftLen && strcmp(left[i1], right[i2]) <= 0)) {
			strcpy(name[i], left[i1]);
			for (j = 0; j < RANK_LENGTH; j++) {
				rank[i][j] = leftRank[i1][j];
			}
			i1++;
		} else {
			strcpy(name[i], right[i2]);
			for (j = 0; j < RANK_LENGTH; j++) {
				rank[i][j] = rightRank[i2][j];
			}
			i2++;
		}
	}
}
/*
 * Separates and sorts the merge to begin merging.
 */
void mergeSortString(char name[][NAME_LENGTH],
		int rank[][RANK_LENGTH], int nameCount) {
	int i1, i2, j;
	//left and right array to contain data
	char left[nameCount/2][NAME_LENGTH];
	int leftRank[nameCount/2][RANK_LENGTH];
	char right[nameCount/2 + 1][NAME_LENGTH];
	int rightRank[nameCount/2 + 1][RANK_LENGTH];
	int leftCount = nameCount / 2;
	int rightCount = nameCount - leftCount;
	if(nameCount >= 2) {
		for(i1 = 0; i1 < leftCount; i1++) {
			strcpy(left[i1], name[i1]);
			for (j = 0; j < RANK_LENGTH; j++) {
				leftRank[i1][j] = rank[i1][j];
			}
		}
		for(i2 = 0; i2 < rightCount; i2++, i1++) {
			strcpy(right[i2], name[i1]);
			for (j = 0; j < RANK_LENGTH; j++) {
				rightRank[i2][j] = rank[i1][j];
			}
		}
		mergeSortString(left, leftRank, leftCount);
		mergeSortString(right, rightRank, rightCount);

		merge(name, rank, left, right, leftRank, rightRank,
				nameCount, leftCount, rightCount);
	}
}
/*
 * Prints the output to a CSV file.
 */
void printToCSV(char name[][NAME_LENGTH], int rank[][RANK_LENGTH],
		FILE *fileCSV) {
	int nameRow = 0;
	int rankColumn = 0;
	int i = 0;
	char *header[11] = {"Name", "1920", "1930", "1940", "1950", "1960",
			"1970", "1980", "1990", "2000", "2010"};
	//okay fencepost paradigm. for header
	fprintf(fileCSV, "%s,", header[0]);
	for(i = 1; i < 11; i++) {
		fprintf(fileCSV, "%s,", header[i]);
	}
	fprintf(fileCSV, "\n");

	for(nameRow = 0; nameRow < TOTAL && //loop through total names and
		name[nameRow][0] != '\0'; nameRow++) { //check if there's a null.
		fprintf(fileCSV, "%s,", name[nameRow]); //print the name
		for(rankColumn = 0; rankColumn < RANK_LENGTH; rankColumn++) {
			if(rank[nameRow][rankColumn] != 0) { //if it's not 0
				fprintf(fileCSV, "%d,", rank[nameRow][rankColumn]);
			} else { //otherwise print a blank space.
				fprintf(fileCSV, ",");
			}
		} // new line for a new line.
		fprintf(fileCSV, "\n");
	}
}
