// Behaviour test for CMuteGuardOutsideWoods::Main (0x00D86150): wait for a hero hit or the parent's attack flag, then attack.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Guard {
    bool outsideWoodGuardsAttack;
    int frames, terminateAt, hitAt, specialAt, special14;
    bool scripting;
    void NewScriptFrame() { ++frames; Log("f"); }
    bool IsActiveThreadTerminating() const { return frames >= terminateAt; }
    bool StartScriptingEntity() { return true; }
    bool MsgIsHitBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && frames == hitAt; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) const { return frames == specialAt; }
    bool MsgIsHitBySpecialAbilityFrom(long ability, const char*) const { return ability == 14 && special14; }
    void Main()
    {
        Log("pred");
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (scripting) { scripting = false; Log("rel"); }
        while (!StartScriptingEntity()) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!(MsgIsHitBy("SCRIPT_NAME_HERO")
                 || (MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                     && !MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))
                 || outsideWoodGuardsAttack)) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        outsideWoodGuardsAttack = true;
        Log("target");
        scripting = true;
        if (scripting) { scripting = false; Log("rel"); }
        do {
            NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};

int main()
{
    Guard g; g.outsideWoodGuardsAttack = false; g.frames = 0; g.terminateAt = 8; g.hitAt = 3; g.specialAt = 2; g.special14 = 1; g.scripting = false;
    g_log[0] = 0; g.Main();
    if (std::strcmp(g_log, "pred,f,f,f,target,rel,f,f,f,f,f,") != 0 || !g.outsideWoodGuardsAttack) { std::printf("%s 00d86150_TEST FAIL\n", g_log); return 1; }
    Guard h = g; h.outsideWoodGuardsAttack = false; h.frames = 0; h.hitAt = 99; h.specialAt = 2; h.special14 = 0;
    g_log[0] = 0; h.Main();
    if (std::strcmp(g_log, "pred,f,f,target,rel,f,f,f,f,f,f,") != 0) { std::printf("%s 00d86150_TEST FAIL\n", g_log); return 1; }
    Guard k = g; k.outsideWoodGuardsAttack = false; k.frames = 0; k.hitAt = 99; k.specialAt = 99; k.terminateAt = 4;
    g_log[0] = 0; k.Main();
    if (std::strcmp(g_log, "pred,f,f,f,f,") != 0 || k.outsideWoodGuardsAttack) { std::printf("%s 00d86150_TEST FAIL\n", g_log); return 1; }
    std::printf("00d86150_TEST PASS\n");
    return 0;
}
