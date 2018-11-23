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
	char title[1024];	// 제목
	int year;			// 연도
	char genres[1024];	// 장르
}Movie;

// 함수 정의
Movie* make_movie_array(int*);
void remove_char(char *str, char ch);
bool check_ascii(char *str);
int main(int argc, char* argv[]) {
	// 1. 영화 데이터를 받아옴
	int total = 0;	// 최종 배열의 수
	int input = 0;  // 사용자 입력값
	Movie *movies = make_movie_array(&total);		// 영화 목록 배열 변수
	/*
	clock_t start, end;					// 시간측정 변수
	start = clock();
	end = clock();
	printf("%.3lfs \n", (double)(end - start) / (double)1000);
	*/
	// 2. 메뉴출력
	while (1) {
		scanf("%d", &input);
		//getche();
	}

	// 3. 입력받은 데이터를 기준으로 정렬 수행

	// 4. 정렬 결과 출력 및 시간 출력

	
	
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
	Movie *movies = (Movie*)malloc(sizeof(Movie) * 150000);		// 영화 배열 힙영역에 할당
	FILE* fp = fopen(FILE_PATH, "r");	// 파일 포인터
	char buf[2048], title_year[1024], title[1024], genres[1024], index[128], year[128];		// 버퍼 배열
	char temp[1024], dumme[1024];	// 임시 버퍼 배열
	int err;

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
		err = sscanf(buf, "%[^,],%[^\n]%s]", index, temp, dumme);

		// 2-5. 분리된 부분이 큰따옴표(")로 시작하는지 확인하고 제목과 장르를 분리
		if (*temp == '\"') {
			err = sscanf(temp, "%*[\"]%[^\"]%*[\",]%[a-zA-Z0-9\ ()|]", title_year, genres);
		}
		else
			err = sscanf(temp, "%[^,],%[a-zA-Z0-9\ ()|]", title_year, genres);

		// 2-6. 제목에서 연도를 추출하여 분리
		err = sscanf(title_year, "%[^(](%s", title, year);

		// 2-7. 연도의 괄호를 제거
		remove_char(year, '(');
		remove_char(year, ')');

		// 2-8. 정제된 데이터를 삽입
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