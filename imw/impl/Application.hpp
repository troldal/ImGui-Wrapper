//
// Created by kenne on 02/02/2024.
//

#pragma once

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui.h>
#include <implot.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#    include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#    pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <cstdint>
#include <filesystem>
#include <optional>

#include <boost-te/te.hpp>

#include<windows.h>

namespace te = boost::te;

namespace fs = std::filesystem;

namespace imw
{
    struct IWindow : te::poly< IWindow >
    {
        using te::poly< IWindow >::poly;

        void render() const
        {
            te::call([](auto& self) { self.render(); }, *this);
        }
    };

    class Application final
    {
        static constexpr auto s_WindowWidth  = std::uint32_t { 1280 };
        static constexpr auto s_WindowHeight = std::uint32_t { 720 };

        GLFWwindow*              window = nullptr;
        std::optional< IWindow > window_obj {};

    public:
        static Application& instance()
        {
            static Application app;
            return app;
        }

        static void glfw_error_callback(int error, const char* description) { fprintf(stderr, "Glfw Error %d: %s\n", error, description); }

        static void start_cycle()
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }

        static void end_cycle(GLFWwindow* const window)
        {
            constexpr auto clear_color = ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
            int            display_w   = 0;
            int            display_h   = 0;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

        static void init(IWindow&& window) { instance().window_obj = std::move(window); }

        static int run() { return instance().runMainLoop(); }

                     Application(const Application&) = delete;
        Application& operator=(const Application&)   = delete;
                     Application(Application&&)      = delete;
        Application& operator=(Application&&)        = delete;

    private:
        int runMainLoop()
        {
            while (!glfwWindowShouldClose(window)) {
                start_cycle();

                ImGui::NewFrame();
                window_obj->render();
                //ImGui::ShowDemoWindow();
                //ImPlot::ShowDemoWindow();
                ImGui::Render();

                end_cycle(window);
            }

            return EXIT_SUCCESS;
        }

        Application()
        {
            // Setup window
            glfwSetErrorCallback(glfw_error_callback);
            if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

                // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
            // GL ES 2.0 + GLSL 100
            const char* glsl_version = "#version 100";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
            // GL 3.2 + GLSL 150
            const char* glsl_version = "#version 150";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);              // Required on Mac
#else
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

            // Create window with graphics context
            window = glfwCreateWindow(static_cast< std::int32_t >(s_WindowWidth),
                                      static_cast< std::int32_t >(s_WindowHeight),
                                      "Gui",
                                      nullptr,
                                      nullptr);
            if (window == nullptr) throw std::runtime_error("Failed to create window");
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);    // Enable vsync

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            ImPlot::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            // io.FontGlobalScale = 2;

            ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Users\\kenne\\Desktop\\fonts\\segoeui.ttf", 28);

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);

            auto& style                              = ImGui::GetStyle();
            //style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
            //style.Colors[ImGuiCol_TableBorderLight]  = ImVec4(1.0, 1.0, 1.0, 1.0);

            ImVec4* colors                         = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
            colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
            colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
            colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
            colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
            colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
            colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            //colors[ImGuiCol_DockingPreview]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            //colors[ImGuiCol_DockingEmptyBg]        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

            //ImGuiStyle& style       = ImGui::GetStyle();
            style.WindowPadding     = ImVec2(8.00f, 8.00f);
            style.FramePadding      = ImVec2(12.00f, 6.00f);
            style.CellPadding       = ImVec2(6.00f, 6.00f);
            style.ItemSpacing       = ImVec2(10.00f, 7.00f);
            style.ItemInnerSpacing  = ImVec2(10.00f, 7.00f);
            style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
            style.IndentSpacing     = 25;
            style.ScrollbarSize     = 20;
            style.GrabMinSize       = 10;
            style.WindowBorderSize  = 1;
            style.ChildBorderSize   = 1;
            style.PopupBorderSize   = 1;
            style.FrameBorderSize   = 1;
            style.TabBorderSize     = 1;
            style.WindowRounding    = 7;
            style.ChildRounding     = 5;
            style.FrameRounding     = 8;
            style.PopupRounding     = 4;
            style.ScrollbarRounding = 9;
            style.GrabRounding      = 3;
            style.LogSliderDeadzone = 4;
            style.TabRounding       = 4;

            auto& plotStyle = ImPlot::GetStyle();
            plotStyle.LineWeight = 3;

        }

        ~Application()
        {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);
            glfwTerminate();
        }
    };
}    // namespace imw
