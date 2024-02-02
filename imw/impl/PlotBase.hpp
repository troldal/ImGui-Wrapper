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
    class PlotBase
    {
        friend DERIVED;

    protected:
        ~PlotBase() = default;

    private:
        ImGuiWindowFlags m_flags {};
        ImVec2           m_size {};
        ImVec2           m_pos {};
        std::string      m_windowName {};

    public:

        explicit PlotBase(std::string_view windowName)
            : m_windowName(windowName)
        {}

        PlotBase(const PlotBase& other)     = default; /**< Default copy constructor. */
        PlotBase(PlotBase&& other) noexcept = default; /**< Default move constructor. */

        PlotBase& operator=(const PlotBase& other)     = default; /**< Default copy assignment operator. */
        PlotBase& operator=(PlotBase&& other) noexcept = default; /**< Default move assignment operator. */

        //void setFlags(ImGuiWindowFlags flags) { m_flags = flags; }
        //void setSize(ImVec2 size) { m_size = size; }
        //void setPos(ImVec2 pos) { m_pos = pos; }
        //void setName(std::string_view name) { m_windowName = name; }

        void render()
        {
            ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);

            static_cast< DERIVED& >(*this).exec();

            ImPlot::EndPlot();
        }
    };

}    // namespace imw
