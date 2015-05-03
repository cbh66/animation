/*---------------------------------------------------------------------------*\
 *  sprite.h                                                                 *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Defines the Sprite class, which represents a figure with a position and  *
 *    speed and an animation cycle.  Each part of the animation cycle is a   *
 *    "frame" and is represented as an Image.                                *
 *  A Sprite can have its information read in from an input stream.  It can  *
 *    have frames added to its animation, and it can have its values set and *
 *    read.                                                                  *
 *    The advance() function moves the Sprite forward one unit in            *
 *    time: its position will change based on its speed, and the current     *
 *    frame will also change, based on the frame rate.                       *
 *    Finally, a sprite's current frame can be drawn onto another image at   *
 *    the appropriate position with the draw_to() function.                  *
 *                                                                           *
 * TO DO:                                                                    *
 * - Allow moving around of frames, or at least a remove() function          *
\*---------------------------------------------------------------------------*/
#ifndef SPRITE_H_
#define SPRITE_H_
#include <vector>
#include <fstream>
#include "image.h"

class Sprite
{
public:
    Sprite(void);

    bool read_in(std::istream &input);
    void display(std::ostream &output) const;

    void add_frame(Image<char> new_frame);
    void draw_to(Image<char> *board) const;
    void advance(unsigned canvas_height, unsigned canvas_width);

    void set_height(unsigned h);
    void set_width(unsigned w);
    unsigned get_height(void) const;
    unsigned get_width(void) const;

private:
    void print() const;
    unsigned height, width;
    double row_pos, col_pos;
    double v_speed, h_speed;
    double frame_rate;
    double current_frame;
    std::vector< Image<char> > frames;
};

/*  >> operator provided for convenience.
 *  Calls the read_in() method, and may set the input stream's failbit if
 *    the operation fails.
 */
inline std::istream &operator>>(std::istream &input, Sprite &spr)
{
    if (!spr.read_in(input) && !input.eof()) {
        input.setstate(std::ios::failbit);
    }
    return input;
}


/*  << operator provided for convenience uses the display() method.
 */
inline std::ostream &operator<<(std::ostream &output, Sprite const &spr)
{
    spr.display(output);
    return output;
}

#endif
