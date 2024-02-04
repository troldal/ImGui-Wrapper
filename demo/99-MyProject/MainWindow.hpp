#pragma once
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <cstdint>
#include <string_view>

class MainWindow : public imw::WindowBase< MainWindow >
{
    using BASE = imw::WindowBase< MainWindow >;
    friend BASE;

public:
    std::string_view m_Label = "Label";

    static constexpr auto bufferSize = std::size_t { 1024 };
    static constexpr auto popUpFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popUpSize       = ImVec2(300.0F, 100.0F);
    static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popUpPos        = ImVec2(1280.0F / 2.0F - popUpSize.x / 2.0F, 720.0F / 2.0F - popUpSize.y / 2.0F);

    char        textBuffer[bufferSize];
    std::string currentFilename;

public:
    using BASE::BASE;

    explicit MainWindow(std::string_view windowName)
        : BASE(windowName)
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    void exec()
    {
        constexpr static auto window_flags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos  = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);


        ImGui::BeginChild("SidePanel", ImVec2(300, 600));
        ImGui::SetCursorPosY(100.0);
        ImGui::SetCursorPosX(10.0);
        if (ImGui::Button("Save")) {
            std::cout << "Save button clicked" << std::endl;
        }

        ImGui::SameLine();

        if (ImGui::Button("Load")) {
            std::cout << "Load button clicked" << std::endl;
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear")) {
            std::cout << "Clear button clicked" << std::endl;
        }

        ImGui::SetCursorPosX(10.0);
        static char str0[128] = "Hello, world!";
        ImGui::PushItemWidth(250);
        ImGui::InputText("###input text", str0, IM_ARRAYSIZE(str0));
        ImGui::PopItemWidth();

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::InputTextMultiline("###inputField", textBuffer, bufferSize,  ImVec2(800.0F, 625.0F), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll);

    }

};
