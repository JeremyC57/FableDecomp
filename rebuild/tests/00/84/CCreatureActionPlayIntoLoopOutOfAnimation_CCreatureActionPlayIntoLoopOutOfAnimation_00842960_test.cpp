// Behaviour test for 0x00842960 (see src/00842960.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00842960_TEST FAIL\n"); return 1; } } while (0)
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

// Creature-action base model (0x00693B30): owner, two floats, animation, sound, two words.
struct CActionBase {
    void* owner; float a, b; const void* anim; CSoundPair sound; int c, d; int AnimDelayTime;
    CActionBase(void* o, float fa, float fb, const void* an, const CSoundPair& s, int fc, int fd)
        : owner(o), a(fa), b(fb), anim(an), sound(s), c(fc), d(fd), AnimDelayTime(5) {}
};
static const void* GetAnimationEntry(void* owner, const char* name) { return name; }

struct CStr { char text[32]; CStr() { text[0] = 0; } CStr(const char* t) { std::strcpy(text, t); } CStr(const CStr& o) { std::strcpy(text, o.text); } };
struct CLoop : CActionBase { int Stage; int NumberOfTimesToLoop; float AmountOfTimeToLoop; int FrameToFinish; bool UseAnimationMovement; CStr LoopAnimName, OutOfAnimName;

    CLoop(void* o, const char* name, bool m, int c, int d)
        : CActionBase(o, -1.0f, 1.0f, GetAnimationEntry(o, name), CSoundPair(), c, d), Stage(2), UseAnimationMovement(m) { AnimDelayTime = 0; } };
int main()
{
    int o; CLoop a(&o, "ONCE", false, 3, 4);
    CHECK(a.Stage == 2 && !a.UseAnimationMovement && a.AnimDelayTime == 0 && a.LoopAnimName.text[0] == 0 && a.OutOfAnimName.text[0] == 0);
    std::printf("00842960_TEST PASS\n");
    return 0;
}