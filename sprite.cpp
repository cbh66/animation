/*---------------------------------------------------------------------------*\
 *  sprite.cpp                                                               *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Defines the methods for the Sprite class.                                *
\*---------------------------------------------------------------------------*/
#include <iostream>
#include <cmath>
#include "sprite.h"
using namespace std;



/*  Default constructor sets all values to zero.
 */
Sprite::Sprite(void)
{
    height = width = 0;
    row_pos = col_pos = 0;
    v_speed = h_speed = 0;
    frame_rate = 0;
    current_frame = 0;
    // Default vector constructor ensures board is an empty 2-D vector
}


/*  read_in()
 *  Purpose:  Reads the information for the sprite from the given input
 *            stream, and initializes the sprite accordingly.
 *  Returns:  True if successful, false if not.  Reading could be unsuccessful
 *            if information is missing, incomplete, or invalid.
 *  Notes:  - An entry is expected to begin with a line containing the height,
 *            width, initial row, initial column, vertical speed,
 *            horizontal speed, number of frames, and frames per animation
 *            cycle.  Then each of the frames (which is an image),
 *            one at a time.
 *          - Lines in the image that are too short will be padded with empty
 *            characters, while lines that are too long will be truncated.
 *          - However, each frame should have exactly the expected number of
 *            lines, or unexpected display errors could occur.
 *          - Each frame should immediately follow the previous one, with no
 *            padding or extra lines between them.
 *          - If this function returns false, the Sprite's data will be
 *            unchanged
 */
bool Sprite::read_in(istream &input)
{
    string line;
    unsigned h, w;
    double r, c, v_s, h_s;
    unsigned num_frames;
    double frames_per_cycle;
    if (!(input >> h >> w >> r >> c) || r < 0 || c < 0) {
        return false;
    }
    if (!(input >> v_s >> h_s >> num_frames >> frames_per_cycle)) {
        return false;
    }
    getline(input, line);    // Ignore remainder of line; advance to the
    set_height(h);           // next line in input
    set_width(w);
    row_pos = r;
    col_pos = c;
    v_speed = v_s;
    h_speed = h_s;
    frame_rate = (frames_per_cycle == 0) ? 0 : num_frames / frames_per_cycle;
    for (unsigned f = 0; f < num_frames; ++f) {
        if (!(input.good())) {
            frames.resize(frames.size() - f);  // Remove frames added thus far
            return false;
        }
        Image<char> next_frame(height, width);
        next_frame.set_all(' ');
        input >> next_frame;
        add_frame(next_frame);
    }
    return true;
}


/*  display()
 *  Purpose:  Prints the Sprite to the given output stream, where the sprite's
 *            image is determined by its current frame.  Note the Sprite's
 *            position is not taken into account; for that, draw the sprite
 *            into a larger image and print that.
 */
void Sprite::display(std::ostream &output) const
{
    output << frames[current_frame];
}


/*  add_frame()
 *  Purpose:  Adds the given image to the Sprite's frame cycle.  The image
 *            is placed at the end of the cycle.
 */
void Sprite::add_frame(Image<char> new_frame)
{
    frames.push_back(new_frame);
}


/*  wrap()
 *  Purpose:  A helper function that "wraps" a given number so that it is
 *            within the given interval.
 */
static double wrap(double number, double min, double max)
{
    number = fmod(number - min, max - min);
    if (number < 0) {
        return number + max;
    }
    return min + number;
}


/*  advance()
 *  Purpose:  Advance the Sprite forward one unit of time, thus potentially
 *            changing its position and current frame.
 *  Parameters: The height and width of the canvas the sprite is moving in,
 *            used to properly wrap the sprite's position if necesarry.
 *  Notes:  - The Sprite's position will wrap as it hits the edges, so that it
 *            will appear on the other side of the canvas.
 *  TO DO:
 *  - Allow a boolean value to determine whether to wrap or bounce off edges.
 *  - Or, even better, have such a boolean "default" within the class, and
 *    use that if none is supplied.  For us to distinguish, maybe have two
 *    different kinds of sprites?  W-SPRITE vs. B-SPRITE (for wrap and bounce)
 *    perhaps?  And also just allow SPRITE for the default, which can be a
 *    program setting.
 */
void Sprite::advance(unsigned canvas_height, unsigned canvas_width)
{
    row_pos = wrap(row_pos + v_speed, 0, canvas_height);
    col_pos = wrap(col_pos + h_speed, 0, canvas_width);
    current_frame = wrap(current_frame + frame_rate, 0, frames.size());
}


/*  draw_to()
 *  Purpose:  Draws the current frame of the Sprite at the appropriate position
 *            in the given image.
 *  Parameters: A pointer to the image to draw the Sprite in, which will be
 *            modified.
 *  Notes:  - Relies on the Image's update_at() function to handle
 *            out-of-bounds values when the Sprite is on the edge of the board.
 */
void Sprite::draw_to(Image<char> *board) const
{
    for (unsigned row = 0; row < height; ++row) {
        for (unsigned col = 0; col < width; ++col) {
            unsigned board_row = row + row_pos;
            unsigned board_col = col + col_pos;
            board->update_at(board_row, board_col,
                             frames[current_frame].at(row, col));
        }
    }
}


/*  set_height()
 *  Purpose:  Sets the height of every frame of the Sprite to the given value.
 *  Notes:  - If the new height is smaller than the old height, the images are
 *            truncated and characters on the bottom are lost.
 *          - If the new height is larger than the old height, the images are
 *            expanded with empty characters placed in the new positions.
 */
void Sprite::set_height(unsigned h)
{
    if (h != height) {
        unsigned size = frames.size();
        for (unsigned f = 0; f < size; ++f) {
            frames[f].set_height(h);
        }
        height = h;
    }
}


/*  set_width()
 *  Purpose: Sets the width of every frame of the Sprite to the given value.
 *  Notes:  - If the new width is smaller than the old width, the images are
 *            truncated and characters on the edge are lost.
 *          - If the new width is larger than the old width, the images are
 *            expanded with empty characters placed in the new positions.
 */
void Sprite::set_width(unsigned w)
{
    if (w != width) {
        unsigned size = frames.size();
        for (unsigned f = 0; f < size; ++f) {
            frames[f].set_width(w);
        }
        width = w;
    }
}


/*  get_height()
 *  Purpose:  Returns the height of the Sprite's image frames.
 */
unsigned Sprite::get_height(void) const
{
    return height;
}


/*  get_width()
 *  Purpose:  Returns the width of the Sprite's image frames.
 */
unsigned Sprite::get_width(void) const
{
    return width;
}


/*  print()
 *  Purely for debugging; prints the sprite's information to cout in a
 *    nicely formatted way that is easy to follow.
 */
void Sprite::print() const
{
    cout << "Size: " << height << " x " << width << endl;
    cout << "Position: (" << row_pos << ", " << col_pos << ")" << endl;
    cout << "Velocity: (" << v_speed << ", " << h_speed << ")" << endl;
    cout << "Frame rate: " << frame_rate << ",   current frame: "
         << current_frame << endl;
    for (unsigned f = 0; f < frames.size(); ++f) {
        cout << "============= FRAME " << f << " ===============" << endl;
        cout << frames[f];
    }
    cout << "======================================" << endl;
}

