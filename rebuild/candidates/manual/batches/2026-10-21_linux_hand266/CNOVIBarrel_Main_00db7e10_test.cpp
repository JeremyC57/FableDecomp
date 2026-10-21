#include <cstdio>
#include <cstring>
// Behaviour test for CNOVI_Barrel::Main (0x00DB7E10): until the parent flag is set, the barrel waits for the hero
// within range, shows the platform's game-info text once, waits for it to be clicked past and sets the flag; then it
// idles frame by frame until the thread terminates.
static int g_frames = 0, g_stopAt = 0, g_nearAt = 0, g_clickAt = 0, g_shown = 0;
static bool g_xbox = false;
static char g_text[16];
struct CNOVI_Barrel {
    bool GameInfoShown;
    bool Term() { return g_frames >= g_stopAt; }
    void Frame() { ++g_frames; }
    bool Near() { return g_frames >= g_nearAt; }
    bool Clicked() { return g_frames >= g_clickAt; }
    void Show(const char* t) { ++g_shown; std::strcpy(g_text, t); }
    void Main()
    {
        Frame();
        if (Term()) return;
        while (!GameInfoShown) {
            Frame();
            if (Term()) return;
            if (Near()) {
                if (Term()) return;
                if (g_xbox) { if (Term()) return; Show("XBOX"); while (!Clicked()) { Frame(); if (Term()) return; } }
                else        { if (Term()) return; Show("PC");   while (!Clicked()) { Frame(); if (Term()) return; } }
                if (Term()) return;
                GameInfoShown = true;
            }
        }
        while (!Term()) Frame();
    }
};
int main()
{
    int bad = 0;
    CNOVI_Barrel b; b.GameInfoShown = false;
    g_stopAt = 20; g_nearAt = 4; g_clickAt = 7;
    b.Main();
    bad |= !b.GameInfoShown || g_shown != 1 || std::strcmp(g_text, "PC") != 0 || g_frames != 20;
    g_frames = 0; g_shown = 0; g_xbox = true; b.GameInfoShown = false; g_stopAt = 6;
    b.Main();
    bad |= b.GameInfoShown || g_shown != 1 || std::strcmp(g_text, "XBOX") != 0 || g_frames != 6;
    g_frames = 0; g_shown = 0; b.GameInfoShown = true; g_stopAt = 3;
    b.Main();
    bad |= g_shown != 0 || g_frames != 3;
    if (bad) { std::printf("00db7e10_TEST FAIL\n"); return 1; }
    std::printf("00db7e10_TEST PASS\n");
    return 0;
}
