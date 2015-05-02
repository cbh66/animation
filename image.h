/*---------------------------------------------------------------------------*\
 *  image.h                                                                  *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Defines a class for holding an ASCII image; that is, a 2-D array of      *
 *    characters.  Allows for setting the size of the image, placing and     *
 *    retrieving characters, setting all characters to something, and        *
 *    displaying the image to an output stream.                              *
 *                                                                           *
 *  TO DO:                                                                   *
 *  - Use template parameters instead of characters, to allow for having     *
 *      images of different kinds of things too (like RGB values).           *
 *  - Would need to generalize what EMPTY means; maybe that should be client *
 *    side instead, and instead you just use the default, like vector<>.     *
 *  - Also overload << instead of using print()                              *
\*---------------------------------------------------------------------------*/
#ifndef IMAGE_H_
#define IMAGE_H_
#include <vector>
#include <fstream>

class Image
{
public:
    Image(void);
    Image(unsigned h, unsigned w);

    void clear(void);
    void display(std::ostream &output);
    void set_all(char c);
    char at(unsigned row, unsigned col);
    void update_at(unsigned row, unsigned col, char c);

    void set_height(unsigned h);
    void set_width(unsigned w);
    unsigned get_height(void);
    unsigned get_width(void);

private:
    static const char EMPTY = ' ';
    unsigned height, width;
    std::vector< std::vector<char> > board;
};

#endif
