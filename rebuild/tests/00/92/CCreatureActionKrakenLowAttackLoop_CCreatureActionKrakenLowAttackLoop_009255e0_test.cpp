// Behaviour test for 0x009255E0 (see src/009255e0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("009255e0_TEST FAIL\n"); return 1; } } while (0)
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

struct CLow : CCombatActionBase { bool flag; int value;
    CLow(void* owner, void* thing, int v, bool f) : CCombatActionBase(owner, thing, 150, CSoundPair()), flag(f), value(v) { SetAnimationName("ATTACK_LOW_LOOP"); } };
int main()
{
    int o, t; CLow a(&o, &t, 9, true);
    CHECK(a.actionCode == 150 && a.sound.FirstSound == 0 && a.sound.LastSound == 0 && a.flag && a.value == 9 && std::strcmp(a.animName, "ATTACK_LOW_LOOP") == 0);
    std::printf("009255e0_TEST PASS\n");
    return 0;
}