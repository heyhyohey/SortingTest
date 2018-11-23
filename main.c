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
	char title[1024];	// ����
	int year;			// ����
	char genres[1024];	// �帣
}Movie;

// �Լ� ����
Movie* make_movie_array(int*);
void remove_char(char *str, char ch);
bool check_ascii(char *str);
int main(int argc, char* argv[]) {
	// 1. ��ȭ �����͸� �޾ƿ�
	int total = 0;	// ���� �迭�� ��
	int input = 0;  // ����� �Է°�
	Movie *movies = make_movie_array(&total);		// ��ȭ ��� �迭 ����
	/*
	clock_t start, end;					// �ð����� ����
	start = clock();
	end = clock();
	printf("%.3lfs \n", (double)(end - start) / (double)1000);
	*/
	// 2. �޴����
	while (1) {
		scanf("%d", &input);
		//getche();
	}

	// 3. �Է¹��� �����͸� �������� ���� ����

	// 4. ���� ��� ��� �� �ð� ���

	
	
	free(movies);

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

// ��ȭ ������ �迭 ���� �Լ�
Movie* make_movie_array(int* total) {
	Movie *movies = (Movie*)malloc(sizeof(Movie) * 150000);		// ��ȭ �迭 �������� �Ҵ�
	FILE* fp = fopen(FILE_PATH, "r");	// ���� ������
	char buf[2048], title_year[1024], title[1024], genres[1024], index[128], year[128];		// ���� �迭
	char temp[1024], dumme[1024];	// �ӽ� ���� �迭
	int err;

	// 1. ���� �����͸� �޾Ҵ��� Ȯ��
	if (fp == NULL) {
		fprintf(stderr, "File I/O Error\n");
		exit(1);
	}

	// 2. ������ �о ����ü �迭�� ����

	// 2-1. movieId,title,genres ����
	fgets(buf, sizeof(buf), fp);
	
	while (!feof(fp)) {
		// 2-2. ���๮�� ������ �о��
		fgets(buf, sizeof(buf), fp);

		// 2-3. �ƽ�Ű �ڵ尡 �ƴ϶�� �迭���� ����
		if (!check_ascii(buf))	continue;

		// 2-4. �ε����� ������ �κ��� �и�
		err = sscanf(buf, "%[^,],%[^\n]%s]", index, temp, dumme);

		// 2-5. �и��� �κ��� ū����ǥ(")�� �����ϴ��� Ȯ���ϰ� ����� �帣�� �и�
		if (*temp == '\"') {
			err = sscanf(temp, "%*[\"]%[^\"]%*[\",]%[a-zA-Z0-9\ ()|]", title_year, genres);
		}
		else
			err = sscanf(temp, "%[^,],%[a-zA-Z0-9\ ()|]", title_year, genres);

		// 2-6. ���񿡼� ������ �����Ͽ� �и�
		err = sscanf(title_year, "%[^(](%s", title, year);

		// 2-7. ������ ��ȣ�� ����
		remove_char(year, '(');
		remove_char(year, ')');

		// 2-8. ������ �����͸� ����
		movies[*total].index = atoi(index);
		strcpy(movies[*total].title, title);
		movies[*total].year = atoi(year);
		strcpy(movies[*total].genres, genres);
		(*total)++;
	}
	
	*total -= 1;

	for (int j = 0; j < *total; j++)
		printf("%d %s %d %s\n", movies[j].index, movies[j].title, movies[j].year, movies[j].genres);

	fclose(fp);

	return movies;
}