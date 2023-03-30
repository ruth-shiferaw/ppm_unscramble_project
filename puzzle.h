//Ruth Shiferaw <RSHIFER1>
//Kalista Fong <kfong4>
#ifndef PUZZLE_H
#define PUZZLE_H
#include <stdbool.h>

#include "ppm_io.h" // for Image data type

/** A  struct used to bundle together the information needed for a Puzzle. */
typedef struct {
  int size; //stores the specified puzzle size
  int **puzzle_arr; //a 2D array to store the tile values within a puzzle
  int black_row; //stores the row number for the black (gap) tile
  int black_col; //stores the column number for the black (gap) tile
} Puzzle;

/** A function that destroys a puzzle by freeing all allocated memory for a Puzzle instance.
 * @param p the puzzle that needs to be freed */
void puzzle_destroy(Puzzle *p);

/** A function that destroys an image by freeing all allocated memory for an Image instance.
 * @param imge the image that needs to be freed */
void image_destroy(Image *imge);

/** A function that returns the value of the tile at a Puzzle's specified index.
 * @param p the puzzle the tile value will be extracted from
 * @param col the column number of the tile
 * @param row the row number of the tile */
int puzzle_get_tile(const Puzzle *p, int col, int row);

/** A function that creates, allocates memory for, and returns a puzzle instance.
 * @param size the desired size of the new puzzle */
Puzzle *puzzle_create(int size);

/** A function that creates an initial puzzle with the specified size rows and columns.
 * @param in the specified .txt input file
 * @param p the puzzle instance that is updated with the newly attained size, row, and column information */
int handle_C_command(FILE *in, Puzzle **p);

/** A function that initialize the puzzle's configuration with a specified series of tile numbers.
 * @param in the specified .txt input file the series of tile numbers are written in
 * @param p the puzzle that will be updated with the new tile numbers */
int handle_T_command(FILE *in, Puzzle **p);

/** A function that loads the background image from a specified PPM file.
 * @param in the specified .txt input file the PPM filename is written in
 * @param mainimg the Image instance that will be updated with the information within the PPM file*/
int handle_I_command(FILE *in, Image **mainimg);

/** A function that prints a sequence of tile numbers reflecting current puzzle configuration.
 * @param p the puzzle whose tile configuration will be printed */
int handle_P_command(Puzzle *p);

/** A function that writes a puzzle's image to a .ppm file and the puzzle's configuration to a .txt file.
 * @param in the specified .txt input file the .ppm and .txt filenames are stored in
 * @param p the puzzle instance whose configuration will be printed into the .txt file
 * @param image the image instance whose information will be written into a .ppm file */
int handle_W_command(FILE *in, Puzzle *p, Image *image);

/** A function that checks to see whether the puzzle is in the correct configuration.
 * @param p the puzzle whose tile configuration will be checked */
int handle_K_command(Puzzle *p);

/** A function that creates a series of steps to solve a puzzle.
 * @param p the original puzzle
 * @param puzz_copy a copy of the original puzzle
 * @param steps the array of directions that stores the solution's steps
 * @param max_steps the maximum number of steps to try
 * @param cur_steps how many steps have been considered so far */
int handle_V_command (Puzzle *p, char steps[], int max_steps, int cur_steps, char last_command);

/** A function that slides a free tile in a specified direction.
 * @param in the specified .txt input file
 * @param p the original puzzle */
int handle_S_command(FILE *in, Puzzle *p);

/** A function that is a modified version of handle_S_command(), and is meant for taking 
 * different parameters but achieving the same goal as handle_S_command.
 * @param desired_direction the specified direction to move the gap in the puzzle
 * @param p the puzzle the desired_direction needs to occur in */
int modified_S_command(char desired_direction, Puzzle **p);

/** A function that creates a copy of a puzzle.
 * @param orig_puzz the original puzzle that needs to be copied */
Puzzle *create_copy(Puzzle *orig_puzz);

/** A function that prints the results of the function responsible for the V command.
 * @param steps the char array that stores the solutions found by the V command */
int printing_V_results(char steps[], int max_index);

/** A modified version of the K command function. modified_K_command is modified because
 * it does not have any printf statements, and the return type is bool. It allows for
 * handle_V_command to function
 * @param p the puzzle being compared to winning configuration */
bool modified_K_command(Puzzle *p);

#endif // PUZZLE_H
