// Behaviour test for 0x00976F30 (see src/00976f30.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00976f30_TEST FAIL\n"); return 1; } } while (0)
class CSoundPair { public: int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
class CCombatActionBase { public:
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetCombatAnimation(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

struct CJackShotFloat : CCombatActionBase {
    CJackShotFloat(void* owner, void* thing, const char* name) : CCombatActionBase(owner, thing, 250, GetSoundPair(owner, "SWORD_SWING")) { SetCombatAnimation(name); } };
int main()
{
    int o, t; CJackShotFloat a(&o, &t, "SHOT_FLOAT");
    CHECK(a.owner == &o && a.thing == &t && a.actionCode == 250 && a.sound.LastSound == 11);
    CHECK(std::strcmp(a.animName, "SHOT_FLOAT") == 0);
    std::printf("00976f30_TEST PASS\n");
    return 0;
}