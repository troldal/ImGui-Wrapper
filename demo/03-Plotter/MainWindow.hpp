#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <set>
#include <string_view>
#include "Plot.hpp"
#include <imw.hpp>

#include <implot.h>

class MainWindow : public imw::WindowBase< MainWindow >
{
    using BASE = imw::WindowBase< MainWindow >;
    friend BASE;

    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos  = ImVec2(0.0F, 0.0F);

    bool sinSelected = false;
    bool cosSelected = false;

    Plot plot { "###plot" };

public:
    using BASE::BASE;

    explicit MainWindow(std::string_view windowName)
        : BASE(windowName) {};

private:
    void exec()
    {
        if (ImGui::Checkbox("sin(x)", &sinSelected))
            plot.setSinSelected(sinSelected);

        if (ImGui::Checkbox("cos(x)", &cosSelected))
            plot.setCosSelected(cosSelected);

        plot.render();
    }
};
