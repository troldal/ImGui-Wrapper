//
// Created by kenne on 02/02/2024.
//

#pragma once

#include <imw.hpp>

class RenameFilePopup : public imw::PopupBaseModal< RenameFilePopup >
{
    using BASE = imw::PopupBaseModal< RenameFilePopup >;
    friend BASE;

    fs::path selectedEntry;

    void exec()
    {
        static char buffer_name[512] = { '\0' };

        ImGui::Text("New name: ");
        ImGui::InputText("###newName", buffer_name, sizeof(buffer_name));

        if (ImGui::Button("Rename")) {
            auto new_path = selectedEntry.parent_path() / buffer_name;
            if (renameFile(selectedEntry, new_path)) {
                isOpen = false;
                selectedEntry    = new_path;
                std::memset(buffer_name, 0, sizeof(buffer_name));
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) isOpen = false;
    }

public:
    using BASE::BASE;

    static bool renameFile(const fs::path& old_path, const fs::path& new_path)
    {
        try {
            fs::rename(old_path, new_path);
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    void setSelectedEntry(const fs::path& entry) { selectedEntry = entry; }
};
