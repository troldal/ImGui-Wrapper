//
// Created by kenne on 02/02/2024.
//

#pragma once

#include <imw.hpp>

class FileSystemView : public imw::WindowBase< FileSystemView >
{
    fs::path currentPath;
    fs::path selectedEntry;

    using BASE = imw::WindowBase< FileSystemView >;

    friend BASE;

public:
    using BASE::BASE;

    explicit FileSystemView(std::string_view windowName)
        : BASE(windowName),
          currentPath(fs::current_path()),
          selectedEntry(fs::path {}) {};

    void setCurrentPath(const fs::path& path) { currentPath = path; }

    [[nodiscard]]
    auto getSelectedEntry() const -> const fs::path&
    {
        return selectedEntry;
    }

    [[nodiscard]]
    auto getCurrentPath() const -> const fs::path&
    {
        return currentPath;
    }

private:
    void exec() { DrawContent(); }

    void DrawContent()
    {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            const auto is_selected  = entry.path() == selectedEntry;
            const auto is_directory = entry.is_directory();
            const auto is_file      = entry.is_regular_file();
            auto       entry_name   = entry.path().filename().string();

            if (is_directory)
                entry_name = "[D]" + entry_name;
            else if (is_file)
                entry_name = "[F]" + entry_name;

            if (ImGui::Selectable(entry_name.c_str(), is_selected)) {
                if (is_directory) currentPath /= entry.path().filename();

                selectedEntry = entry.path();
            }
        }
    }
};