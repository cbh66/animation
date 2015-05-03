/*---------------------------------------------------------------------------*\
 *  animation.cpp                                                            *
 *  Version:  0.0.2                                                          *
 *  Written by: Colin Hamilton, Tufts University                             *
 *                                                                           *
 *  Inspired by the Tufts COMP11 second project, in use 2013-2015.           *
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
 *  - Options should include bounce or wrap.                                 *
 *  - Option to stop animation after a certain number of frames (or seconds) *
 *  - Allow command-line options, which would override options in the file   *
 *  - Process user commands while things are running.  Perhaps allow users   *
 *    to select certain sprites and, eg, change their speed or position.     *
 *  - Attach a layer to each frame, then sort them by layer before drawing.  *
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
static unsigned FPS = 30;
static const unsigned USECS_PER_SEC = 1000000;


string toupper(string s);
vector<Sprite> read_in(int size, char *files[], Image<char> *canvas);
void process_file(istream &input, vector<Sprite> *sprites,
                  Image<char> *canvas);
void run_animation(Image<char> *canvas, vector<Sprite> sprites);


int main(int argc, char *argv[])
{
    Image<char> canvas;
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
 *  Notes:  - Prints to cerr when a given file cannot be opened, but does not
 *            abort.
 */
vector<Sprite> read_in(int size, char *files[], Image<char> *canvas)
{
    vector<Sprite> sprites;
    for (int i = 0; i < size; ++i) {
        ifstream input;
        input.open(files[i]);
        if (!input.is_open()) {
            cerr << "Could not open file \"" << files[i] << "\"" << endl;
            continue;
        }
        process_file(input, &sprites, canvas);
        input.close();
    }
    return sprites;
}


/*  toupper()
 *  Purpose:  Converts a string to all uppercase.
 *  Parameters:  The string to make uppercase
 *  Returns:  The uppercase'd string
 */
string toupper(string s)
{
    unsigned size = s.length();
    for (unsigned i = 0; i < size; ++i) {
        s[i] = toupper(s[i]);
    }
    return s;
}


/*  process_file()
 *  Purpose:  Reads data from a given stream and updates data according to
 *            the instructions therein.
 *  Parameters:  A reference to the stream to read from.  Pointers to the
 *            vector of sprites and the canvas, both of which may be modified.
 *  Notes:  - Handles program settings by currently setting global variables.
 *            May change that to take in some sort of Settings object.
 */
void process_file(istream &input, vector<Sprite> *sprites, Image<char> *canvas)
{
    string first;
    while (input >> first) {
        first = toupper(first);
        if (first == "CANVAS") {
            unsigned height, width;
            if (input >> height >> width) {
                canvas->set_height(height);
                canvas->set_width(width);
            }
        } else if (first == "SPRITE") {
            Sprite current;
            if (input >> current) {
                sprites->push_back(current);
            }
        } else if (first == "FPS") {
            input >> FPS;
        } else if (first == "SINGLE-STEP") {
            SINGLE_STEP = true;
        } else if (first == "CONTINUOUS") {
            SINGLE_STEP = false;
        }
    }
}


/*  run_animation()
 *  Purpose:  To show the animation on cout, with the given canvas and sprites
 *  Parameters: A pointer to a canvas to use, which will be modified over
 *            the course of the animation.  A vector of sprites to use.
 *  Notes:  - Runs until the user presses the QUIT character.  Changes behavior
 *            based on SINGLE_STEP.  When it is false, uses FPS to control the
 *            frame rate.
 */
void run_animation(Image<char> *canvas, vector<Sprite> sprites)
{
    unsigned height = canvas->get_height();
    unsigned width = canvas->get_width();
    unsigned num_sprites = sprites.size();
    char c = '\0';
    screen_clear();
    do {
        screen_home();
        canvas->set_all(' ');
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

