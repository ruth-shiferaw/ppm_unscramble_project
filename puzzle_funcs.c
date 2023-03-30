// Ruth Shiferaw <RSHIFER1>
// Kalista Fong <kfong4>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "puzzle.h"
#include "ppm_io.h"
#include <stdbool.h>

Puzzle *puzzle_create(int new_size) {
    Puzzle *current_puzz = malloc(sizeof(Puzzle));
    current_puzz->size = new_size;
    current_puzz->black_row = 0;
    current_puzz->black_col = 0;
    current_puzz->puzzle_arr = (int **)malloc(new_size * sizeof(int *)); // creating space for rows
    for (int i = 0; i < new_size; ++i) { // then creating space for columns
        current_puzz->puzzle_arr[i] = calloc(new_size, sizeof(int));
    }
    return current_puzz;
}

int puzzle_get_tile(const Puzzle *p, int col, int row) {
    int tile_value = 0;
    tile_value =p->puzzle_arr[row][col];
    return tile_value;
}

void puzzle_destroy(Puzzle *p) {
    // freeing all the sub-arrays
    int limit = p->size;
    for (int i = 0; i < limit; ++i) {
        free(p->puzzle_arr[i]);
    }
    // then freeing the top level
    free(p->puzzle_arr);
    free(p);
}

void image_destroy(Image *imge) {
    free(imge->data); // freeing the data Pixel array
    free(imge); // freeing the entire struct
}

int handle_C_command(FILE *in, Puzzle **p) {
    int size_to_pass = 0;
    if (in == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (fscanf(in, " %d", &size_to_pass) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    else if (size_to_pass < 2 || size_to_pass > 20) {
        fprintf(stderr, "Invalid puzzle size\n");
        return 1;
    } else {
        *p = puzzle_create(size_to_pass); // creating puzzle and updating instance in main
        return 0;
    }
}

int handle_T_command(FILE *in, Puzzle **p) {
    if (in == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    if (*p == NULL) { // handle error where puzzle has not been created
        fprintf(stderr, "No puzzle\n");
        return 1;
    } else {
        int scanned_int = 0;
        int desired_size = (*p)->size;
        int num_tiles = desired_size * desired_size;

        char I_whitespace = '\0';
        if ((fscanf(in, "%c", &I_whitespace)) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }

        for (int rows = 0; rows < num_tiles/desired_size; rows++) {
            for (int cols = 0; cols < num_tiles/desired_size; cols++) {
                if ((I_whitespace != '\n') && (I_whitespace != 'I')) {
                    if (((fscanf(in, "%d", &scanned_int))!= 1) || ((fscanf(in, "%c", &I_whitespace)) != 1)) {
                        fprintf(stderr, "Invalid input\n");
                    } else if (scanned_int < 0 || scanned_int > (num_tiles)) { // handle error for invalid tile value
                        fprintf(stderr, "Invalid tile value\n");
                        return 1;
                    } else {
                        (*p)->puzzle_arr[rows][cols] = scanned_int;
                        if (scanned_int == 0) { // updating black tile fields in struct
                            (*p)->black_row = rows;
                            (*p)->black_col = cols;
                        }
                    }
                } else if ((I_whitespace == '\n') || (I_whitespace == 'W')) { //stops the loop from reading too much
                    return 0;
                } else {
                    return 0;
                }
            }
        }
    }
    return 0;
}

int handle_I_command(FILE *in, Image **mainimg) {
    int max_ppmfile_length = 255;
    char ppm_filename[max_ppmfile_length];
    char delete[3];

    if (in == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    fgets(delete, 2, in); // for some reason when using fgets next there's two whitespaces, this deletes them
    
    fgets(ppm_filename, max_ppmfile_length, in); //gets the string (fgets retrieves until end of string so that fixed the issue we were getting earlier with it reading more input)
    
    int file_len = strlen(ppm_filename);
    ppm_filename[(file_len - 1)] = '\0'; //makes sure the \n symbol read from .txt is eliminated with \0

    FILE *input_ppm = fopen(ppm_filename, "r");
    
    if (!input_ppm) { // checks if input file is NULL
        fprintf(stderr, "Could not open image file '%s'\n", ppm_filename);
        return 1;
    }

    *mainimg = ReadPPM(input_ppm);
    fclose(input_ppm);
    return 0;
}

int handle_P_command(Puzzle *p) {
    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    int puzz_rows = p->size;
    int puzz_cols = p->size;

    for (int i = 0; i < puzz_rows; i++) {
        for (int j = 0; j < puzz_cols; j++) {
            printf("%d ", p->puzzle_arr[i][j]);
        }
    }
    printf("\n");
    return 0;
}

int handle_W_command(FILE *in, Puzzle *p, Image *image) {
    //handling initial errors
    if (in == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    if (image == NULL) {
        fprintf(stderr, "No image\n");
        return 1;
    }
    
    int puzz_size = p->size;
    int puzz_rows = p->size;
    int puzz_cols = p->size;
    int image_rows = image->rows;
    int image_cols = image->cols;

    //creating new instance of Image struct called img
    Image *img;
    img =  malloc(sizeof(Image));
    img->data = (Pixel *)malloc(image_rows * image_cols * sizeof(Pixel));
    img->rows = image->rows;
    img->cols = image->cols;

    // copying image->data to the new img->data
    for (int i = 0; i < image_rows; i++) {
        for (int j = 0; j < image_cols; j++) {
            img->data[i * image_cols + j] = image->data[i * image_cols + j];
        }
    }

    if ( (image_rows % puzz_size != 0) || (image_cols % puzz_size != 0)) {
        fprintf(stderr, "Invalid image dimensions\n");
        return 1;
    }

    char bothfiles [510];
    char filename1 [255];
    char filename2 [255];

    //saving the entire line of .txt into bothfiles[]
    char delete[2];
    fgets(delete, 2, in);
    fgets(bothfiles, 510, in);

    int bothfiles_length = (int) strlen(bothfiles);
    char current_char = '\n';
    bool file1_filled = false;
    int file1_index = 0;
    int file2_index = 0;

    //separating bothfiles into filename1 and filename2 arrays
    for (int i = 0; i < bothfiles_length; i++) {
        current_char = bothfiles[i];
        if (current_char == ' ') {
            file1_filled = true;
            continue;
        }
        if (current_char == '\n') {
            break;
        }
        if (file1_filled) {
            filename1[file1_index] = current_char;
            file1_index++;
        } else {
            filename2[file2_index] = current_char;
            file2_index++;
        }
    }

    // null terminating both files
    filename1[file1_index] = '\0'; //SAVES THE .TXT FILE
    filename2[file2_index] = '\0'; //SAVES THE .PPM FILE

    int pixels_per_row = (image_rows/puzz_rows);
    int pixels_per_col = (image_cols/puzz_cols);
    int tile_value = 0;
    int num_tiles = (puzz_size*puzz_size);

    for (int tilecount = 0; tilecount < num_tiles; tilecount++) {
        int dest_row = (tilecount/puzz_rows);
        int dest_col = (tilecount%puzz_size);
        int destinationtopleft = dest_row*(pixels_per_row) * image_cols + dest_col * pixels_per_col;
        tile_value = puzzle_get_tile(p, dest_col, dest_row) -1;
        int real_row = (tile_value/puzz_rows);
        int real_col = (tile_value%puzz_size);
        int realtopleft = real_row*(pixels_per_row) * image_cols + real_col * pixels_per_col;

        for (int i = 0; i < pixels_per_row; i++) {
            for (int j = 0; j < pixels_per_col; j++) {   
                if (tile_value == -1) {
                    (img) -> data[(i * (image_cols) + j) + destinationtopleft].r = 0;
                    (img) -> data[(i * (image_cols) + j) + destinationtopleft].g = 0;
                    (img) -> data[(i * (image_cols) + j) + destinationtopleft].b = 0;
                } else {
                    img -> data[(i * (image_cols) + j) + destinationtopleft] = image->data[(i * (image_cols) + j) + realtopleft];
                }
            }
        }
    }

    //writing puzzle image to filename2 (the .ppm file)
    FILE *output_file1 = fopen(filename2, "w");
    if (output_file1 == NULL) {
        fprintf(stderr, "Could not open output image file %s\n", filename1);
        fclose(output_file1);
        return 1;
    } else {
        if ((WritePPM(output_file1, img)) < 0) { //using writeppm function to transfer info to new .ppm file
            fprintf(stderr, "Could not write puzzle image to %s\n", filename2);//error checking: if writeppm returns negative num of pixels written
            fclose(output_file1);
            return 1;
        }
    }

    //opening .txt file to print puzzle configuration to it
    FILE *output_file2 = fopen(filename1, "w"); 
    if (output_file2 == NULL) {
        fprintf(stderr, "Could not open output puzzle file %s\n", filename2);
        return 1;
    } else {
        for (int i = 0; i < puzz_rows; i++) {
            for (int j = 0; j < puzz_cols; j++) {
                fprintf(output_file2, "%d ", p->puzzle_arr[i][j]);
                if ((p->puzzle_arr[i][j]) == ' ') {
                    fprintf(stderr, "Could not write puzzle data %s", filename2);
                    fclose(output_file1);
                    fclose(output_file2);
                    return 1;

                }
            }
        }
    }
    
    fclose(output_file1);
    fclose(output_file2);
    return 0;
}

int handle_K_command(Puzzle *p) {
    int winning_array [p->size][p->size];
    int counter = 1;
    bool win_or_lose = true; 
    if (p == NULL) { // handle error where puzzle has not been created
        fprintf(stderr, "No puzzle\n");
        return 1; //use 1 for errors
    }

    for (int i = 0; i < (p->size); i++) {
        for (int j = 0; j < (p->size); j++) {
            winning_array[i][j] = counter;
            counter++;
        }
    }

    winning_array[p->size - 1][p->size - 1] = 0;

    for (int i = 0; i < p->size && win_or_lose; i++) {
        for (int j = 0; j < p->size && win_or_lose; j++) {
            if (winning_array[i][j] != p->puzzle_arr[i][j]) {
                win_or_lose = false;
            }
        }
    }

    if (win_or_lose == true) {
        printf("Solved\n");
        return 3; //if solved, use 3
    } else {
        printf("Not solved\n"); //if not solved, exit normally
        return 0; 
    }
}

bool modified_K_command (Puzzle *p) {
    int winning_array [p->size][p->size];
    int counter = 1;
    bool win_or_lose = true; 
    if (p == NULL) { // handle error where puzzle has not been created
        fprintf(stderr, "No puzzle\n");
        return 1; //use 1 for errors
    }

    for (int i = 0; i < (p->size); i++) {
        for (int j = 0; j < (p->size); j++) {
            winning_array[i][j] = counter;
            counter++;
        }
    }

    winning_array[p->size - 1][p->size - 1] = 0;

    for (int i = 0; i < p->size && win_or_lose; i++) {
        for (int j = 0; j < p->size && win_or_lose; j++) {
            if (winning_array[i][j] != p->puzzle_arr[i][j]) {
                win_or_lose = false;
            }
        }
    }

    return win_or_lose;
} 

int handle_S_command(FILE *in, Puzzle *p) {
    if (in == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    
    int puzz_size = p->size;
    char player_move; 
    int temp = 0; 
    int current_black_row = p->black_row;
    int current_black_col =  p->black_col;
    int result = fscanf(in, " %c", &player_move);
    if (result == EOF) {
        fprintf(stderr, "End of file reached\n");
        return 1;
    } else if (result != 1) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    switch(player_move) {
        case 'u':
            if (current_black_row == puzz_size - 1) {
                fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = p->puzzle_arr[current_black_row + 1][current_black_col];
            p->puzzle_arr[current_black_row + 1][current_black_col] = p->puzzle_arr[current_black_row][current_black_col];
            p->puzzle_arr[current_black_row][current_black_col] = temp; 
            p->black_row = current_black_row + 1;
            return 0;
        case 'd':
            if (current_black_row == 0) {
                fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = p->puzzle_arr[current_black_row - 1][current_black_col];
            p->puzzle_arr[current_black_row - 1][current_black_col] = p->puzzle_arr[current_black_row][current_black_col];
            p->puzzle_arr[current_black_row][current_black_col] = temp; 
            p->black_row = current_black_row - 1;
            return 0;
        case 'l':
            if (current_black_col == puzz_size - 1) {
                fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = p->puzzle_arr[current_black_row][current_black_col + 1];
            p->puzzle_arr[current_black_row][current_black_col + 1] = p->puzzle_arr[current_black_row][current_black_col];
            p->puzzle_arr[current_black_row][current_black_col] = temp; 
            p->black_col = current_black_col + 1;
            return 0;
        case 'r':
            if (current_black_col == 0) {
                fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp =  p->puzzle_arr[current_black_row][current_black_col - 1];
            p->puzzle_arr[current_black_row][current_black_col - 1] = p->puzzle_arr[current_black_row][current_black_col];
            p->puzzle_arr[current_black_row][current_black_col] = temp; 
            p->black_col = current_black_col - 1;
            return 0;
        default:
            fprintf(stderr, "Invalid input");
            return 1;
    }
    return 0;
}

int modified_S_command(char desired_direction, Puzzle **p) {
    if (*p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }

    int puzz_size = (*p)->size;
    int temp = 0; 
    int current_black_row = (*p)->black_row;
    int current_black_col =  (*p)->black_col;

    switch(desired_direction) {
        case 'u':
            if (current_black_row == puzz_size - 1) {
                //fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = (*p)->puzzle_arr[current_black_row + 1][current_black_col];
            (*p)->puzzle_arr[current_black_row + 1][current_black_col] = (*p)->puzzle_arr[current_black_row][current_black_col];
            (*p)->puzzle_arr[current_black_row][current_black_col] = temp; 
            (*p)->black_row = current_black_row + 1;
            return 0;
        case 'd':
            if (current_black_row == 0) {
                //fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = (*p)->puzzle_arr[current_black_row - 1][current_black_col];
            (*p)->puzzle_arr[current_black_row - 1][current_black_col] = (*p)->puzzle_arr[current_black_row][current_black_col];
            (*p)->puzzle_arr[current_black_row][current_black_col] = temp; 
            (*p)->black_row = current_black_row - 1;
            return 0;
        case 'l':
            if (current_black_col == puzz_size - 1) {
                //fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp = (*p)->puzzle_arr[current_black_row][current_black_col + 1];
            (*p)->puzzle_arr[current_black_row][current_black_col + 1] = (*p)->puzzle_arr[current_black_row][current_black_col];
            (*p)->puzzle_arr[current_black_row][current_black_col] = temp; 
            (*p)->black_col = current_black_col + 1;
            return 0;
        case 'r':
            if (current_black_col == 0) {
                //fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
                return 1;
            }
            temp =  (*p)->puzzle_arr[current_black_row][current_black_col - 1];
            (*p)->puzzle_arr[current_black_row][current_black_col - 1] = (*p)->puzzle_arr[current_black_row][current_black_col];
            (*p)->puzzle_arr[current_black_row][current_black_col] = temp; 
            (*p)->black_col = current_black_col - 1;
            return 0;
        default:
            fprintf(stderr, "Invalid input");
            return 1;
    }
    return 0;
}


Puzzle *create_copy (Puzzle *orig_puzz) {
    int puzz_size = orig_puzz->size;
    Puzzle *new_copy = puzzle_create(puzz_size);

    //copying fields from orignal to the copy
    new_copy->size = puzz_size;
    new_copy->black_row = orig_puzz->black_row;
    new_copy->black_col= orig_puzz->black_col;
    for (int i = 0; i < puzz_size; i++) {
        for (int j = 0; j < puzz_size; j++) {
        new_copy->puzzle_arr[i][j] = orig_puzz->puzzle_arr[i][j];
        }
    }
    return new_copy;
}


int handle_V_command (Puzzle *p, char steps[], int max_steps, int cur_steps, char last_command) { 
    int returned_value = 0;
    char possible_directions[] = {'u', 'd', 'l', 'r'};
    char current_direction  = '\0';
    
    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    
    if (modified_K_command(p) == true) { //checking if puzzle is solved
        return cur_steps; // steps array has a complete sequence of steps
    } 
    
    if (cur_steps >= max_steps) {
        return -1; // exit function if max-steps has been reached
    }

    for (int i = 0; i < 4; i++) {
        current_direction = possible_directions[i];
        Puzzle *temp_puzz = create_copy(p);
        if (last_command == possible_directions[i]) {
            continue;
        } else if (modified_S_command(current_direction, &temp_puzz) == 0) {
            returned_value = handle_V_command(temp_puzz, steps, max_steps, cur_steps + 1, last_command);
            if (returned_value != -1) {
                steps[cur_steps] = possible_directions[i];
                (cur_steps)++;
                last_command = possible_directions[i];
                puzzle_destroy(temp_puzz);
                return cur_steps;
            }
        }
        puzzle_destroy(temp_puzz);
    }
    return -1;
} 

int printing_V_results(char steps[], int max_index) {
    for (int i = 0; i < max_index; i++) {
        printf("S %c\n", steps[i]);
    }
    return 0;
}


