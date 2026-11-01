// Test for PlayCutsceneSound (0x00CBF7FE, parked DIFFER): the macro parameter `key` is looked up; non-empty text is parsed
// item by item: "Play2DSound <name>" plays a 2D sound, "PlayMusic <type>" overrides the music when the music-type symbol
// map knows the name (and raises the cutscene-music flag), "StopMusic" stops the override; each item then skips to the
// next line. A missing or empty parameter does nothing.
#include <cstdio>
#include <cstring>
static char g_log[256]; static bool g_musicOverridden = false;
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long GetSymbolSLONG(const char* name) { if (std::strcmp(name, "MUSIC_COMBAT") == 0) return 3; if (std::strcmp(name, "MUSIC_TOWN") == 0) return 1; return -1; }
struct Parser {   // whitespace tokens, newline-separated items
    const char* p;
    void SkipBlanks() { while (*p == ' ') ++p; }
    bool NextItemExists() { while (*p == ' ' || *p == '\n') ++p; return *p != 0; }
    void ReadAsString(char* out) { SkipBlanks(); int n = 0; while (*p && *p != ' ' && *p != '\n') out[n++] = *p++; out[n] = 0; }
    void SkipPastNewline() { while (*p && *p != '\n') ++p; if (*p) ++p; }
};
struct Param { int key; const char* text; };
static void PlayCutsceneSound(const Param* params, int count, int key)
{
    const char* text = 0;
    for (int i = 0; i < count; ++i) if (params[i].key == key) { text = params[i].text; break; }
    if (!text || !*text) return;
    Parser parser = { text };
    while (parser.NextItemExists()) {
        char command[64]; parser.ReadAsString(command);
        if (std::strcmp(command, "Play2DSound") == 0) { char name[64]; parser.ReadAsString(name); Log("2D"); Log(name); }
        if (std::strcmp(command, "PlayMusic") == 0) {
            char name[64]; parser.ReadAsString(name); long type = GetSymbolSLONG(name);
            if (type != -1) { char t[8]; std::sprintf(t, "M%ld", type); Log(t); g_musicOverridden = true; }
        }
        if (std::strcmp(command, "StopMusic") == 0) Log("STOP");
        parser.SkipPastNewline();
    }
}
int main()
{
    Param params[3] = { { 1, "Play2DSound SND_DOOR extra\nPlayMusic MUSIC_BOGUS\nStopMusic\n" }, { 2, "" }, { 4, "PlayMusic MUSIC_COMBAT\nUnknown x" } };
    PlayCutsceneSound(params, 3, 1);
    if (std::strcmp(g_log, "2D;SND_DOOR;STOP;") != 0 || g_musicOverridden) { std::printf("00cbf7fe_TEST FAIL\n"); return 1; }
    PlayCutsceneSound(params, 3, 2); PlayCutsceneSound(params, 3, 9);
    if (std::strcmp(g_log, "2D;SND_DOOR;STOP;") != 0) { std::printf("00cbf7fe_TEST FAIL\n"); return 1; }
    PlayCutsceneSound(params, 3, 4);
    if (std::strcmp(g_log, "2D;SND_DOOR;STOP;M3;") != 0 || !g_musicOverridden) { std::printf("00cbf7fe_TEST FAIL\n"); return 1; }
    std::printf("00cbf7fe_TEST PASS\n");
    return 0;
}
