#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)     // Windows uses milliseconds
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000) // Linux/macOS uses microseconds
#endif



int get_random_number(){
	// Generate a random number (0 or 1)
    	int random_num = rand() % 100;
	if(random_num < 80){
		return 0;
	}else{
		return 1;
	}
}


int** initialize_grid(int num_rows, int num_cols) {
    int** matrix = malloc(num_rows * sizeof(int*));
    if (matrix == NULL) return NULL;
    for (int i = 0; i < num_rows; i++) {
        matrix[i] = malloc(num_cols * sizeof(int));
        if (matrix[i] == NULL) return NULL;
        for (int j = 0; j < num_cols; j++) {
            matrix[i][j] = get_random_number();
        }
    }
    return matrix;
}


int sum_list(int* list, int size){
	int result = 0;
	for(int i=0; i<size; i++){
		result += list[i];
	}
	return result;
}


int compute_cell_next_state(int num_rows, int num_cols, int** curr_state, int cell_row_index, int cell_column_index){
	// given a cell, compute it next value in regard to neighbour values.
	// rules:
	// 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
	// 2. Any live cell with two or three live neighbours lives on to the next generation.
	// 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
	// 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
	int neighbours[8] = {0,0,0,0,0,0,0,0};
	if(cell_row_index == 0){
		if(cell_column_index == 0){
			neighbours[3] = curr_state[cell_row_index][cell_column_index + 1];
			neighbours[4] = curr_state[cell_row_index + 1][cell_column_index + 1];
			neighbours[5] = curr_state[cell_row_index + 1][cell_column_index];
		}else if(cell_column_index == num_cols - 1){
			neighbours[7] = curr_state[cell_row_index][cell_column_index - 1];
			neighbours[6] = curr_state[cell_row_index + 1][cell_column_index - 1];
			neighbours[5] = curr_state[cell_row_index + 1][cell_column_index];
		}
	}else if(cell_row_index == num_rows - 1){
		if(cell_column_index == 0){
			neighbours[1] = curr_state[cell_row_index - 1][cell_column_index];
			neighbours[2] = curr_state[cell_row_index - 1][cell_column_index + 1];
			neighbours[3] = curr_state[cell_row_index][cell_column_index + 1];
		}else if(cell_column_index == num_cols - 1){
			neighbours[0] = curr_state[cell_row_index - 1][cell_column_index -1];
			neighbours[1] = curr_state[cell_row_index - 1][cell_column_index];
			neighbours[7] = curr_state[cell_row_index][cell_column_index - 1];
		}
	}
	else if(cell_column_index == 0){
		neighbours[1] = curr_state[cell_row_index - 1][cell_column_index];
		neighbours[2] = curr_state[cell_row_index - 1][cell_column_index + 1];
		neighbours[3] = curr_state[cell_row_index][cell_column_index + 1];
		neighbours[4] = curr_state[cell_row_index + 1][cell_column_index + 1];
		neighbours[5] = curr_state[cell_row_index + 1][cell_column_index];
	}
	else if(cell_column_index == num_cols - 1){
		neighbours[0] = curr_state[cell_row_index - 1][cell_column_index - 1];
		neighbours[1] = curr_state[cell_row_index - 1][cell_column_index];
		neighbours[5] = curr_state[cell_row_index + 1][cell_column_index];
		neighbours[6] = curr_state[cell_row_index + 1][cell_column_index - 1];
		neighbours[7] = curr_state[cell_row_index][cell_column_index - 1];
	}else{
		neighbours[0] = curr_state[cell_row_index - 1][cell_column_index - 1];
		neighbours[1] = curr_state[cell_row_index - 1][cell_column_index];
		neighbours[2] = curr_state[cell_row_index - 1][cell_column_index + 1];
		neighbours[3] = curr_state[cell_row_index][cell_column_index + 1];
		neighbours[4] = curr_state[cell_row_index + 1][cell_column_index + 1];
		neighbours[5] = curr_state[cell_row_index + 1][cell_column_index];
		neighbours[6] = curr_state[cell_row_index + 1][cell_column_index - 1];
		neighbours[7] = curr_state[cell_row_index][cell_column_index - 1];
	}
        // deduce cell value from neighbourhood.
	int curr_cell_state = curr_state[cell_row_index][cell_column_index];
	if(curr_cell_state == 1 && sum_list(neighbours, 8) < 2){
		curr_state[cell_row_index][cell_column_index] = 0;
	}else if(curr_cell_state == 1 && sum_list(neighbours, 8) <= 3){
		curr_state[cell_row_index][cell_column_index] = 1;	
	}else if(curr_cell_state == 1 && sum_list(neighbours, 8) > 3){
		curr_state[cell_row_index][cell_column_index] = 0;
	}else if(curr_cell_state == 0 && sum_list(neighbours, 8) == 3){
		curr_state[cell_row_index][cell_column_index] = 1;
	}
	return 1;
}


int compute_next_state(int num_rows, int num_cols, int** curr_state){
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			compute_cell_next_state(num_rows, num_cols, curr_state, i, j);	
		}
	}
	return 1;
}


int display_state(int num_rows, int num_cols, int** curr_state){
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			if(curr_state[i][j] == 1){
				printf("\u2588\u2588");
			}else{
				printf("  ");
			}
		}
		printf("\n");
	}	
	return 1;	
}


void clearScreen() {
    // \033[H  - Moves cursor to home position
    // \033[2J - Clears the entire screen
    printf("\033[H\033[2J");
}


int main(int argc, char** argv){
	setlocale(LC_CTYPE, "en_US.UTF-8");
	srand(time(NULL));
	int rows_num = 50;
	int cols_num = 50;
	int** world = initialize_grid(rows_num, cols_num);
	while(1){
		clearScreen();
		compute_next_state(rows_num, cols_num, world);
		display_state(rows_num, cols_num, world);
		SLEEP(500);
	}
	return 1;
}
