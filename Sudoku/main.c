#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int BLOCK_SIZE = 3;

typedef enum {
	SUCCESS,
	LINE_ERROR,
	COLUMN_ERROR,
	SQUARE_ERROR,
	EMPTY_ERROR,
	NOT_EMPTY_ERROR,
} Status;

typedef enum {
	ACTION_FAILURE,
	ACTION_ADD,
	ACTION_DELETE,
} Action;

typedef enum {
	DIFFICULTY_ERROR,
	EASY,
	MEDIUM,
	HARD,
}Difficulties;

typedef struct {
	char* nums;
	int size;
	int nums_left;
} Sudoku;

typedef struct {
	int* x;
	int* y;
	int initial_quantity;
} Initial_pos;

void clean_buffer(void) {
	int c;
	while ((c = getchar()) != EOF && c != '\n');
}

static Initial_pos init_pos;
static Sudoku game;
void init_sudoku(void) {
	if (game.size == 0) {
		game.size = 9;
		
		int num_elems = game.size * game.size;

		game.nums = malloc(num_elems + 1);
		if (!game.nums) exit(EXIT_FAILURE);

		game.nums[num_elems] = '\0';

		memset(game.nums, ' ', num_elems);
		game.nums_left = num_elems;
	}
}

void destroy_sudoku(void) {
	game.size = 0;

	free(game.nums);
	game.nums = NULL;

	game.nums_left = 0;

	free(init_pos.x);
	init_pos.x = NULL;

	free(init_pos.y);
	init_pos.y = NULL;
}

void reset_sudoku(void) {
	int num_elems = game.size * game.size;

	if (!game.nums) return;
	memset(game.nums, ' ', num_elems);

	game.nums_left = num_elems;

	free(init_pos.x);
	init_pos.x = NULL;

	free(init_pos.y);
	init_pos.y = NULL;
}

inline int idx(int x, int y) {
	return x + (y * game.size);
}

void display_grid(void) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	int limit = game.size * 2 + 1;
	for (int i = 0; i < limit; ++i) {
		for (int j = 0; j < limit; ++j) {
			if ((i & 1) == 0) printf("%s", (j & 1) ? "---" : "-|-");
			else printf(" %c ", (j & 1) ? game.nums[(j / 2) + ((i / 2) * game.size)] : '|');
		}
		printf("\n");
	}
}

bool line_check(int y, char num) {
	for (int i = 0; i < game.size; ++i) {
		if (game.nums[i + (y * game.size)] == num) return false;
	}

	return true;
}

bool column_check(int x, char num) {
	for (int i = 0; i < game.size; ++i) {
		if (game.nums[x + (i * game.size)] == num) return false;
	}

	return true;
}

bool square_check(int x, int y, char num) {
	int start_x = BLOCK_SIZE * (x / BLOCK_SIZE);
	int end_x = start_x + BLOCK_SIZE;
	int start_y = BLOCK_SIZE * (y / BLOCK_SIZE);
	int end_y = start_y + BLOCK_SIZE;

	for (int i = start_y; i < end_y; ++i) {
		for (int j = start_x; j < end_x; ++j) {
			if (game.nums[j + (i * game.size)] == num) return false;
		}
	}

	return true;
}

Status number_check(int x, int y, char num) {
	if (!line_check(y, num)) return LINE_ERROR;
	if (!column_check(x, num)) return COLUMN_ERROR;
	if (!square_check(x, y, num)) return SQUARE_ERROR;

	return SUCCESS;
}

void set_easy(void) {
	game.nums[idx(0, 0)] = '7';
	game.nums[idx(8, 0)] = '8';
	game.nums[idx(2, 1)] = '5';
	game.nums[idx(4, 1)] = '6';
	game.nums[idx(6, 1)] = '1';
	game.nums[idx(0, 2)] = '8';
	game.nums[idx(1, 2)] = '4';
	game.nums[idx(3, 2)] = '7';
	game.nums[idx(5, 2)] = '1';
	game.nums[idx(7, 2)] = '9';
	game.nums[idx(8, 2)] = '5';
	game.nums[idx(2, 3)] = '2';
	game.nums[idx(5, 3)] = '7';
	game.nums[idx(6, 3)] = '5';
	game.nums[idx(1, 4)] = '7';
	game.nums[idx(7, 4)] = '6';
	game.nums[idx(2, 5)] = '8';
	game.nums[idx(3, 5)] = '9';
	game.nums[idx(6, 5)] = '3';
	game.nums[idx(0, 6)] = '2';
	game.nums[idx(1, 6)] = '5';
	game.nums[idx(3, 6)] = '1';
	game.nums[idx(5, 6)] = '4';
	game.nums[idx(7, 6)] = '3';
	game.nums[idx(8, 6)] = '9';
	game.nums[idx(2, 7)] = '7';
	game.nums[idx(4, 7)] = '5';
	game.nums[idx(6, 7)] = '8';
	game.nums[idx(0, 8)] = '1';
	game.nums[idx(8, 8)] = '2';
}

void set_medium(void) {
	game.nums[idx(0, 0)] = '9';
	game.nums[idx(4, 0)] = '8';
	game.nums[idx(7, 0)] = '5';
	game.nums[idx(8, 0)] = '4';
	game.nums[idx(1, 1)] = '1';
	game.nums[idx(8, 1)] = '8';
	game.nums[idx(2, 2)] = '7';
	game.nums[idx(3, 2)] = '6';
	game.nums[idx(5, 2)] = '4';
	game.nums[idx(7, 2)] = '2';
	game.nums[idx(1, 3)] = '2';
	game.nums[idx(4, 3)] = '3';
	game.nums[idx(7, 3)] = '4';
	game.nums[idx(3, 4)] = '9';
	game.nums[idx(5, 4)] = '8';
	game.nums[idx(0, 5)] = '7';
	game.nums[idx(4, 5)] = '4';
	game.nums[idx(8, 5)] = '2';
	game.nums[idx(1, 6)] = '4';
	game.nums[idx(3, 6)] = '8';
	game.nums[idx(5, 6)] = '7';
	game.nums[idx(6, 6)] = '1';
	game.nums[idx(0, 7)] = '3';
	game.nums[idx(7, 7)] = '9';
	game.nums[idx(0, 8)] = '1';
	game.nums[idx(1, 8)] = '7';
	game.nums[idx(4, 8)] = '9';
	game.nums[idx(8, 8)] = '6';
}

void set_hard(void) {
	game.nums[idx(8, 0)] = '2';
	game.nums[idx(1, 1)] = '9';
	game.nums[idx(3, 1)] = '1';
	game.nums[idx(4, 1)] = '4';
	game.nums[idx(5, 1)] = '7';
	game.nums[idx(0, 2)] = '8';
	game.nums[idx(2, 2)] = '7';
	game.nums[idx(3, 2)] = '5';
	game.nums[idx(5, 2)] = '2';
	game.nums[idx(1, 3)] = '6';
	game.nums[idx(2, 3)] = '9';
	game.nums[idx(4, 3)] = '2';
	game.nums[idx(5, 3)] = '1';
	game.nums[idx(6, 3)] = '3';
	game.nums[idx(7, 3)] = '8';
	game.nums[idx(1, 4)] = '3';
	game.nums[idx(3, 4)] = '6';
	game.nums[idx(5, 4)] = '8';
	game.nums[idx(7, 4)] = '9';
	game.nums[idx(1, 5)] = '8';
	game.nums[idx(2, 5)] = '4';
	game.nums[idx(3, 5)] = '7';
	game.nums[idx(4, 5)] = '3';
	game.nums[idx(6, 5)] = '2';
	game.nums[idx(7, 5)] = '6';
	game.nums[idx(3, 6)] = '8';
	game.nums[idx(5, 6)] = '5';
	game.nums[idx(6, 6)] = '1';
	game.nums[idx(3, 7)] = '2';
	game.nums[idx(4, 7)] = '1';
	game.nums[idx(5, 7)] = '6';
	game.nums[idx(7, 7)] = '3';
	game.nums[idx(0, 8)] = '1';
	game.nums[idx(6, 8)] = '5';
}

Difficulties get_difficulty(void) {
	Difficulties difficulty;
	printf("Escolha a dificuldade do jogo\n1- Facil\n2- Medio\n3- Dificil\n");
	if (scanf("%d", &difficulty) != 1) {
		clean_buffer();
		return DIFFICULTY_ERROR;
	}

	while (difficulty < 1 || difficulty > 3) {
		printf("Dificuldade invalida, insira novamente: ");
		if (scanf("%d", &difficulty) != 1) {
			clean_buffer();
			return DIFFICULTY_ERROR;
		}
	}

	return difficulty;
}

bool get_initial_pos(int initial_count) {
	init_pos.initial_quantity = 0;

	init_pos.x = malloc(initial_count * sizeof(*init_pos.x));
	if (!init_pos.x) return false;

	init_pos.y = malloc(initial_count * sizeof(*init_pos.y));
	if (!init_pos.y) {
		free(init_pos.x);
		init_pos.x = NULL;
		return false;
	}

	int x_idx = 0;
	int y_idx = 0;

	for (int i = 0; i < game.size; ++i) {
		for (int j = 0; j < game.size; ++j) {
			if (game.nums[idx(j, i)] != ' ') {
				init_pos.x[x_idx++] = j;
				init_pos.y[y_idx++] = i;
			}
		}
	}

	init_pos.initial_quantity = initial_count;

	return true;
}

int get_initial_count(void) {
	int count = 0;
	for (int i = 0; i < game.size; ++i) {
		for (int j = 0; j < game.size; ++j) {
			count += game.nums[idx(j, i)] != ' ';
		}
	}
	
	return count;
}

void choose_difficulty(void) {
	int difficulty;
	while ((difficulty = get_difficulty()) == DIFFICULTY_ERROR) {
		printf("Falha ao ler dificuldade\n");
	}

	switch (difficulty) {
	case EASY:
		set_easy();
		break;
	case MEDIUM:
		set_medium();
		break;
	case HARD:
		set_hard();
		break;
	default:
		printf("Dificuldade nao escolhida\n");
	}

	get_initial_pos(get_initial_count());
}

bool check_init_pos(int x, int y) {
	for (int i = 0; i < init_pos.initial_quantity; ++i) {
		if (x == init_pos.x[i] && y == init_pos.y[i]) return false;
	}

	return true;
}

bool get_pos(int* x_io, int* y_io) {
	printf("Insira uma coordenada x e y (0-%d): ", game.size - 1);
	if (scanf("%d %d", x_io, y_io) != 2) {
		clean_buffer();
		return false;
	}

	while (*x_io < 0 || *x_io >= game.size || *y_io < 0 || *y_io >= game.size) {
		printf("Posicao invalida, insira novamente: ");
		if (scanf("%d %d", x_io, y_io) != 2) {
			clean_buffer();
			return false;
		}
	}

	return true;
}

bool get_number(char* num) {
	printf("Insira um numero (1-%d): ", game.size);
	if (scanf(" %c", num) != 1) {
		clean_buffer();
		return false;
	}

	while (*num < '1' || *num > game.size + '0') {
		printf("Numero invalido, insira novamente: ");
		if (scanf(" %c", num) != 1) {
			clean_buffer();
			return false;
		}
	}

	return true;
}

Status set_num(void) {
	int x, y;
	while (!get_pos(&x, &y)) {
		display_grid();
		printf("Falha ao ler posicao\n");
	}

	if (game.nums[idx(x, y)] != ' ') return NOT_EMPTY_ERROR;

	char num;
	while (!get_number(&num)) {
		printf("Falha ao ler numero\n");
	}

	Status number_status = number_check(x, y, num);
	if (number_status != SUCCESS) return number_status;

	game.nums[idx(x, y)] = num;

	--game.nums_left;

	return SUCCESS;
}

Status delete_num(void) {
	int x, y;
	while (!get_pos(&x, &y)) {
		display_grid();
		printf("Falha ao ler posicao\n");
	}

	while (!check_init_pos(x, y)) {
		printf("Os numeros nessas posicoes nao pode ser removidos\n");

		while (!get_pos(&x, &y)) {
			display_grid();
			printf("Falha ao ler posicao\n");
		}
	}

	if (game.nums[idx(x, y)] == ' ') return EMPTY_ERROR;

	game.nums[idx(x, y)] = ' ';

	++game.nums_left;

	return SUCCESS;
}

Action get_choice(void) {
	Action choice;
	printf("Escolha sua acao\n1- adicionar numero\n2- deletar numero\n");
	if (scanf("%d", &choice) != 1) {
		clean_buffer();
		return ACTION_FAILURE;
	}

	while (choice < 1 || choice > 2) {
		printf("Acao invalida, insira novamente: ");
		if (scanf("%d", &choice) != 1) {
			clean_buffer();
			return ACTION_FAILURE;
		}
	}

	return choice;
}

int get_play_again(void) {
	int play_again;
	printf("Deseja jogar novamente?\n1- sim\n0- nao\n");
	if (scanf("%d", &play_again) != 1) {
		clean_buffer();
		return 2;
	}

	while (play_again < 0 || play_again > 1) {
		printf("Escolha invalida, insira novamente: ");
		if (scanf("%d", &play_again) != 1) {
			clean_buffer();
			return 2;
		}
	}

	return (bool)play_again;
}

int main(void) {
	init_sudoku();

	int play_again;
	do {
		choose_difficulty();
		display_grid();
		do {
			Action choice;
			while ((choice = get_choice()) == ACTION_FAILURE) {
				display_grid();
				printf("Falha ao ler acao\n");
			}

			Status status;
			switch (choice) {
				case ACTION_ADD:
					status = set_num();
					display_grid();
					switch (status) {
					case LINE_ERROR:
						printf("Numero ja existe na linha\n");
						break;
					case COLUMN_ERROR:
						printf("Numero ja existe na coluna\n");
						break;
					case SQUARE_ERROR:
						printf("Numero ja existe no quadrante\n");
						break;
					case NOT_EMPTY_ERROR:
						printf("Posicao ja possui um numero\n");
						break;
					case SUCCESS:
						break;
					default:
						printf("Status desconhecido\n");
					}
				break;
				case ACTION_DELETE:
					status = delete_num();
					display_grid();
					if (status == EMPTY_ERROR) printf("Posicao ja esta vazia\n");
				break;
				default:
					printf("Acao desconhecida\n");
			}

		} while (game.nums_left);
		
		while ((play_again = get_play_again()) == 2) {
			display_grid();
			printf("Falha ao ler escolha\n");
		}

		reset_sudoku();
	} while (play_again);

	destroy_sudoku();

	return 0;
}
