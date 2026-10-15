// Behaviour test for CSystemManager::SetExclusiveMode (0x009A59A0): two passes (requested mode, then the old one), each
// resetting the priority and switching the display unless the input manager refuses; locked out entirely when prevented.
#include <cstdio>
static int g_prio, g_display; static bool g_modes[4]; static int g_nmodes;
struct Input { bool accept; bool SetExclusiveMode(bool) { return accept; } };
struct Display { long SetExclusiveMode(bool m) { g_modes[g_nmodes++] = m; ++g_display; return 0; } };
static void SetPriorityClass(void*, unsigned int c) { if (c == 0x20) ++g_prio; }
struct Sys {
    Input* PInputManager; Display* PDisplayManager; bool ExclusiveMode; bool PreventExclusiveModeChanges;
    bool SetExclusiveMode(bool exclusive)
    {
        bool oldMode = ExclusiveMode;
        if (!PreventExclusiveModeChanges) {
            void* process = 0;
            for (long pass = 0; pass < 2; ++pass) {
                if (pass == 0) ExclusiveMode = exclusive; else ExclusiveMode = oldMode;
                SetPriorityClass(process, 0x20);
                if (!PInputManager || PInputManager->SetExclusiveMode(ExclusiveMode))
                    PDisplayManager->SetExclusiveMode(ExclusiveMode);
            }
        }
        return false;
    }
};
int main()
{
    Display d; Input in; in.accept = true;
    Sys s; s.PInputManager = 0; s.PDisplayManager = &d; s.ExclusiveMode = false; s.PreventExclusiveModeChanges = false;
    if (s.SetExclusiveMode(true) || g_prio != 2 || g_display != 2 || g_modes[0] != true || g_modes[1] != false || s.ExclusiveMode) { std::printf("009a59a0_TEST FAIL\n"); return 1; }
    s.PInputManager = &in; in.accept = false;
    s.SetExclusiveMode(true);
    if (g_display != 2 || g_prio != 4) { std::printf("009a59a0_TEST FAIL\n"); return 1; }
    s.PreventExclusiveModeChanges = true;
    s.SetExclusiveMode(true);
    if (g_prio != 4) { std::printf("009a59a0_TEST FAIL\n"); return 1; }
    std::printf("009a59a0_TEST PASS\n");
    return 0;
}
