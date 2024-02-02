#include <imw.hpp>
#include "MainWindow.hpp"


int main(int, char **)
{
    imw::Application::init(MainWindow{});  // <--- This is the only line of code that is needed to change the window implementation
    imw::Application::run();

    return 0;
}

