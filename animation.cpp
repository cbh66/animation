/*---------------------------------------------------------------------------*\
 *  animation.cpp                                                            *
 *  Version:  0.0.1                                                          *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  A program for running simple ASCII animations in terminal.               *
 *  Provide, on the command line, the names of files with appropriate        *
 *    animation information.  The first file should begin with the line      *
 *             CANVAS height width                                           *
 *    where height and width are integers representing the size of the scene *
 *    to be animated, in characters.                                         *
 *  Subsequently, files should contain a sequence of sprites.  The           *
 *    information for each sprite should begin with a line of the form       *
 *             SPRITE height width row col v-speed h-speed frames framerate  *
 *    where height and width are the integer dimensions of the picture;      *
 *          row and col are the initial position of the sprite in the image, *
 *             and may contain a decimal;                                    *
 *          v-speed and h-speed are the vertical and horizontal speed of the *
 *             sprite in the image, in characters per frame, which may       *
 *             contain a decimal;                                            *
 *          frames are the number of frames in the animation cycle of the    *
 *             sprite, and must be an integer;                               *
 *          framerate is the number of frames it should take to complete the *
 *             sprite's animation cycle, and must be an integer.             *
 *    Following this line should be a sequence of images representing what   *
 *      each frame of the sprite's animation should look like.               *
 *                                                                           *
 *  TO DO:                                                                   *
 *  - Allow for additional control sequences in a file, eg. instead of just  *
 *    CANVAS and SPRITE, allow OPTION followed by some sequence              *
 *  - Such options should include single-step or not, FPS, bounce or wrap.   *
 *  - Allow command-line options, which would override options in the file   *
 *  - Process user commands as things are running.  Perhaps allow users to   *
 *    select certain sprites and, eg, change their speed or position.        *
\*---------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "termfuncs.h"
#include "image.h"
#include "sprite.h"
using namespace std;


static bool SINGLE_STEP = false;
static const char QUIT = 'q';
static unsigned FPS = 10;
static unsigned USECS_PER_SEC = 1000000;


vector<Sprite> read_in(int size, char *files[], Image *canvas);
void run_animation(Image *canvas, vector<Sprite> sprites);


int main(int argc, char *argv[])
{
    Image canvas;
    if (argc < 2) {
        cerr << "Please provide at least one filename." << endl;
        return 1;
    }
    vector<Sprite> sprites = read_in(argc - 1, argv + 1, &canvas);
    run_animation(&canvas, sprites);
    return 0;
}


/*  read_in()
 *  Purpose:  Reads information from the given list of files.  Creates an array
 *            of sprites, and updates the canvas and program options to reflect
 *            what is read.
 *  Parameters: The number of files to read, and an array of their names.
 *            A pointer to the canvas, whose size may be modified.
 *  Returns:  A vector of the sprites read in.
 *  Notes:    Prints to cerr when a given file cannot be opened, but does not
 *            abort.
 *            Exits the program when the first file does not begin with valid
 *            canvas information.  This may be changed to make things more
 *            flexible.
 */
vector<Sprite> read_in(int size, char *files[], Image *canvas)
{
    unsigned height, width;
    string first;
    vector<Sprite> sprites;
    for (int i = 0; i < size; ++i) {
        ifstream input;
        input.open(files[i]);
        if (!input.is_open()) {
            cerr << "Could not open file \"" << files[i] << "\"" << endl;
            continue;
        }
        if (i == 0) {
            if (!(input >> first) || first != "CANVAS" ||
                !(input >> height >> width)) {
                cerr << "First file, \"" << files[i] << "\", should begin "
                     << "with canvas information." << endl;
                exit(1);
            }
            canvas->set_height(height);
            canvas->set_width(width);
        }
        Sprite current;
        while (current.read_in(input)) {
            sprites.push_back(current);
            current = Sprite();
        }
        input.close();
    }
    return sprites;
}


/*  run_animation()
 *  Purpose:  To show the animation on cout, with the given canvas and sprites
 *  Parameters: A pointer to a canvas to use, which will be modified over
 *            the course of the animation.  A vector of sprites to use.
 *  Notes:    Runs until the user presses the QUIT character.  Changes behavior
 *            based on SINGLE_STEP.  When it is false, uses FPS to control the
 *            frame rate.
 */
void run_animation(Image *canvas, vector<Sprite> sprites)
{
    unsigned height = canvas->get_height();
    unsigned width = canvas->get_width();
    unsigned num_sprites = sprites.size();
    char c = '\0';
    screen_clear();
    do {
        screen_home();
        canvas->clear();
        for (unsigned i = 0; i < num_sprites; ++i) {
            sprites[i].draw_to(canvas);
            sprites[i].advance(height, width);
        }
        canvas->display(cout);
        if (SINGLE_STEP) {
            c = getachar();
        } else {
            c = getacharnow(0);
            usleep(USECS_PER_SEC / FPS);
        }
    } while (c != QUIT);
}

