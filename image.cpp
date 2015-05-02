/*---------------------------------------------------------------------------*\
 *  image.cpp                                                                *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Defines the methods for the Image class.                                 *
\*---------------------------------------------------------------------------*/
#include <iostream>
#include "image.h"
using namespace std;


static const unsigned DEFAULT_SIZE = 10;


/*  Default constructor sets size to 0x0.
 */
Image::Image(void)
{
    Image(0, 0);
}


/*  Overloaded constructor sets size based on parameters.
 */
Image::Image(unsigned h, unsigned w)
{
    set_height(h);
    set_width(w);
}


/*  clear()
 *  Purpose:  Fills the image with blank characters
 */
void Image::clear(void)
{
    set_all(EMPTY);
}


/*  display()
 *  Purpose:  Prints the image to the given output stream, each row on a
 *            single line.
 */
void Image::display(ostream &output)
{
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            output << board[row][col];
        }
        output << endl;
    }
}


/*  update_at()
 *  Purpose:  Places the given character at the specified coordinates.
 *  Notes:  - Uses 0-based indexing for both row and column.
 *          - If the row or column are out-of-bounds, they will "wrap".  So,
 *              for example, if in a 10x10 board, the coordinates 10 and 15 are
 *              used, the update will happen to position 0, 5.
 */
void Image::update_at(unsigned row, unsigned col, char c)
{
    board[row % height][col % width] = c;
}


/*  set_all()
 *  Purpose:  Fills the image with the given character.
 */
void Image::set_all(char c)
{
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            board[row][col] = c;
        }
    }
}


/*  at()
 *  Purpose:  Returns the character at the given coordinates.
 *  Notes:  - If the coordinates are out-of-bounds, the function will "wrap,"
 *            just as in update_at().
 */
char Image::at(unsigned row, unsigned col)
{
    return board[row % height][col % width];
}


/*  set_height()
 *  Purpose:  Sets the height of the image to the given height.
 *  Notes:  - If the new height is smaller than the old height, the image is
 *            truncated and characters on the bottom are lost.
 *          - If the new height is larger than the old height, the image is
 *            expanded with empty characters placed in the new positions.
 */
void Image::set_height(unsigned h)
{
    height = h;
    board.resize(height);
    set_width(width);
}


/*  set_width()
 *  Purpose:  Sets the width of the image to the given width.
 *  Notes:  - If the new width is smaller than the old width, the image is
 *            truncated and characters on the edge are lost.
 *          - If the new width is larger than the old width, the image is
 *            expanded with empty characters placed in the new positions.
 */
void Image::set_width(unsigned w)
{
    width = w;
    for (unsigned row = 0; row < height; ++row) {
        board[row].resize(width, EMPTY);
    }
}


/*  get_height()
 *  Purpose:  Returns the current height of the image.
 */
unsigned Image::get_height(void)
{
    return height;
}


/*  get_width()
 *  Purpose:  Returns the current width of the image.
 */
unsigned Image::get_width(void)
{
    return width;
}
