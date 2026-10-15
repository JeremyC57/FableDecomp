// Behaviour test for 0x008B7F80 (see src/008b7f80.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008b7f80_TEST FAIL\n"); return 1; } } while (0)
struct CSoundPair { int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
struct CCombatActionBase {
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

struct CPlayCombat : CCombatActionBase { bool flag, firstFrame, started;
    CPlayCombat(void* owner, void* thing, const char* name, bool f) : CCombatActionBase(owner, thing, 50, CSoundPair()), flag(f), firstFrame(true), started(false) { SetAnimationName(name); } };
int main()
{
    int o, t; CPlayCombat a(&o, &t, "SWING", true);
    CHECK(a.actionCode == 50 && a.flag && a.firstFrame && !a.started && std::strcmp(a.animName, "SWING") == 0);
    std::printf("008b7f80_TEST PASS\n");
    return 0;
}