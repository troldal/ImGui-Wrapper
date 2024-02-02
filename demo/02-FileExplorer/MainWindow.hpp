#pragma once

#include <cstdint>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <string_view>

#include "DeleteFilePopup.hpp"
#include "FileSystemView.hpp"
#include "RenameFilePopup.hpp"

class MainWindow : public imw::WindowBase< MainWindow >
{
    fs::path currentPath;
    fs::path selectedEntry;

    FileSystemView  fileSystemView { "File System Viewer" };
    DeleteFilePopup deleteFilePopup { "Delete File" };
    RenameFilePopup renameFilePopup { "Rename File" };

    constexpr static auto main_window_size     = ImVec2(1280.0F, 720.0F);
    constexpr static auto main_window_pos      = ImVec2(0.0F, 0.0F);
    constexpr static auto upper_section_height = 100.0F;
    constexpr static auto lower_section_height = 200.0F;
    constexpr static auto content_height       = main_window_size.y - lower_section_height - upper_section_height;

    using BASE = imw::WindowBase< MainWindow >;
    friend BASE;

public:
    using BASE::BASE;

    explicit MainWindow(std::string_view windowName)
        : BASE(windowName),
          currentPath(fs::current_path()),
          selectedEntry(fs::path {})
    {
        fileSystemView.setSize(ImVec2(main_window_size.x, content_height));
        fileSystemView.setPos(ImVec2(main_window_pos.x, upper_section_height));
        fileSystemView.setFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoTitleBar);
    };

private:
    void exec()
    {
        DrawMenu();
        ImGui::Separator();

        DrawFileSystemView();
        ImGui::Separator();

        DrawActions();
        ImGui::Separator();

        DrawFilter();
    }

    void DrawFileSystemView()
    {
        fileSystemView.setName(fmt::format("##innerContent{}", currentPath.string()));
        fileSystemView.setCurrentPath(currentPath);

        fileSystemView.render();
        selectedEntry = fileSystemView.getSelectedEntry();
        currentPath   = fileSystemView.getCurrentPath();
    }

    void DrawMenu()
    {
        if (ImGui::Button("Go Up")) {
            if (currentPath.has_parent_path()) {
                currentPath = currentPath.parent_path();
            }
        }

        ImGui::SameLine();

        ImGui::Text("Current directory: %s", currentPath.string().c_str());
    }

    void DrawActions()
    {
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height);
        if (fs::is_directory(selectedEntry))
            ImGui::Text("Selected dir: %s", selectedEntry.string().c_str());
        else if (fs::is_regular_file(selectedEntry))
            ImGui::Text("Selected file: %s", selectedEntry.string().c_str());
        else {
            ImGui::Text("Selected file: n/a");
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.0f);
            ImGui::Button("Non-clickable button");
            ImGui::PopStyleVar();
            return;
        }

        if (fs::is_regular_file(selectedEntry) && ImGui::Button("Open")) openFileWithDefaultEditor();

        ImGui::SameLine();

        if (ImGui::Button("Rename")) {
            renameFilePopup.setSelectedEntry(selectedEntry);
            renameFilePopup.open("Rename File");
            ImGui::OpenPopup("Rename File");
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            deleteFilePopup.setSelectedEntry(selectedEntry);
            deleteFilePopup.open("Delete File");
            ImGui::OpenPopup("Delete File");
        }

        deleteFilePopup.render();
        renameFilePopup.render();
    }

    void DrawFilter() const
    {
        static char extension_filter[16] = { "\0" };

        ImGui::Text("Filter by extension");
        ImGui::SameLine();
        ImGui::InputText("###inFilter", extension_filter, sizeof(extension_filter));

        if (std::strlen(extension_filter) == 0) return;

        auto filtered_file_count = std::size_t { 0 };
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (!fs::is_regular_file(entry)) continue;

            if (entry.path().extension().string() == extension_filter) ++filtered_file_count;
        }

        ImGui::Text("Number of files: %u", filtered_file_count);
    }

    void openFileWithDefaultEditor() const
    {
#ifdef _WIN32
        const auto command = "start \"\" \"" + selectedEntry.string() + "\"";
#elif __APPLE__
        const auto command = "open \"" + selectedEntry.string() + "\"";
#else
        const auto command = "xdg-open \"" + selectedEntry.string() + "\"";
#endif

        std::system(command.c_str());
    }
};
