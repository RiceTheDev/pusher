#include "ui.h"

#include "../screen.h"
#include "../cpu.h"
#include "../rom.h"
#include "../audio.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"
#include "imgui/imgui.h"
#include "../assets.h"
#include "imgui/imgui_memory_editor.h"
#include "nfd.h"

#include "theme.h"

MemoryEditor mem_edit;

bool show_mem_editor = true;
bool show_debug_info = true;
bool show_audio_debug = true;
bool show_emu_controls = true;
bool show_about_dialog = false;
bool show_credits_dialog = false;
bool dark_theme = true;

void ui_init(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;

    ImFont* custom_font = io.Fonts->AddFontFromMemoryTTF(SometypeMono_ttf, SometypeMono_len, 20.0f, &font_cfg);

    if (custom_font == NULL) {
        printf("failed to load font, falling back to default font.\n");
        io.Fonts->AddFontDefault();
    }

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    if (NFD_Init() != NFD_OKAY) {
        printf("failed to initialize nfd: %s\n", NFD_GetError());
    }
}

void ui_poll_events(SDL_Event *event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
}

void ui_begin_frame(void)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ui_create_main_menu_bar(void) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File ")) {
            if (ImGui::MenuItem("Load ROM")) {
                nfdu8char_t *file_path;
                nfdresult_t result = NFD_OpenDialogU8(&file_path, NULL, 0, NULL);
                if (result == NFD_OKAY) {
                    printf("selected rom: %s\n", file_path);
                    if (rom_load(file_path)) {
                        pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];
                        running = 1;
                    }
                }
                NFD_FreePathU8(file_path);
            }

            if (ImGui::MenuItem("Quit")) {
                running = 0;
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu(" View ")) {
            ImGui::Checkbox("Memory Editor", &show_mem_editor);

            ImGui::Checkbox("Debug Info", &show_debug_info);

            ImGui::Checkbox("Audio Visualizer", &show_audio_debug);

            ImGui::Checkbox("Emulator Controls", &show_emu_controls);
            
            if (ImGui::BeginMenu("Theme")) {
                if (ImGui::MenuItem("Light Theme")) {
                    dark_theme = false;
                    setup_imgui_light_theme();
                }
                if (ImGui::MenuItem("Dark Theme")) {
                    dark_theme = true;
                    setup_imgui_dark_theme();
                }

                ImGui::EndMenu();
            }


            if (ImGui::Button("Reset windows to default")) {
                show_mem_editor = true;
                show_debug_info = true;
                show_audio_debug = true;
                show_emu_controls = true;

                ImGui::LoadIniSettingsFromMemory(DEFAULT_LAYOUT_INI, strlen(DEFAULT_LAYOUT_INI));
            }
            
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu(" Help ")) {
            ImGui::Text("Keyboard Shortcuts:");
            ImGui::Separator();
            ImGui::Text("F10: Step (when paused)");
            ImGui::Text("ESC: Quit");
            if (ImGui::MenuItem("About")) {
                show_about_dialog = true;
            }
            if (ImGui::MenuItem("Credits")) {
                show_credits_dialog = true;
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }

    if (show_about_dialog) {
        ImGui::OpenPopup("About");
        show_about_dialog = false;
    }

    
    if (show_credits_dialog) {
        ImGui::OpenPopup("Credits");
        show_credits_dialog = false;
    }

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(
        ImVec2(
            viewport->Pos.x + viewport->Size.x * 0.5f,
            viewport->Pos.y + viewport->Size.y * 0.5f
        ),
        ImGuiCond_Appearing,
        ImVec2(0.5f, 0.5f)
    );

    if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("pusher8 emu v1.0\n");
        ImGui::Text("author: raice (ricethedev/pusher8)\n");
        ImGui::Separator();
        ImGui::Text("a simple bytepusher emulator in c/c++.\n");
        ImGui::Text("(dont use this, its really bad)");
        ImGui::Spacing();

        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SetNextWindowPos(
        ImVec2(
            viewport->Pos.x + viewport->Size.x * 0.5f,
            viewport->Pos.y + viewport->Size.y * 0.5f
        ),
        ImGuiCond_Appearing,
        ImVec2(0.5f, 0.5f)
    );

    if (ImGui::BeginPopupModal("Credits", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("pusher8 by raice (ricethedev)\n");
        ImGui::Text("-- emulator --\n");
        if (ImGui::BeginTable("EmuInfo", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("author: raice");
            ImGui::Text("license: Apache 2.0");
            ImGui::EndTable();
        }

        ImGui::Text("-- font --\n");
        if (ImGui::BeginTable("FontInfo", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("font: SometypeMono");
            ImGui::Text("author: Ryoichi Tsunekawa");
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("license: SIL Open Font License");
            ImGui::EndTable();
        }

        ImGui::Text("-- themes --\n");
        if (ImGui::BeginTable("ThemeInfo", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("dark theme: Rest");
            ImGui::Text("author: AaronBeardless");
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("from imthemes");
            ImGui::Text("\n");

            ImGui::Separator();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("light theme: Comfortable Light Orange");
            ImGui::Text("author: SouthCraftX");
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("from imthemes");

            ImGui::EndTable();
        }
        ImGui::Spacing();

        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ui_create_emu_window(void)
{
    ImGui::SetNextWindowSize(ImVec2(512, 512), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(35, 52), ImGuiCond_FirstUseEver);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Emulator");
    
    ImVec2 avail_size = ImGui::GetContentRegionAvail();
    ImGui::Image(
        (ImTextureID)(intptr_t)render_target,
        avail_size,
        ImVec2(0, 0),
        ImVec2(1, 1),
        ImVec4(1, 1, 1, 1),
        ImVec4(0, 0, 0, 0)
    );

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void ui_create_emu_memview(void) {
    ImGui::SetNextWindowPos(ImVec2(879, 139), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(379, 379), ImGuiCond_FirstUseEver);
    
    mem_edit.DrawWindow("Memory Editor", memory, sizeof(memory));
}

void ui_create_emu_controls(void) {
    ImGui::SetNextWindowPos(ImVec2(589,50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(667, 85), ImGuiCond_FirstUseEver);

    ImGui::Begin("Emulator Controls");

    bool can_step = paused;
    if (!can_step) ImGui::BeginDisabled();

    if (ImGui::Button("Step") || (can_step && ImGui::IsKeyPressed(ImGuiKey_F10))) {
        pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];

        for (int i = 0; i < 0x10000; i++) {
            cpu_step();
        }
    }

    if (!can_step) ImGui::EndDisabled();

    ImGui::SameLine();

    if (paused) {
        if (ImGui::Button("Resume")) {
            paused = 0;
        }
    } else {
        if (ImGui::Button("Pause")) {
            paused = 1;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset")) {
        pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];
        running = 1;
    }

    ImGui::SameLine();

    if (ImGui::Button("Hard Reset")) {
        if (rom_load(rom_path)) {
            pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];
            running = 1;
        }
    }

    ImGui::SameLine();
    ImGui::TextDisabled("|");
    ImGui::SameLine();
    
    if (paused) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Paused");
    } else if (!started) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Stopped");
    } else {
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Running");
    }

    ImGui::End();
}

void ui_create_emu_audio_debug(void) {
    ImGui::SetNextWindowPos(ImVec2(589, 139), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(285, 321), ImGuiCond_FirstUseEver);

    ImGui::Begin("Audio Visualizer");

    static float audio_debug_values[256];
    for (int i = 0; i < 256; ++i) {
        audio_debug_values[i] = (float)audio_debug_buffer[i] / 128.0f + 0.5f;
    }

    ImVec2 avail = ImGui::GetContentRegionAvail();
    
    if (avail.x > 1.0f && avail.y > 1.0f) {
        ImGui::PlotHistogram("##audio_plot", audio_debug_values, 256, 0, "", 0.0f, 2.0f, avail, sizeof(float));
    }

    ImGui::End();
}

void ui_create_emu_debug_info(void) {
    ImGui::SetNextWindowPos(ImVec2(589, 464), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(286, 142), ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug Info");

    ImGui::Text("PC: %06X", pc);
    ImGui::Text("Next Instruction: %06X", ((uint32_t)memory[pc + 0] << 16) | ((uint32_t)memory[pc + 1] << 8) | memory[pc + 2]);
    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::End();
}

void ui_render(void)
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

    SDL_RenderPresent(renderer);
}

void ui_quit(void)
{
    NFD_Quit();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}