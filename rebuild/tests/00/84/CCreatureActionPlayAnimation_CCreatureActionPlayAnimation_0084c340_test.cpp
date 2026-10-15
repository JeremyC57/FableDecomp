// Behaviour test for 0x0084C340 (see src/0084c340.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0084c340_TEST FAIL\n"); return 1; } } while (0)
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

struct CPlayAnimation : CActionBase {
    bool UseAnimationMovement, UsePhysics, StayOnLastFrame, Looping; int NumberOfTimesToLoop; bool Cam;

    CPlayAnimation(void* o) : CActionBase(o, -1.0f, 1.0f, 0, CSoundPair(), 0, 0), UseAnimationMovement(true), UsePhysics(false),
          StayOnLastFrame(true), Looping(false), NumberOfTimesToLoop(6), Cam(true) {}
    CPlayAnimation(const CPlayAnimation& o) : CActionBase(o), UseAnimationMovement(o.UseAnimationMovement), UsePhysics(o.UsePhysics),
          StayOnLastFrame(o.StayOnLastFrame), Looping(o.Looping), NumberOfTimesToLoop(o.NumberOfTimesToLoop), Cam(o.Cam) {} };
int main()
{
    int o; CPlayAnimation src(&o); CPlayAnimation a(src);
    CHECK(a.owner == &o && a.UseAnimationMovement && !a.UsePhysics && a.StayOnLastFrame && !a.Looping && a.NumberOfTimesToLoop == 6 && a.Cam);
    std::printf("0084c340_TEST PASS\n");
    return 0;
}