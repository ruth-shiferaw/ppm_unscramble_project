//Ruth Shiferaw <RSHIFER1>
//Kalista Fong <kfong4>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"

// also tests puzzle_destroy
void test_puzzle_create(void) {

  Puzzle *p3 = puzzle_create(3);
  Puzzle *p5 = puzzle_create(5);

  assert(p3->size == 3);
  for (int i = 0; i < 9; ++i) {
    assert(p3->puzzle_arr[i] == 0);
  }

  assert(p5->size == 5);
  for (int i = 0; i < 25; ++i) {
    assert(p5->puzzle_arr[i] == 0);
  }

  puzzle_destroy(p3);
  puzzle_destroy(p5);
}

// also tests puzzle_get_tile and puzzle_destroy?
void test_puzzle_get_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

  Puzzle *p4 = puzzle_create(4);
  // for (int i = 0; i < 4; ++i)
  //   for (int j = 0; j < 4; ++j)
  //     puzzle_set_tile(p4, j, i, init[i][j]);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      assert(puzzle_get_tile(p4, j, i) == init[i][j]);

  puzzle_destroy(p4);
}

void test_handle_C_command() {
  FILE* input = fopen("test01.txt", "r");
  Puzzle *testC_puzz = NULL;
  handle_C_command(input, &testC_puzz);
  assert((testC_puzz->size) == 4);
  puzzle_destroy(testC_puzz);

  FILE* fake_input = fopen("fileDNE.txt", "r");
  Puzzle *failC_puzz = NULL;
  assert((handle_C_command(fake_input, &failC_puzz)) == 1);
  puzzle_destroy(failC_puzz);
}

void test_handle_T_command () {
  FILE *input = fopen("test02.txt", "r");
  Puzzle *testT_puzz = NULL;
  handle_C_command(input, &testT_puzz);
  assert((handle_T_command(input, &testT_puzz)) == 0);

  FILE *fake_input = fopen("fake.txt", "r");
  assert((handle_T_command(fake_input, &testT_puzz)) == 1);

  FILE *bad_input = fopen("bad.txt", "w+");
  fprintf(bad_input, "C 4\n T 12 1 2 15 11 6 5 L 7 10 9 4 0 13 14 3\n");
  handle_C_command(bad_input, &testT_puzz);
  assert((handle_T_command(bad_input, &testT_puzz)));

} 
  

/*void test_handle_I_command();

int test_handle_P_command(Puzzle *p);

int test_handle_W_command(FILE *in, Puzzle *p, Image *image);

int test_handle_K_command(Puzzle *p);

int test_handle_V_command (Puzzle *p, Puzzle **puzz_copy, char *steps[], int max_steps, int *cur_steps);

int test_handle_S_command(FILE *in, Puzzle *p);

int test_modified_S_command(char desired_direction, Puzzle **p);

int test_create_copy(Puzzle *orig_puzz);

int test_printing_V_results(char *steps[], int max_index);

int test_modified_K_command(Puzzle *p); */


int main(void) {

  test_handle_C_command();
  test_handle_T_command();

  test_puzzle_create();
  //test_puzzle_set_tile();
  // ...call other test functions...
  /*
  int test_puzzle_destroy(Puzzle *p);

  test_image_destroy(Image *imge);

  test_handle_I_command(FILE *in, Image **mainimg);

  test_handle_P_command(Puzzle *p);

  test_handle_W_command(FILE *in, Puzzle *p, Image *image);

  test_handle_K_command(Puzzle *p);

  test_handle_V_command (Puzzle *p, Puzzle **puzz_copy, char *steps[], int max_steps, int *cur_steps);

  test_handle_S_command(FILE *in, Puzzle *p);

  test_modified_S_command(char desired_direction, Puzzle **p);

  test_create_copy(Puzzle *orig_puzz);

  test_printing_V_results(char *steps[], int max_index);

  test_modified_K_command(Puzzle *p);
  
  */

  printf("All tests passed!\n");
  return 0;
}
