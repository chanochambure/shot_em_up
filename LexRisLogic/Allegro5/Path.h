/* Path.h -- Path Allegro 5 Header - LexRis Logic Headers

    Copyright (c) 2016 LexRisLogic

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so.

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of
    the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
    THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef INCLUDED_LL_AL5_PATH_H
#define INCLUDED_LL_AL5_PATH_H

#include <string>

namespace LL_AL5
{
    std::string get_current_directory()
    {
        char* actual_dir=al_get_current_directory();
        std::string return_dir;
        if(actual_dir)
        {
            return_dir=actual_dir;
            al_free(actual_dir);
        }
        return return_dir;
    }

    bool change_directory(std::string new_path)
    {
        return al_change_directory(new_path.c_str());
    }

    bool make_directory(std::string new_dir)
    {
        return al_make_directory(new_dir.c_str());
    }

    bool path_exists(std::string path)
    {
        return al_filename_exists(path.c_str());
    }

    bool remove_path(std::string path)
    {
        return al_remove_filename(path.c_str());
    }
}

#endif // INCLUDED_LL_AL5_PATH_H
