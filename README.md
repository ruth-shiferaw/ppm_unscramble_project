# ppm_unscramble_project
This is a university project I worked on (with another CS student to shorten the time frame) to implement a program to perform operations on a 15 Puzzle (a puzzle with a square grid of tiles with one tile missing). The tiles can be moved around by sliding the “gap” tile. The goal is to rearrange the tiles so that each one is in the correct position. 

Here is the description of the program's functionality:
The C command creates the initial instance of the puzzle. The size argument must be an integer in the range 2 to 20, inclusive, which specifies the number of rows and columns. Each position in the puzzle must be initialized with the value of the “gap”. This means that the puzzle is not “valid” until it has been explicitly initialized with the T command.

The T command initializes the puzzle by assigning either a tile number or the gap to each position. The tile… argument is a sequence of integers in the range 0…N2
 (inclusive) where N
 is the number of rows/columns with the value 0
 representing the gap. The numbers specify the puzzle configuration in row major order. There must be exactly N2
 tile numbers. The T command must be specified after the C command creating the puzzle.

The I command reads a PPM file from the specified filename. This iamge will be used as the “background” image, and is the basis for the rendering of the current puzzle configuration as an image performed by the W command.

The P command prints the current puzzle configuration to stdout as a sequence of integer values. The format is exactly the same as the tile… sequence specified in the T command, but should reflect the current puzzle configuration based on any S commands that have been executed to update the configuration of the puzzle. The output should be printed as a single line of text ending with a newline (\n) character.

The W command writes two output files, their names being specified by the filename1 and filename2 arguments. The first filename is the name of a PPM image file to be created as a rendering of the current puzzle configuration. The gap should be rendered as consisting of exclusively black pixels (with the red, green, and blue color components being set to 0.) The other tiles should be copied from the appropriate rectangular region of the background image loaded previously with the I command. The background image is expected to have width and height which are both exact multiples of the number of rows/columns in the puzzle. The second filename is the name of a text file to write with the current puzzle configuration, in exactly the same format as the output written to stdout by the P command.

The S command slides the “free” tile in the direction specified by the dir argument, which is one of the characters u (up), d (down), l (left), or r (right). The “free” tile to be moved is the one adjacent to the single gap position that is free to move into the gap. As an illustration of the behavior of the S command, consider the following puzzle configuration
