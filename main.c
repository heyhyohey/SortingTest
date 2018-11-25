#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// ���� ��� ����
#define FILE_PATH "movies.txt"

// �迭 ũ�� ����
#define ARRAY_SIZE 100000

// ��ȭ ����ü
typedef struct movie {
	int index;			// ����
	char title[1024];	// ����
	int year;			// ����
	char genres[1024];	// �帣
}Movie;

// �Լ� ����
void execute_insertion_sort(Movie*, int);
void insertion_sort(Movie*, int);
Movie* make_movie_array(int*);
void remove_char(char*, char);
bool check_ascii(char*);
bool check_bracket(char*);
int check_quotes(char*);
int main(int argc, char* argv[]) {
	// 1. ��ȭ �����͸� �޾ƿ�
	int total = 0;	// ���� �迭�� ��
	int input = 0;  // ����� �Է°�
	char c;	// ������ ���ڸ� ���� ���� ����
	bool flag = true;	// ���� �÷���
	Movie *movies = make_movie_array(&total);		// ��ȭ ��� �迭 ����
	
	// 2. � ������ ������ ������ �Է�
	while (flag) {
		input = 6;
		c = 'a';
		printf("<Sort>\n 1) Insertino sort\n 2) Merge sort\n 3) Quick sort\n 4) Heap sort\n 5) Redix sort\n-1) Exit\ninput>> ");
		scanf("%d", &input);
		switch (input) {
		case 1:
			execute_insertion_sort(movies, total);
			break;
		case 2:
			//merge_sort();
			break;
		case 3:
			//quick_sort();
			break;
		case 4:
			//heap_sort();
			break;
		case 5:
			//redix_sort();
			break;
		case -1:
			flag = false;
			break;
		default:
			printf("Wrong input\n");
		}
		printf("\n");
		while (c != '\n')
			c = getchar(stdin);
	}	
	
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

// ��ȣ ���� Ȯ�� �Լ�
bool check_bracket(char *str) {
	for (; *str != '\0'; str++) {
		if (*str == '(' || *str == ')')
			return true;
	}
	return false;
}

// ū����ǥ ���� ī��Ʈ �Լ�
int check_quotes(char *str) {
	int cnt = 0;
	for (; *str != '\0'; str++) {
		if (*str == '\"')
			cnt++;
	}
	return cnt;
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
	Movie *movies = (Movie*)malloc(sizeof(Movie) * ARRAY_SIZE);		// ��ȭ �迭 �������� �Ҵ�
	FILE* fp = fopen(FILE_PATH, "r");	// ���� ������
	char buf[2048], title_year[1024], title[1024], genres[1024], index[128], *year[128];		// ���� �迭
	char temp[1024], dumme[1024];	// �ӽ� ���� �迭
	char* p_title_year;
	int err;

	memset(movies, 0, sizeof(Movie)*ARRAY_SIZE);

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
		err = sscanf(buf, "%[^,],%[^\n]%s", index, temp, dumme);

		// 2-5. ū����ǥ�� 2�� �̻����� �˻�
		if (check_quotes(temp) > 2)	continue;

		// 2-6. �и��� �κ��� ū����ǥ(")�� �����ϴ��� Ȯ���ϰ� ����� �帣�� �и�
		if (*temp == '\"') {
			err = sscanf(temp, "%*[\"]%[^\"]%*[\",]\",%[a-zA-Z0-9 ()|]", title_year, genres);
		}
		else
			err = sscanf(temp, "%[^,],%[a-zA-Z0-9 ()|]", title_year, genres);

		// 2-7. ������ �ִ��� �˻�
		if (!check_bracket(title_year))	continue;

		// 2-8. ����� ������ ����
		strcpy(year, strrchr(title_year, '('));
		strncpy_s(title, sizeof(title), title_year, strlen(title_year) - 7);
		if (year == NULL) continue;

		// 2-9. ������ ��ȣ�� ����
		remove_char(year, '(');
		remove_char(year, ')');
		
		// 2-10. ������ �����͸� ����
		movies[*total].index = atoi(index);
		strcpy(movies[*total].title, title);
		movies[*total].year = atoi(year);
		strcpy(movies[*total].genres, genres);
		(*total)++;
	}

	*total -= 1;
	fclose(fp);
	return movies;
}

// ���� ���� ���α׷�
void execute_insertion_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);

	for (int i = 0; i < total; i++) {
		printf("%d %s\n", all_movies[i].index, all_movies[i].title);
	}

	printf("\n<Insertion Sort>\n");
	// 1000�� ����
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 1000);

	// 5000�� ����
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 5000);

	// 10000�� ����
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 10000);

	// ��ü ����
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, total);

	free(movies);
}

// ���� ����
void insertion_sort(Movie* movies, int cnt) {
	Movie temp;
	int i, j;
	clock_t start, end;					// �ð����� ����

	start = clock();	// ���� �ð�

	// 1. ���� �������� �������� ����
	for (i = 1; i < cnt; i++) {
		j = i;
		while (j > 0 && movies[j].year < movies[j - 1].year) {
			temp = movies[j];
			movies[j] = movies[j - 1];
			movies[j - 1] = temp;
			j--;
		}
	}

	// 2. ���� �������� �������� ����
	for (i = 1; i < cnt; i++) {
		j = i;
		while (j > 0 && strcmp(movies[j].title, movies[j - 1].title) < 0 && movies[j].year == movies[j - 1].year) {
			temp = movies[j];
			movies[j] = movies[j - 1];
			movies[j - 1] = temp;
			j--;
		}
	}
	end = clock();	// ���� �ð�

	printf("%5d data\t%.3lfs \n", cnt, (double)(end - start) / (double)1000);
}