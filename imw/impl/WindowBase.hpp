//
// Created by kenne on 02/02/2024.
//

#pragma once

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui.h>

namespace imw
{

    template< typename DERIVED >
    class WindowBase
    {
        friend DERIVED;

    protected:
        ~WindowBase() = default;

    private:
        ImGuiWindowFlags m_flags {};
        ImVec2           m_size {};
        ImVec2           m_pos {};
        std::string      m_windowName {};

    public:

        explicit WindowBase(std::string_view windowName)
            : m_windowName(windowName)
        {}

        WindowBase(const WindowBase& other)     = default; /**< Default copy constructor. */
        WindowBase(WindowBase&& other) noexcept = default; /**< Default move constructor. */

        WindowBase& operator=(const WindowBase& other)     = default; /**< Default copy assignment operator. */
        WindowBase& operator=(WindowBase&& other) noexcept = default; /**< Default move assignment operator. */

        void setFlags(ImGuiWindowFlags flags) { m_flags = flags; }
        void setSize(ImVec2 size) { m_size = size; }
        void setPos(ImVec2 pos) { m_pos = pos; }
        void setName(std::string_view name) { m_windowName = name; }

        void render()
        {
            ImGui::Begin(m_windowName.data(), nullptr, m_flags);

            static_cast< DERIVED& >(*this).exec();

            ImGui::End();
        }
    };

}    // namespace imw
