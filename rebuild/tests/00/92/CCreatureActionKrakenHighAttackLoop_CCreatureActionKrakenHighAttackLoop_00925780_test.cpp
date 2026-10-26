// Behaviour test for 0x00925780 (see src/00925780.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00925780_TEST FAIL\n"); return 1; } } while (0)
class CSoundPair { public: int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
class CCombatActionBase { public:
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

struct CHigh : CCombatActionBase { bool flag; int value;
    CHigh(void* owner, void* thing, int v, bool f) : CCombatActionBase(owner, thing, 150, CSoundPair()), flag(f), value(v) { SetAnimationName("ATTACK_HIGH_LOOP"); } };
int main()
{
    int o, t; CHigh a(&o, &t, 9, true);
    CHECK(a.actionCode == 150 && a.sound.FirstSound == 0 && a.sound.LastSound == 0 && a.flag && a.value == 9 && std::strcmp(a.animName, "ATTACK_HIGH_LOOP") == 0);
    std::printf("00925780_TEST PASS\n");
    return 0;
}