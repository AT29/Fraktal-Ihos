#include "Scenes.h"

int main(int argc, char* argv[])
{
    Scenes scene;

    PathContainer paths;
    paths = scene.theme();

    std::string song;
    song = scene.menu(paths.path1);

    if(song != "")
        scene.fractal(song, paths.path2);

    return 0;
}