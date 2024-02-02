#include <imw.hpp>
#include "MainWindow.hpp"


int main(int, char **)
{
    auto window = MainWindow{"MyWindow"};
    window.setFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    window.setSize(ImVec2(1280.0F, 720.0F));
    window.setPos(ImVec2(0.0F, 0.0F));

    imw::Application::init(window);  // <--- This is the only line of code that is needed to change the window implementation
    imw::Application::run();

    return 0;
}

