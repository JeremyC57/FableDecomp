// Behaviour test for 0x00977100 (see src/00977100.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00977100_TEST FAIL\n"); return 1; } } while (0)
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

struct CPtr { void* Data; void* Info; CPtr() : Data(0), Info(0) {} };
struct CScream : CCombatActionBase { CPtr mode;
    CScream(void* owner) : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")) { SetAnimationName("SCREAM_INTO"); } };
int main()
{
    int o; CScream a(&o);
    CHECK(a.actionCode == 250 && a.thing == 0 && a.mode.Data == 0 && a.mode.Info == 0 && std::strcmp(a.animName, "SCREAM_INTO") == 0);
    std::printf("00977100_TEST PASS\n");
    return 0;
}