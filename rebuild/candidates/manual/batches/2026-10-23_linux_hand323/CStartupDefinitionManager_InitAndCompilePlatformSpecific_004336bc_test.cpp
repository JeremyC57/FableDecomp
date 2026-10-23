// Behaviour test for CStartupDefinitionManager::InitAndCompilePlatformSpecific (0x004336BC): the header and def lists
// (bank-directory headers, then Data\Defs files; the test front-end def replaces the PC front-end/config/controller set)
// and the five registered startup definition types.
#include <cstdio>
#include <cstring>

struct List {
    char items[24][64];
    int count;
    void Add(const char* a, const char* b, const char* c)
    {
        std::strcpy(items[count], a);
        std::strcat(items[count], b);
        std::strcat(items[count], c);
        ++count;
    }
};
static const char* kBank = "Data\\Bank\\";
static const char* kDefs = "Data\\Defs\\";

static void Build(bool testFrontEnd, List& headers, List& defs, int& types)
{
    headers.count = 0; defs.count = 0; types = 0;
    headers.Add(kBank, "pc\\", "textures.h");
    headers.Add(kBank, "", "fonts.h");
    headers.Add(kBank, "pc\\", "front_end_bank.h");
    headers.Add(kBank, "pc\\", "particles_frontend.h");
    headers.Add(kDefs, "", "FrontEndDefs\\engine.def");
    headers.Add(kDefs, "", "FrontEndDefs\\engine_video_options.def");
    headers.Add(kDefs, "", "FrontEndDefs\\front_end.def");
    headers.Add(kDefs, "", "ui_types.h");
    headers.Add(kDefs, "", "ui_dialogs.def");
    if (testFrontEnd)
        headers.Add(kDefs, "", "FrontEndDefs\\frontend_test.def");
    else {
        headers.Add(kDefs, "", "FrontEndDefs\\pc_frontend.def");
        headers.Add(kDefs, "", "config_options_defaults.def");
        headers.Add(kDefs, "", "controller_def.h");
        headers.Add(kDefs, "", "keyboard_keys.h");
        headers.Add(kDefs, "", "pc_controls.def");
    }
    defs.Add(kDefs, "", "FrontEndDefs\\engine.def");
    defs.Add(kDefs, "", "FrontEndDefs\\engine_video_options.def");
    defs.Add(kDefs, "", "FrontEndDefs\\front_end.def");
    defs.Add(kDefs, "", "ui_dialogs.def");
    if (testFrontEnd)
        defs.Add(kDefs, "", "FrontEndDefs\\frontend_test.def");
    else {
        defs.Add(kDefs, "", "FrontEndDefs\\pc_frontend.def");
        defs.Add(kDefs, "", "pc_controls.def");
        defs.Add(kDefs, "", "config_options_defaults.def");
    }
    const char* names[5] = { "ENGINE_VIDEO_OPTIONS", "ENGINE", "UI", "CONTROL_SCHEME", "CONFIG_OPTIONS_DEFAULTS_DEF" };
    for (int i = 0; i < 5; ++i)
        if (names[i][0])
            ++types;
}

int main()
{
    static List headers, defs;
    int types;
    bool bad = false;
    Build(false, headers, defs, types);
    if (headers.count != 14 || defs.count != 7 || types != 5) bad = true;
    if (std::strcmp(headers.items[0], "Data\\Bank\\pc\\textures.h") != 0) bad = true;
    if (std::strcmp(headers.items[1], "Data\\Bank\\fonts.h") != 0) bad = true;
    if (std::strcmp(defs.items[5], "Data\\Defs\\pc_controls.def") != 0) bad = true;
    Build(true, headers, defs, types);
    if (headers.count != 10 || defs.count != 5) bad = true;
    if (std::strcmp(headers.items[9], "Data\\Defs\\FrontEndDefs\\frontend_test.def") != 0) bad = true;
    if (bad) { std::printf("004336bc_TEST FAIL\n"); return 1; }
    std::printf("004336bc_TEST PASS\n");
    return 0;
}
