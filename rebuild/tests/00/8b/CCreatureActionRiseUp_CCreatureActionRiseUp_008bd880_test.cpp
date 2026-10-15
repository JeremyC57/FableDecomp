// Behaviour test for 0x008BD880 (see src/008bd880.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008bd880_TEST FAIL\n"); return 1; } } while (0)
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

struct CRiseUp : CCombatActionBase { bool hasTarget;
    CRiseUp(void* owner) : CCombatActionBase(owner, 240, GetSoundPair(owner, 0)) { SetAnimationName("RISE_UP"); hasTarget = false; } };
int main()
{
    int o; CRiseUp a(&o);
    CHECK(a.owner == &o && a.thing == 0 && a.actionCode == 240 && a.sound.LastSound == -1);
    CHECK(std::strcmp(a.animName, "RISE_UP") == 0 && !a.hasTarget);
    std::printf("008bd880_TEST PASS\n");
    return 0;
}