/*---------------------------------------------------------------------------*\
 *  image.h                                                                  *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Defines a class for holding an image; that is, a 2-D array of some kind  *
 *    of characters.  Allows for setting the size of the image, placing and  *
 *    retrieving characters, setting all characters to something, and        *
 *    displaying the image to an output stream.                              *
 *                                                                           *
 *  The class is defined with template parameters, to generalize what is     *
 *    meant by "characters".  This way, this class could also hold an image, *
 *    for example, of some sort of Pixel type.                               *
\*---------------------------------------------------------------------------*/
#ifndef IMAGE_H_
#define IMAGE_H_
#include <vector>
#include <fstream>
#include <string>

template <typename T>
class Image
{
public:
    Image(void);
    Image(unsigned h, unsigned w);

    void display(std::ostream &output) const;
    void read_in(std::istream &input);

    void set_all(T c);
    T at(unsigned row, unsigned col) const;
    void update_at(unsigned row, unsigned col, T c);

    void set_height(unsigned h);
    void set_width(unsigned w);
    unsigned get_height(void) const;
    unsigned get_width(void) const;

private:
    unsigned height, width;
    std::vector< std::vector<T> > board;
};


/*  Default constructor sets size to 0x0.
 */
template <typename T>
inline Image<T>::Image(void)
{
    Image(0, 0);
}


/*  Overloaded constructor sets size based on parameters.
 */
template <typename T>
inline Image<T>::Image(unsigned h, unsigned w)
{
    set_height(h);
    set_width(w);
    //  Note clearing happens automatically when these are set.
}


/*  read_in()
 *  Purpose:  Reads in and initializes the image from the given input stream
 *  Notes:  - The input stream should begin on the same line as the start
 *            of the image.
 *          - Lines in the image that are too short will be padded with empty
 *            characters, while lines that are too long will be truncated.
 *          - However, exactly the expected number of lines should be
 *            available, or unexpected display errors could occur.  In
 *            particular, the last readable line will likely be repeated.
 */
template <>
inline void Image<char>::read_in(std::istream &input)
{
    std::string line;
    for (unsigned row = 0; row < height; ++row) {
        unsigned line_width = width;
        getline(input, line);
        if (line.length() < line_width) line_width = line.length();
        for (unsigned col = 0; col < line_width; ++col) {
            board[row][col] = line[col];
        }
    }
}


/*  read_in()
 *  The general version for non-char types uses the >> operator, and cannot
 *    in general check for lines being too short or too long
 */
template <typename T>
inline void Image<T>::read_in(std::istream &input)
{
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            input >> board[row][col];
        }
    }
}


/*  >> operator provided for convenience simply calls the read_in() method
 */
template <typename T>
inline std::istream &operator>>(std::istream &input, Image<T> &img)
{
    img.read_in(input);
    return input;
}


/*  display()
 *  Purpose:  Prints the image to the given output stream, each row on a
 *            single line.
 *  Notes:  - Uses the << operator to print each element, and prints a newline
 *            after each row.
 */
template <typename T>
inline void Image<T>::display(std::ostream &output) const
{
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            output << board[row][col];
        }
        output << std::endl;
    }
}


/*  << operator provided for convenience simply calls the display() method
 */
template <typename T>
inline std::ostream &operator<<(std::ostream &output, Image<T> const &img)
{
    img.display(output);
    return output;
}


/*  update_at()
 *  Purpose:  Places the given character at the specified coordinates.
 *  Notes:  - Uses 0-based indexing for both row and column.
 *          - If the row or column are out-of-bounds, they will "wrap".  So,
 *              for example, if in a 10x10 board, the coordinates 10 and 15 are
 *              used, the update will happen to position 0, 5.
 */
template <typename T>
inline void Image<T>::update_at(unsigned row, unsigned col, T c)
{
    board[row % height][col % width] = c;
}


/*  set_all()
 *  Purpose:  Fills the image with the given character.
 */
template <typename T>
inline void Image<T>::set_all(T c)
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
template <typename T>
inline T Image<T>::at(unsigned row, unsigned col) const
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
template <typename T>
inline void Image<T>::set_height(unsigned h)
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
template <typename T>
inline void Image<T>::set_width(unsigned w)
{
    width = w;
    for (unsigned row = 0; row < height; ++row) {
        board[row].resize(width);
    }
}


/*  get_height()
 *  Purpose:  Returns the current height of the image.
 */
template <typename T>
inline unsigned Image<T>::get_height(void) const
{
    return height;
}


/*  get_width()
 *  Purpose:  Returns the current width of the image.
 */
template <typename T>
inline unsigned Image<T>::get_width(void) const
{
    return width;
}


#endif
/* IMAGE_H_ */
