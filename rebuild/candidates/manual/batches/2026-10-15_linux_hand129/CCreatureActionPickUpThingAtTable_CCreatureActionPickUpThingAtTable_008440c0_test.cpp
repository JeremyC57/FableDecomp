// Behaviour test for 0x008440C0 (see src/008440c0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008440c0_TEST FAIL\n"); return 1; } } while (0)
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

struct CPtr { void* item; CPtr(void* p) : item(p) {} };
struct CPickUp : CActionBase { CPtr thing; int point;
    CPickUp(void* owner, void* t, const char* kind, int p) : CActionBase(owner, -1.0f, 0.5f, GetAnimationEntry(owner, kind), CSoundPair(), 0, 0), thing(t), point(p) {} };
int main()
{
    int o, t; CPickUp a(&o, &t, "PICK", 4);
    CHECK(a.a == -1.0f && a.b == 0.5f && std::strcmp((const char*)a.anim, "PICK") == 0);
    CHECK(a.thing.item == &t && a.point == 4 && a.c == 0 && a.d == 0);
    std::printf("008440c0_TEST PASS\n");
    return 0;
}
