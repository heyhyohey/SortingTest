#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// 파일 경로 정의
#define FILE_PATH "movies.txt"

// 배열 크기 정의
#define ARRAY_SIZE 100000

// 영화 구조체
typedef struct movie {
	int index;			// 순번
	char title[1024];	// 제목
	int year;			// 연도
	char genres[1024];	// 장르
}Movie;

// 함수 정의
void execute_insertion_sort(Movie*, int);
void insertion_sort(Movie*, int);
Movie* make_movie_array(int*);
void remove_char(char*, char);
bool check_ascii(char*);
bool check_bracket(char*);
int check_quotes(char*);
int main(int argc, char* argv[]) {
	// 1. 영화 데이터를 받아옴
	int total = 0;	// 최종 배열의 수
	int input = 0;  // 사용자 입력값
	char c;	// 버퍼의 문자를 비우기 위한 변수
	bool flag = true;	// 종료 플래그
	Movie *movies = make_movie_array(&total);		// 영화 목록 배열 변수
	
	// 2. 어떤 정렬을 수행할 것인지 입력
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

// 아스키코드 확인 함수
bool check_ascii(char *str) {
	for (; *str != '\0'; str++) {
		if(!isascii(*str))
			return false;
	}
	return true;
}

// 괄호 개수 확인 함수
bool check_bracket(char *str) {
	for (; *str != '\0'; str++) {
		if (*str == '(' || *str == ')')
			return true;
	}
	return false;
}

// 큰따옴표 개수 카운트 함수
int check_quotes(char *str) {
	int cnt = 0;
	for (; *str != '\0'; str++) {
		if (*str == '\"')
			cnt++;
	}
	return cnt;
}

// 문자 제거 함수
void remove_char(char *str, char ch) {
	for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복
	{
		if (*str == ch)//ch와 같은 문자일 때
		{
			strcpy(str, str + 1);
			str--;
		}
	}
}

// 영화 데이터 배열 생성 함수
Movie* make_movie_array(int* total) {
	Movie *movies = (Movie*)malloc(sizeof(Movie) * ARRAY_SIZE);		// 영화 배열 힙영역에 할당
	FILE* fp = fopen(FILE_PATH, "r");	// 파일 포인터
	char buf[2048], title_year[1024], title[1024], genres[1024], index[128], *year[128];		// 버퍼 배열
	char temp[1024], dumme[1024];	// 임시 버퍼 배열
	char* p_title_year;
	int err;

	memset(movies, 0, sizeof(Movie)*ARRAY_SIZE);

	// 1. 파일 포인터를 받았는지 확인
	if (fp == NULL) {
		fprintf(stderr, "File I/O Error\n");
		exit(1);
	}

	// 2. 파일을 읽어서 구조체 배열로 저장

	// 2-1. movieId,title,genres 제거
	fgets(buf, sizeof(buf), fp);
	
	while (!feof(fp)) {
		// 2-2. 개행문자 단위로 읽어옴
		fgets(buf, sizeof(buf), fp);

		// 2-3. 아스키 코드가 아니라면 배열에서 제외
		if (!check_ascii(buf))	continue;

		// 2-4. 인덱스와 나머지 부분을 분리
		err = sscanf(buf, "%[^,],%[^\n]%s", index, temp, dumme);

		// 2-5. 큰따옴표가 2개 이상인지 검사
		if (check_quotes(temp) > 2)	continue;

		// 2-6. 분리된 부분이 큰따옴표(")로 시작하는지 확인하고 제목과 장르를 분리
		if (*temp == '\"') {
			err = sscanf(temp, "%*[\"]%[^\"]%*[\",]\",%[a-zA-Z0-9 ()|]", title_year, genres);
		}
		else
			err = sscanf(temp, "%[^,],%[a-zA-Z0-9 ()|]", title_year, genres);

		// 2-7. 연도가 있는지 검사
		if (!check_bracket(title_year))	continue;

		// 2-8. 제목과 연도를 추출
		strcpy(year, strrchr(title_year, '('));
		strncpy_s(title, sizeof(title), title_year, strlen(title_year) - 7);
		if (year == NULL) continue;

		// 2-9. 연도의 괄호를 제거
		remove_char(year, '(');
		remove_char(year, ')');
		
		// 2-10. 정제된 데이터를 삽입
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

// 삽입 정렬 프로그램
void execute_insertion_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);

	for (int i = 0; i < total; i++) {
		printf("%d %s\n", all_movies[i].index, all_movies[i].title);
	}

	printf("\n<Insertion Sort>\n");
	// 1000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 1000);

	// 5000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 5000);

	// 10000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, 10000);

	// 전체 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	insertion_sort(movies, total);

	free(movies);
}

// 삽입 정렬
void insertion_sort(Movie* movies, int cnt) {
	Movie temp;
	int i, j;
	clock_t start, end;					// 시간측정 변수

	start = clock();	// 시작 시간

	// 1. 연도 기준으로 오름차순 정렬
	for (i = 1; i < cnt; i++) {
		j = i;
		while (j > 0 && movies[j].year < movies[j - 1].year) {
			temp = movies[j];
			movies[j] = movies[j - 1];
			movies[j - 1] = temp;
			j--;
		}
	}

	// 2. 제목 기준으로 오름차순 정렬
	for (i = 1; i < cnt; i++) {
		j = i;
		while (j > 0 && strcmp(movies[j].title, movies[j - 1].title) < 0 && movies[j].year == movies[j - 1].year) {
			temp = movies[j];
			movies[j] = movies[j - 1];
			movies[j - 1] = temp;
			j--;
		}
	}
	end = clock();	// 종료 시간

	printf("%5d data\t%.3lfs \n", cnt, (double)(end - start) / (double)1000);
}