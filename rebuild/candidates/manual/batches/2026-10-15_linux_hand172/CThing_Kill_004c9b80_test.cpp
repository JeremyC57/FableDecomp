#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct CThing {
    bool Dead, Dying, KillLocked, DeathPersistent, canKillNow, createOk;
    bool OnCreate() { g_log[g_n++] = 8; return createOk; }
    void InitialActivate() { g_log[g_n++] = 9; }
    void OnInitialActivate() { g_log[g_n++] = 10; }
    void OnKillImmediately() { g_log[g_n++] = 11; }
    void OnKill() { g_log[g_n++] = 12; }
    bool CanBeKilledImmediately() { g_log[g_n++] = 13; return canKillNow; }
    void Update() { g_log[g_n++] = 20; }
    void Kill(bool immediately) {
        if (Dead || Dying || KillLocked) return;
        if (immediately && !CanBeKilledImmediately()) return;
        Dying = true;
        if (immediately) { DeathPersistent = true; OnKillImmediately(); }
        OnKill(); Dead = true; Update();
    }
    bool DoActivationSequence() { if (OnCreate()) { InitialActivate(); OnInitialActivate(); return true; } Kill(false); return false; }
};
int main()
{
    CThing a = { false, false, false, false, true, true };
    a.Kill(true);
    if (!a.Dead || !a.Dying || !a.DeathPersistent || g_n != 4 || g_log[1] != 11 || g_log[2] != 12) { std::printf("004c9b80_TEST FAIL\n"); return 1; }
    g_n = 0; a.Kill(true); if (g_n != 0) { std::printf("004c9b80_TEST FAIL\n"); return 1; }
    CThing b = { false, false, false, false, false, true };
    b.Kill(true); if (b.Dead || g_n != 1) { std::printf("004c9b80_TEST FAIL\n"); return 1; }
    g_n = 0; if (!b.DoActivationSequence() || g_n != 3 || b.Dead) { std::printf("004c9b80_TEST FAIL\n"); return 1; }
    CThing c = { false, false, false, false, false, false };
    g_n = 0; if (c.DoActivationSequence() || !c.Dead || c.DeathPersistent || g_log[1] != 12) { std::printf("004c9b80_TEST FAIL\n"); return 1; }
    std::printf("004c9b80_TEST PASS\n");
    return 0;
}
