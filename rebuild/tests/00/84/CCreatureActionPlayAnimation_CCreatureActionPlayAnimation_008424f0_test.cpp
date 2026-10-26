// Behaviour test for 0x008424F0 (see src/008424f0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008424f0_TEST FAIL\n"); return 1; } } while (0)
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

// Creature-action base model (0x00693B30): owner, two floats, animation, sound, two words.
struct CActionBase {
    void* owner; float a, b; const void* anim; CSoundPair sound; int c, d; int AnimDelayTime;
    CActionBase(void* o, float fa, float fb, const void* an, const CSoundPair& s, int fc, int fd)
        : owner(o), a(fa), b(fb), anim(an), sound(s), c(fc), d(fd), AnimDelayTime(5) {}
};
static const void* GetAnimationEntry(void* owner, const char* name) { return name; }

struct CPlayAnimation : CActionBase {
    bool UseAnimationMovement, UsePhysics, StayOnLastFrame, Looping; int NumberOfTimesToLoop; bool Cam;

    CPlayAnimation(void* o, const char* name, const CSoundPair& s, bool m, bool p, bool cam, int c, int d)
        : CActionBase(o, -1.0f, 1.0f, GetAnimationEntry(o, name), s, c, d), UseAnimationMovement(m), UsePhysics(p),
          StayOnLastFrame(false), Looping(false), NumberOfTimesToLoop(0), Cam(cam) {} };
int main()
{
    int o; CSoundPair s; s.FirstSound = 3; CPlayAnimation a(&o, "WAVE", s, true, false, true, 7, 8);
    CHECK(a.a == -1.0f && a.b == 1.0f && a.sound.FirstSound == 3 && a.c == 7 && a.d == 8);
    CHECK(a.UseAnimationMovement && !a.UsePhysics && !a.StayOnLastFrame && !a.Looping && a.NumberOfTimesToLoop == 0 && a.Cam);
    std::printf("008424f0_TEST PASS\n");
    return 0;
}