// Behaviour test for CGlobal_OpenChestScript::Main (0x00EEC890): locked-chest branch, open/cancel loop, reward wait,
// weapon re-draw and the final quest deactivation.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Sim {
    bool controlled, chestAlive;
    int keysNeeded, keysHeld;
    int openAfter;      // frames until the chest reports open (-1 never)
    int cancelAt;       // frame at which the opening is cancelled (-1 never)
    int hitAt;          // frame at which the hero is hit (-1 never)
    int items;
    bool melee, ranged;
    int frame;
};
static Sim s;
struct Script {
    bool IsActiveThreadTerminating() { return s.frame > 50; }
    void NewScriptFrame() { ++s.frame; }
    bool IsChestOpen() { return s.openAfter >= 0 && s.frame >= s.openAfter; }
    bool Cancelled() { return s.frame == s.cancelAt; }
    bool HitBy() { return s.frame == s.hitAt; }
    void Main();
};
void Script::Main()
{
    if (s.controlled && s.chestAlive) {
        bool canOpen = true;
        if (s.keysNeeded > 0 && s.keysHeld < s.keysNeeded)
            canOpen = false;
        if (canOpen) {
            Log("pause");
            bool hadItems = s.items != 0;
            bool cancelled = true;
            {
                cancelled = Cancelled();
                while (!IsChestOpen() && !cancelled) {
                    NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    cancelled = Cancelled();
                    if (!cancelled)
                        cancelled = HitBy();
                }
            }
            Log("unpause");
            if (hadItems && !cancelled) Log("reward");
            if (s.melee) Log("melee");
            else if (s.ranged) Log("ranged");
        } else {
            Log("CHEST_OPEN_FAIL");
        }
    }
    Log("deactivate");
}
static bool Run(const char* expect)
{
    s.frame = 0; g_log[0] = 0;
    Script sc; sc.Main();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    Sim base = { true, true, 0, 0, 3, -1, -1, 1, true, false, 0 };
    bool ok = true;
    s = base; ok = ok && Run("pause;unpause;reward;melee;deactivate;");
    s = base; s.keysNeeded = 2; s.keysHeld = 1; ok = ok && Run("CHEST_OPEN_FAIL;deactivate;");
    s = base; s.keysNeeded = 2; s.keysHeld = 2; s.melee = false; s.ranged = true; ok = ok && Run("pause;unpause;reward;ranged;deactivate;");
    s = base; s.cancelAt = 1; ok = ok && Run("pause;unpause;melee;deactivate;");
    s = base; s.hitAt = 2; s.melee = false; ok = ok && Run("pause;unpause;deactivate;");
    s = base; s.controlled = false; ok = ok && Run("deactivate;");
    if (!ok) {
        std::printf("00eec890_TEST FAIL\n");
        return 1;
    }
    std::printf("00eec890_TEST PASS\n");
    return 0;
}
