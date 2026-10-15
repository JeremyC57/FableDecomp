// Behaviour test for CSystemManager::CSystemManager (0x009A7440): every counted pointer empty, the strings constructed.
#include <cstdio>
static int g_wide, g_char;
struct Wide { void* P; Wide() : P(0) { ++g_wide; } };
struct Chars { void* P; Chars() : P(0) { ++g_char; } };
struct CP { void* Data; void* Info; CP() : Data(0), Info(0) {} };
struct Sys {
    CP LibDebugManager, PInputManager, PDisplayManager; void* DebugManager;
    CP PGraphicBankManager, PMeshBankManager, PFontManager, PRetailManager;
    Wide WinAppName; CP ProfileManager; Wide TempPath; Chars ApplicationName;
};
int main()
{
    Sys s;
    const CP* all[8] = { &s.LibDebugManager, &s.PInputManager, &s.PDisplayManager, &s.PGraphicBankManager, &s.PMeshBankManager, &s.PFontManager, &s.PRetailManager, &s.ProfileManager };
    for (int i = 0; i < 8; ++i) if (all[i]->Data || all[i]->Info) { std::printf("009a7440_TEST FAIL\n"); return 1; }
    if (g_wide != 2 || g_char != 1) { std::printf("009a7440_TEST FAIL\n"); return 1; }
    std::printf("009a7440_TEST PASS\n");
    return 0;
}