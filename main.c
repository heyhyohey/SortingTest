#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <Windows.h>

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
void execute_merge_sort(Movie*, int);
void execute_quick_sort(Movie*, int);
void execute_heap_sort(Movie*, int);
void execute_redix_sort(Movie*, int);
void insertion_sort(Movie*, int);
void merge_sort(Movie*, int);
void merge(Movie*, int, int, int, int, Movie*, Movie*);
void merge_sort_by_title(Movie*, int);
void merge_by_title(Movie*, int, int, int, int, Movie*, Movie*);
void quick_sort(Movie*, int, int);
void quick_sort_by_title(Movie*, int, int);
void heap_sort(Movie*, int);
void heap_sort_by_title(Movie*, int);
void build_heap(Movie*, int);
void heaplify(Movie*, int, int);
void heaplify_by_title(Movie*, int, int);
void radix_sort(Movie*, int);
void count_sort(Movie*, int, int);
int get_max(Movie*, int);
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
	
	printf("\n");
	// 2. 어떤 정렬을 수행할 것인지 입력
	while (flag) {
		input = 6;	// 입력값 초기화
		c = 'a';	// 문자 초기화
		printf("<Sort>\n 1) Insertino sort\n 2) Merge sort\n 3) Quick sort\n 4) Heap sort\n 5) Redix sort\n-1) Exit\ninput>> ");
		scanf("%d", &input);
		system("cls");
		switch (input) {
		case 1:
			execute_insertion_sort(movies, total);
			break;
		case 2:
			execute_merge_sort(movies, total);
			break;
		case 3:
			execute_quick_sort(movies, total);
			break;
		case 4:
			execute_heap_sort(movies, total);
			break;
		case 5:
			execute_redix_sort(movies, total);
			break;
		case -1:
			printf("\n<Message>\nExit\n");
			flag = false;
			break;
		default:
			printf("\n<Message>\nWrong input\n");
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

// 삽입 정렬 실행 모듈
void execute_insertion_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);

	printf("\n<Insertion Sort> - stable\n");
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

// 합병 정렬 실행 모듈
void execute_merge_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);
	clock_t start, end;		// 시간측정 변수

	printf("\n<Merge Sort> - stable\n");
	// 1000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	merge_sort(movies, 1000);			// 연도를 기준으로 합병 정렬
	merge_sort_by_title(movies, 1000);	// 제목을 기준으로 합병 정렬
	end = clock();		// 종료 시간
	printf(" 1000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 5000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	merge_sort(movies, 5000);			// 연도를 기준으로 합병 정렬
	merge_sort_by_title(movies, 5000);	// 제목을 기준으로 합병 정렬
	end = clock();		// 종료 시간
	printf(" 5000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 10000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	merge_sort(movies, 10000);			// 연도를 기준으로 합병 정렬
	merge_sort_by_title(movies, 10000);	// 제목을 기준으로 합병 정렬
	end = clock();		// 종료 시간
	printf("10000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 전체 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	merge_sort(movies, total);			// 연도를 기준으로 합병 정렬
	merge_sort_by_title(movies, total);	// 제목을 기준으로 합병 정렬
	end = clock();		// 종료 시간
	printf("%5d data\t%.3lfs \n", total, (double)(end - start) / (double)1000);	// 시간 출력

	free(movies);
}

// 합병 정렬
void merge_sort(Movie* movies, int cnt) {
	int step = 1;
	Movie *m1 = malloc(cnt * sizeof(Movie));
	Movie *m2 = malloc(cnt * sizeof(Movie));
	while (step < cnt) {
		int i, s1, e1, s2, e2;
		for (i = 0; (i + step - 1) < (cnt - 1); i += 2 * step) {
			s1 = i;
			e1 = i + step - 1;
			s2 = e1 + 1;
			(i + 2 * step - 1) < (cnt - 1) ? (e2 = i + 2 * step - 1) : (e2 = cnt - 1);
			merge(movies, s1, e1, s2, e2, m1, m2);
		}
		step = step << 1;
	}
	free(m1);
	free(m2);
}

// 합병
void merge(Movie *movies, int s1, int e1, int s2, int e2, Movie* m1, Movie* m2) {
	int len1 = e1 - s1 + 1;
	int len2 = e2 - s2 + 1;
	int p1 = 0;
	int p2 = 0;
	int p = s1;
	memcpy(m1, movies + s1, sizeof(Movie)*len1);
	memcpy(m2, movies + s2, sizeof(Movie)*len2);
	while (p1 < len1&&p2 < len2) {
		if (m1[p1].year < m2[p2].year) {
			movies[p++] = m1[p1++];
		}
		else {
			movies[p++] = m2[p2++];
		}
	}
	while (p1 < len1) {
		movies[p++] = m1[p1++];
	}
	while (p2 < len2) {
		movies[p++] = m2[p2++];
	}
}

// 제목을 기준으로 합병 정렬
void merge_sort_by_title(Movie* movies, int cnt) {
	int step = 1;
	Movie *m1 = malloc(cnt * sizeof(Movie));
	Movie *m2 = malloc(cnt * sizeof(Movie));
	while (step < cnt) {
		int i, s1, e1, s2, e2;
		for (i = 0; (i + step - 1) < (cnt - 1); i += 2 * step) {
			s1 = i;
			e1 = i + step - 1;
			s2 = e1 + 1;
			(i + 2 * step - 1) < (cnt - 1) ? (e2 = i + 2 * step - 1) : (e2 = cnt - 1);
			merge_by_title(movies, s1, e1, s2, e2, m1, m2);
		}
		step = step << 1;
	}
	free(m1);
	free(m2);
}

// 제목을 기준으로 합병
void merge_by_title(Movie *movies, int s1, int e1, int s2, int e2, Movie* m1, Movie* m2) {
	int len1 = e1 - s1 + 1;
	int len2 = e2 - s2 + 1;
	int p1 = 0;
	int p2 = 0;
	int p = s1;
	memcpy(m1, movies + s1, sizeof(Movie)*len1);
	memcpy(m2, movies + s2, sizeof(Movie)*len2);
	while (p1 < len1&&p2 < len2) {
		if ((m1[p1].year == m2[p2].year) && strcmp(m1[p1].title, m2[p2].title) < 0) {
			movies[p++] = m1[p1++];
		}
		else {
			movies[p++] = m2[p2++];
		}
	}
	while (p1 < len1) {
		movies[p++] = m1[p1++];
	}
	while (p2 < len2) {
		movies[p++] = m2[p2++];
	}
}

// 퀵 정렬 실행 모듈
void execute_quick_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);
	clock_t start, end;		// 시간측정 변수

	printf("\n<Quick Sort> - unstable\n");
	// 1000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	quick_sort_by_title(movies, 0, 1000);	// 제목을 기준으로 퀵 정렬
	quick_sort(movies, 0, 1000);			// 연도를 기준으로 퀵 정렬

	end = clock();		// 종료 시간
	printf(" 1000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 5000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	quick_sort(movies, 0, 5000);			// 연도를 기준으로 퀵 정렬
	quick_sort_by_title(movies, 0, 5000);	// 제목을 기준으로 퀵 정렬
	end = clock();		// 종료 시간
	printf(" 5000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 10000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	quick_sort(movies, 0, 10000);			// 연도를 기준으로 퀵 정렬
	quick_sort_by_title(movies, 0, 10000);	// 제목을 기준으로 퀵 정렬
	end = clock();		// 종료 시간
	printf("10000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 전체 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	quick_sort(movies, 0, total - 1);			// 연도를 기준으로 퀵 정렬
	quick_sort_by_title(movies, 0, total - 1);	// 제목을 기준으로 퀵 정렬
	end = clock();		// 종료 시간
	printf("%5d data\t%.3lfs \n", total, (double)(end - start) / (double)1000);	// 시간 출력

	free(movies);
}

// 제목을 기준으로 퀵 정렬
void quick_sort_by_title(Movie* movies, int start, int end) {
	int i, j;
	Movie pivot, temp;
	i = start;
	j = end - 1;
	pivot = movies[(start + end) / 2];

	while (i <= j) {
		while (movies[i].year == pivot.year && (movies[i].title, pivot.title) < 0 && i < end) {
			i++;
		}
		while (movies[j].year == pivot.year && strcmp(movies[j].title, pivot.title) > 0 && j > start) {
			j--;
		}
		if (i <= j) {
			temp = movies[i];
			movies[i] = movies[j];
			movies[j] = temp;
			i++;
			j--;
		}
	}
	if (j > start)
		quick_sort(movies, start, j + 1);
	if (i < end)
		quick_sort(movies, i, end);
}

// 퀵 정렬
void quick_sort(Movie* movies, int start, int end) {
	int i, j;
	Movie pivot, temp;
	i = start;
	j = end - 1;
	pivot = movies[(start + end) / 2];

	while (i <= j) {
		while (movies[i].year < pivot.year && i < end) {
			i++;
		}
		while (movies[j].year > pivot.year && j > start) {
			j--;
		}
		if (i <= j) {
			temp = movies[i];
			movies[i] = movies[j];
			movies[j] = temp;
			i++;
			j--;
		}
	}
	if (j > start)
		quick_sort(movies, start, j + 1);
	if (i < end)
		quick_sort(movies, i, end);
}

// 힙 정렬 실행 모듈
void execute_heap_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);
	clock_t start, end;		// 시간측정 변수

	printf("\n<Heap Sort> - unstable\n");
	// 1000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	heap_sort(movies, 1000);			// 연도를 기준으로 힙 정렬
	heap_sort_by_title(movies, 1000);	// 제목을 기준으로 힙 정렬
	end = clock();		// 종료 시간
	printf(" 1000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 5000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	heap_sort(movies, 5000);			// 연도를 기준으로 힙 정렬
	heap_sort_by_title(movies, 5000);	// 제목을 기준으로 힙 정렬
	end = clock();		// 종료 시간
	printf(" 5000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 10000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	heap_sort(movies, 10000);			// 연도를 기준으로 힙 정렬
	heap_sort_by_title(movies, 10000);	// 제목을 기준으로 힙 정렬
	end = clock();		// 종료 시간
	printf("10000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 전체 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	heap_sort(movies, total);			// 연도를 기준으로 힙 정렬
	heap_sort_by_title(movies, total);	// 제목을 기준으로 힙 정렬
	end = clock();		// 종료 시간
	printf("%5d data\t%.3lfs \n", total, (double)(end - start) / (double)1000);	// 시간 출력

	free(movies);
}

// 힙 정렬
void heap_sort(Movie *movies, int cnt) {
	build_heap(movies, cnt);
	Movie temp;
	int newlen = cnt;
	while (newlen > 1) {
		//swap head with last
		temp = movies[0];
		movies[0] = movies[newlen - 1];
		movies[newlen - 1] = temp;
		newlen--;
		//heapify new heap
		heaplify(movies, newlen, 0);
	}
}

// 힙 생성
void build_heap(Movie *movies, int cnt) {
	int i;
	for (i = cnt - 1; i >= 0; i--) {
		if (2 * i + 1 > cnt - 1) continue;
		heaplify(movies, cnt, i);
	}
}

// heaplify
void heaplify(Movie *movies, int cnt, int index) {
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	Movie temp;
	if (left > cnt - 1) {
		return;
	}
	else if (left == cnt - 1) {
		if (movies[index].year < movies[left].year) {
			//swap
			temp = movies[index];
			movies[index] = movies[left];
			movies[left] = temp;
		}
		return;
	}
	else {
		if (movies[index].year < movies[left].year || movies[index].year < movies[right].year) {
			if (movies[left].year < movies[right].year) {
				//swap right with parent
				temp = movies[index];
				movies[index] = movies[right];
				movies[right] = temp;
				heaplify(movies, cnt, right);
			}
			else {
				//swap left with parent
				temp = movies[index];
				movies[index] = movies[left];
				movies[left] = temp;
				heaplify(movies, cnt, left);
			}
		}
	}
}

// 제목을 기준으로 힙 정렬
void heap_sort_by_title(Movie *movies, int cnt) {
	build_heap(movies, cnt);
	Movie temp;
	int newlen = cnt;
	while (newlen > 1) {
		//swap head with last
		temp = movies[0];
		movies[0] = movies[newlen - 1];
		movies[newlen - 1] = temp;
		newlen--;
		//heapify new heap
		heaplify_by_title(movies, newlen, 0);
	}
}

// 제목을 기준으로 heaplify
void heaplify_by_title(Movie *movies, int cnt, int index) {
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	Movie temp;
	if (left > cnt - 1) {
		return;
	}
	else if (left == cnt - 1) {
		if (strcmp(movies[index].title, movies[left].title) > 0) {
			//swap
			temp = movies[index];
			movies[index] = movies[left];
			movies[left] = temp;
		}
		return;
	}
	else {
		if (strcmp(movies[index].title, movies[left].title) > 0 || strcmp(movies[index].title, movies[right].title) > 0) {
			if (strcmp(movies[left].title, movies[right].title) > 0) {
				//swap right with parent
				temp = movies[index];
				movies[index] = movies[right];
				movies[right] = temp;
				heaplify(movies, cnt, right);
			}
			else {
				//swap left with parent
				temp = movies[index];
				movies[index] = movies[left];
				movies[left] = temp;
				heaplify(movies, cnt, left);
			}
		}
	}
}

// 기수 정렬 실행 모듈
void execute_redix_sort(Movie* all_movies, int total) {
	Movie* movies = malloc(sizeof(Movie) * ARRAY_SIZE);
	clock_t start, end;		// 시간측정 변수

	printf("\n<Redix Sort> - stable\n");
	// 1000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	radix_sort(movies, 1000);			// 연도를 기준으로 기수 정렬
	//radix_sort_by_title(movies, 1000);	// 제목을 기준으로 기수 정렬
	end = clock();		// 종료 시간
	printf(" 1000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력
/*
	for (int i = 0; i < 1000; i++) {
		printf("%d %s\n", movies[i].year, movies[i].title);
	}
	*/
	// 5000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	radix_sort(movies, 5000);			// 연도를 기준으로 기수 정렬
	end = clock();		// 종료 시간
	printf(" 5000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 10000개 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	radix_sort(movies, 10000);			// 연도를 기준으로 기수 정렬
	end = clock();		// 종료 시간
	printf("10000 data\t%.3lfs \n", (double)(end - start) / (double)1000);	// 시간 출력

	// 전체 정렬
	memcpy(movies, all_movies, sizeof(Movie) * ARRAY_SIZE);
	start = clock();	// 시작 시간
	radix_sort(movies, total);			// 연도를 기준으로 기수 정렬
	end = clock();		// 종료 시간
	printf("%5d data\t%.3lfs \n", total, (double)(end - start) / (double)1000);	// 시간 출력

	free(movies);
}

// 최대값 함수
int get_max(Movie* movies, int cnt) {
	Movie mx = movies[0];
	int i;
	for (i = 1; i < cnt; i++)
		if (movies[i].year > mx.year)
			mx = movies[i];
	return mx.year;
}

// 계수 정렬
void count_sort(Movie *movies, int cnt, int exp) {
	Movie* output = malloc(sizeof(Movie) * cnt);
	int i;
	int count[10] = { 0 };

	// 자릿수의 종류별로 카운트
	for (i = 0; i < cnt; i++)
		count[(movies[i].year / exp) % 10]++;

	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	// 출력 배열 생성
	for (i = cnt - 1; i >= 0; i--) {
		output[count[(movies[i].year / exp) % 10] - 1] = movies[i];
		count[(movies[i].year / exp) % 10]--;
	}

	for (i = 0; i < cnt; i++)
		movies[i] = output[i];

	free(output);
}

// 계수 정렬을 사용한 기수 정렬
void radix_sort(Movie* movies, int cnt) {
	int m = get_max(movies, cnt);

	int exp;
	for (exp = 1; m / exp > 0; exp *= 10)
		count_sort(movies, cnt, exp);
}