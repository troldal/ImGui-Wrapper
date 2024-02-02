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
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <cstdint>
#include <filesystem>
#include <optional>

#include <boost-te/te.hpp>

namespace te = boost::te;

namespace fs = std::filesystem;

namespace imw
{
    struct IWindow : te::poly<IWindow> {
        using te::poly<IWindow>::poly;

        void render() const {
            te::call([](auto &self) { self.render(); }, *this);
        }
    };

    class Application final
    {
        static constexpr auto s_WindowWidth = std::uint32_t{1280};
        static constexpr auto s_WindowHeight = std::uint32_t{720};

        GLFWwindow *window = nullptr;
        std::optional<IWindow> window_obj{};


    public:
        static Application &instance()
        {
            static Application app;
            return app;
        }

        static void glfw_error_callback(int error, const char *description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }

        static void start_cycle()
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        }

        static void end_cycle(GLFWwindow *const window)
        {
            constexpr auto clear_color = ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
            int display_w = 0;
            int display_h = 0;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w,
                         clear_color.y * clear_color.w,
                         clear_color.z * clear_color.w,
                         clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

        static void init(IWindow &&window)
        {
            instance().window_obj = std::move(window);
        }

        static int run()
        {
            return instance().runMainLoop();
        }

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;
        Application(Application &&) = delete;
        Application &operator=(Application &&) = delete;

    private:
        int runMainLoop()
        {
            while (!glfwWindowShouldClose(window))
            {
                start_cycle();

                ImGui::NewFrame();
                ImPlot::CreateContext();
                window_obj->render();
                ImGui::Render();

                end_cycle(window);
            }

            return EXIT_SUCCESS;
        }

        Application()
        {
            // Setup window
            glfwSetErrorCallback(glfw_error_callback);
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW");

            // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
            // GL ES 2.0 + GLSL 100
            const char *glsl_version = "#version 100";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
            // GL 3.2 + GLSL 150
            const char *glsl_version = "#version 150";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
            // GL 3.0 + GLSL 130
            const char *glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

            // Create window with graphics context
            window = glfwCreateWindow(static_cast<std::int32_t>(s_WindowWidth),
                                      static_cast<std::int32_t>(s_WindowHeight),
                                      "Gui",
                                      nullptr,
                                      nullptr);
            if (window == nullptr)
                throw std::runtime_error("Failed to create window");
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();

            ImGuiIO& io = ImGui::GetIO();
            // io.FontGlobalScale = 2;

            ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Users\\kenne\\Desktop\\fonts\\segoeui.ttf", 28);

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);

            auto &style = ImGui::GetStyle();
            style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
            style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0, 1.0, 1.0, 1.0);
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
}
