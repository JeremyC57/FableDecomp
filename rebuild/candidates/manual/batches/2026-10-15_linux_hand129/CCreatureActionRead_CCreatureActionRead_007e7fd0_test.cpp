// Behaviour test for 0x007E7FD0 (see src/007e7fd0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("007e7fd0_TEST FAIL\n"); return 1; } } while (0)
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

struct CWide { int id; CWide(int i) : id(i) {} CWide(const CWide& o) : id(o.id) {} };
struct CRead : CActionBase { CWide TextTag;
    CRead(void* o, const CWide& text) : CActionBase(o, 0.0f, 0.0f, GetAnimationEntry(o, "STANDARD_CAST"), CSoundPair(), 0, 0), TextTag(text) {} };
int main()
{
    int o; CWide t(42); CRead a(&o, t);
    CHECK(a.a == 0.0f && a.b == 0.0f && std::strcmp((const char*)a.anim, "STANDARD_CAST") == 0 && a.TextTag.id == 42 && a.sound.FirstSound == 0);
    std::printf("007e7fd0_TEST PASS\n");
    return 0;
}
