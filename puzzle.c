//Ruth Shiferaw <RSHIFER1>
//Kalista Fong <kfong4>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"
#include <stdbool.h>

int main(int argc, char **argv) {
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }

  char current_file_command = '\0';

    FILE *input;
    if (argv[1] != NULL) { //checking if file name is provided
      input = fopen(argv[1], "r");
      if ((input == NULL) || (ferror(input) != 0)) { //checks if file is empty or in an error state
        fprintf(stderr, "Invalid input file provided.\n");
        return 1;
      }
    } else { //if filename is not specified, read from stdin
      input = stdin;
    }
    Puzzle *main_puzz = NULL; //initializing puzzle
    Image *main_image = NULL; // intializing image
    //variables needed for v command:
    char possible_steps[200];
    int current_steps = 0;
    int maximum_steps = 100;
    char previous_command = '\n';

    while((fscanf(input, " %c", &current_file_command)) == 1) {
      switch(current_file_command) {
        case 'C':
          if (handle_C_command(input, &main_puzz) == 1) {
            return 1;
          }
          break;
        case 'T':
          if (handle_T_command(input, &main_puzz) == 1) {
            return 1;
          }
          break;
        case 'I' :
          if (handle_I_command(input, &main_image) == 1) {
            return 1;
          }
          break;
        case 'W' :
          if (handle_W_command(input, main_puzz, main_image) == 1) {
            return 1;
          }
          break;
        case 'S':
          if (handle_S_command(input, main_puzz) == 1) {
            return 1;
          }
          break;
        case 'P':
          if (handle_P_command(main_puzz) == 1) {
            return 1;
          }
          break;
        case 'K':
          if (handle_K_command(main_puzz) == 1) {
            return 1;
          }
          break;
        case 'V' : 
          current_steps = handle_V_command(main_puzz, possible_steps, maximum_steps, current_steps, previous_command);
          if (current_steps == -1) { 
            printf("No solution found\n");
            return 1;
          } else {
            printing_V_results (possible_steps, current_steps);
          }
          break;
        case 'Q' :
          fclose(input);
          fclose(stdout);
          fclose(stdin);
          fclose(stderr);
          puzzle_destroy(main_puzz);
          image_destroy(main_image);
          return 0;
          break; 
        default:
          fprintf(stderr, "Invalid command '%c'\n", current_file_command); 
          return 1;
          break;
      }
    }

    fclose(input);
    fclose(stdout);
    fclose(stdin);
    fclose(stderr);
    puzzle_destroy(main_puzz);
    image_destroy(main_image);

    return 0;

} //end main
