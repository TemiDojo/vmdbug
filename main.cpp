#include "imgui.h"
#include "imgui_internal.h" // Required for DockBuilder and hiding tabs
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

// --- Layout Configuration ---
void SetupImHexLayout(ImGuiID dockspace_id) {
    // 1. Reset the dockspace
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->WorkSize);

    // 2. Split the nodes (0.30f = 30% of space)
    ImGuiID dock_main_id = dockspace_id;
    ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.30f, nullptr, &dock_main_id);
    ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
    ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.30f, nullptr, &dock_main_id);

    // 3. Assign windows to nodes
    ImGui::DockBuilderDockWindow("Hex Editor", dock_main_id);
    ImGui::DockBuilderDockWindow("Data Inspector", dock_left_id);
    ImGui::DockBuilderDockWindow("Disassembly", dock_right_id);
    ImGui::DockBuilderDockWindow("Console", dock_bottom_id);

    // 4. HIDE TABS (The ImHex "Pane" Look)
    // We iterate through the nodes we just created and disable the tab bar
    // ImGui::DockBuilderGetNode(dock_main_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
    // ImGui::DockBuilderGetNode(dock_left_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
    // ImGui::DockBuilderGetNode(dock_right_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
    // ImGui::DockBuilderGetNode(dock_bottom_id)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

    ImGui::DockBuilderFinish(dockspace_id);
}

// --- Main Application ---
int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "vdbug", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Theme: Deep Dark (ImHex Aesthetic)
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // [A] MAIN MENU BAR
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Binary...")) {}
                if (ImGui::MenuItem("Exit", "Alt+F4")) break;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Settings")) {
                static bool show_demo = false;
                ImGui::MenuItem("Show ImGui Demo", nullptr, &show_demo);
                if (show_demo) ImGui::ShowDemoWindow();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // [B] DOCKSPACE SETUP
        // We pass NoTabBar here as a global fallback, but we handled it in DockBuilder too
        ImGuiDockNodeFlags ds_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
        ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ds_flags);

        static bool first_run = true;
        if (first_run) {
            SetupImHexLayout(dockspace_id);
            first_run = false;
        }

        // [C] WINDOW CONTENT (Must match the strings in DockBuilderDockWindow)
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("Hex Editor", nullptr, window_flags);
        ImGui::Text("00000000: 48 89 E5 48 83 EC 10 0F B6 45 F7 48 8D 3D ... ");
        ImGui::End();

        ImGui::Begin("Data Inspector", nullptr, window_flags);
        if (ImGui::BeginTable("Regs", 2, ImGuiTableFlags_Borders)) {
            ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0); ImGui::Text("RAX");
            ImGui::TableSetColumnIndex(1); ImGui::Text("0xDEADC0DE");
            ImGui::EndTable();
        }
        ImGui::End();

        ImGui::Begin("Disassembly", nullptr, window_flags);
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "0x401050: push rbp");
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "0x401051: mov  rbp, rsp");
        ImGui::End();

        ImGui::Begin("Console", nullptr, window_flags);
        ImGui::Text("[info] vdbug initialized.");
        ImGui::Text("[info] Attached to process.");
        ImGui::End();

        // [D] RENDERING
        ImGui::Render();
        glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
