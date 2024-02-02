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

    struct PopupModal
    {
    };
    struct PopupNonmodal
    {
    };

    template< typename DERIVED, typename STATE >
    class PopupBase
    {
        friend DERIVED;

    protected:
        ~PopupBase() = default;

    private:
        std::string m_windowName { "Delete File" };

        bool isOpen = false;

    public:
        explicit PopupBase(std::string_view windowName)
            : m_windowName(windowName)
        {}

        PopupBase(const PopupBase& other)     = default; /**< Default copy constructor. */
        PopupBase(PopupBase&& other) noexcept = default; /**< Default move constructor. */

        PopupBase& operator=(const PopupBase& other)     = default; /**< Default copy assignment operator. */
        PopupBase& operator=(PopupBase&& other) noexcept = default; /**< Default move assignment operator. */

        void open(std::string_view name)
        {
            m_windowName = name;
            isOpen       = true;
        }

        void render()
        {
            if constexpr (std::is_same_v< STATE, PopupNonmodal >) {
                if (ImGui::BeginPopup(m_windowName.c_str())) {
                    static_cast< DERIVED& >(*this).exec();
                    ImGui::EndPopup();
                }
            }
            else if constexpr (std::is_same_v< STATE, PopupModal >) {
                if (ImGui::BeginPopupModal(m_windowName.c_str(), &isOpen)) {
                    static_cast< DERIVED& >(*this).exec();
                    ImGui::EndPopup();
                }
            }
        }
    };

    template< typename DERIVED >
    using PopupBaseModal = PopupBase< DERIVED, PopupModal >;

    template< typename DERIVED >
    using PopupBaseNonmodal = PopupBase< DERIVED, PopupNonmodal >;

}    // namespace imw
