#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// 파일 경로
#define FILE_PATH "movies.txt"

// 영화 구조체
typedef struct movie {
	int index;			// 순번
	char title[2048];	// 제목
	int year;			// 연도
	char genres[1024];	// 장르
}Movie;

// 함수 정의
void remove_char(char *str, char ch);
bool check_ascii(char *str);
int main(int argc, char* argv[]) {
	Movie *movies = (Movie*)malloc(sizeof(Movie) * 150000);		// 영화 목록 배열 변수
	char buf[2048];						// 버퍼 배열 변수
	clock_t start, end;					// 시간측정 변수
	FILE* fp = fopen(FILE_PATH, "r");	// 파일 포인터 변수
	int i = 0;	// 카운트 변수
	char title[2048], genres[1024], index[1024], year[1024];
	char temp[1024], dumme[1024];

	// 파일 읽어오기 실패시 프로그램 종료
	if (fp == NULL) {
		fprintf(stderr, "File I/O Error\n");
		exit(1);
	}

	// 1. 파일을 읽어서 구조체 배열로 저장

	// movieId,title,genres 제거
	fgets(buf, sizeof(buf), fp);
	start = clock();
	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);		// 개행문자 단위로 읽어옴
		if (!check_ascii(buf))	continue;	// 아스키 코드가 아닌 것은 제외
		sscanf(buf, "%[^,],%[^\n]%s", index, temp, dumme);
		// 큰따옴표로 시작할 경우
		if(*temp == '\"') {
			sscanf(temp, "%*[\"]%[^\"]%*[\",]%s", title, genres);
		}
		// 큰따옴표로 시작하기 않을 경우
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

	// 2. 메뉴출력

	// 3. 입력받은 데이터를 기준으로 정렬 수행

	// 4. 정렬 결과 출력 및 시간 출력

	//free();
	fclose(fp);

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
