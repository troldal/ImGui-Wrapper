//
// Created by kenne on 02/02/2024.
//

#pragma once

#include <imw.hpp>

class DeleteFilePopup : public imw::PopupBaseModal< DeleteFilePopup >
{
    using BASE = imw::PopupBaseModal< DeleteFilePopup >;
    friend BASE;

    fs::path selectedEntry;

    void exec()
    {
        ImGui::Text("Are you sure you want to delete %s?", selectedEntry.filename().string().c_str());

        if (ImGui::Button("Yes")) {
            if (deleteFile(selectedEntry)) selectedEntry.clear();
            isOpen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("No")) isOpen = false;
    }

public:
    using BASE::BASE;

    static bool deleteFile(const fs::path& path)
    {
        try {
            fs::remove(path);
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    void setSelectedEntry(const fs::path& entry) { selectedEntry = entry; }
};
