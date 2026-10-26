// Behaviour test for CSystemManager::IsApplicationActive @ 0x009A57B0: active = our window is the
// foreground window; regaining focus after having been active once requests a restore.
#include <cstdio>

struct HWND__ { int unused; };

static HWND__* g_foreground = 0;
static HWND__* GetForegroundWindowStub() { return g_foreground; }

class CSystemManager { public:
    bool    ApplicationActive;
    HWND__* ApplicationWinHandle;
    bool    EverBeenActive;
    bool    Restore;

    bool IsApplicationActive()
    {
        HWND__* window = ApplicationWinHandle;
        bool active = (window == GetForegroundWindowStub());
        if (active && !ApplicationActive && EverBeenActive == true)
            Restore = true;
        ApplicationActive = active;
        EverBeenActive = true;
        return active;
    }
};

int main()
{
    HWND__ ours, other;
    CSystemManager m;
    m.ApplicationActive = false;
    m.ApplicationWinHandle = &ours;
    m.EverBeenActive = false;
    m.Restore = false;
    bool ok = true;

    // First activation: no restore (never been active before).
    g_foreground = &ours;
    if (!m.IsApplicationActive() || !m.ApplicationActive || !m.EverBeenActive || m.Restore) ok = false;

    // Lose focus: inactive, still "ever been active".
    g_foreground = &other;
    if (m.IsApplicationActive() || m.ApplicationActive || !m.EverBeenActive || m.Restore) ok = false;

    // Regain focus: restore requested.
    g_foreground = &ours;
    if (!m.IsApplicationActive() || !m.ApplicationActive || !m.Restore) ok = false;

    // Already active: no new restore request.
    m.Restore = false;
    if (!m.IsApplicationActive() || m.Restore) ok = false;

    if (!ok) { std::printf("009a57b0_TEST FAIL\n"); return 1; }
    std::printf("009a57b0_TEST PASS\n");
    return 0;
}