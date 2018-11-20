#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// ���� ���
#define FILE_PATH "movies.txt"

// ��ȭ ����ü
typedef struct movie {
	int index;			// ����
	char title[2048];	// ����
	int year;			// ����
	char genres[1024];	// �帣
}Movie;

// �Լ� ����
void remove_char(char *str, char ch);
bool check_ascii(char *str);
int main(int argc, char* argv[]) {
	Movie *movies = (Movie*)malloc(sizeof(Movie) * 150000);		// ��ȭ ��� �迭 ����
	char buf[2048];						// ���� �迭 ����
	clock_t start, end;					// �ð����� ����
	FILE* fp = fopen(FILE_PATH, "r");	// ���� ������ ����
	int i = 0;	// ī��Ʈ ����
	char title[2048], genres[1024], index[1024], year[1024];
	char temp[1024], dumme[1024];

	// ���� �о���� ���н� ���α׷� ����
	if (fp == NULL) {
		fprintf(stderr, "File I/O Error\n");
		exit(1);
	}

	// 1. ������ �о ����ü �迭�� ����

	// movieId,title,genres ����
	fgets(buf, sizeof(buf), fp);
	start = clock();
	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);		// ���๮�� ������ �о��
		if (!check_ascii(buf))	continue;	// �ƽ�Ű �ڵ尡 �ƴ� ���� ����
		sscanf(buf, "%[^,],%[^\n]%s", index, temp, dumme);
		// ū����ǥ�� ������ ���
		if(*temp == '\"') {
			sscanf(temp, "%*[\"]%[^\"]%*[\",]%s", title, genres);
		}
		// ū����ǥ�� �����ϱ� ���� ���
		else
			sscanf(temp, "%[^,],%s", title, genres);

		strcpy(year, title + (strlen(title) - 6));
		*(title + (strlen(title) - 7)) = '\0';
		remove_char(year, '(');
		remove_char(year, ')');
		movies[i].index = atoi(index);
		strcpy(movies[i].title, title);
		movies[i].year = atoi(year);
		strcpy(movies[i++].genres, genres);
	}
	end = clock();
	printf("%.3lfs \n", (double)(end - start) / (double)1000);

	for (int j = 0; j < i; j++)
		printf("%d %s %d %s\n", movies[j].index, movies[j].title, movies[j].year, movies[j].genres);

	// 2. �޴����

	// 3. �Է¹��� �����͸� �������� ���� ����

	// 4. ���� ��� ��� �� �ð� ���

	//free();
	fclose(fp);

	return 0;
}

// �ƽ�Ű�ڵ� Ȯ�� �Լ�
bool check_ascii(char *str) {
	for (; *str != '\0'; str++) {
		if(!isascii(*str))
			return false;
	}
	return true;
}

// ���� ���� �Լ�
void remove_char(char *str, char ch) {
	for (; *str != '\0'; str++)//���� ���ڸ� ���� ������ �ݺ�
	{
		if (*str == ch)//ch�� ���� ������ ��
		{
			strcpy(str, str + 1);
			str--;
		}
	}
}
