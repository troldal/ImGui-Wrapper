#pragma once

#include <cstdint>
#include <string_view>

class MainWindow
{
    std::string_view m_Label = "Label";

public:
    void render() const
    {
        constexpr static auto window_flags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
        constexpr static auto window_pos  = ImVec2(0.0F, 0.0F);

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin(m_Label.data(), nullptr, window_flags);

        ImGui::End();
    }
};
