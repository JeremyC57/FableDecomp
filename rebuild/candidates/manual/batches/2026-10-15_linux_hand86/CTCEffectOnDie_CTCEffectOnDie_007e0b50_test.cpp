// Behaviour test for CTCEffectOnDie::CTCEffectOnDie(CThing*) @ 0x007E0B50: flags and timers cleared, the criteria
// string default-constructed, Pausable true, Paused false.
#include <cstdio>
#include <new>
struct CThing { int id; };
static CThing* g_baseParent = 0;
static int g_stringCtors = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) { ++g_stringCtors; }
};
struct CTCEffectOnDie : CTCBase_Calls {
    bool        StartedNonRecoverableFadeout;
    long        Effect;
    long        Timer;
    long        FadeOutTimer;
    CCharString DeathSoundCriteria;
    bool        Pausable;
    bool        Paused;
    virtual void v00() { }
    CTCEffectOnDie(CThing* parent);
};
CTCEffectOnDie::CTCEffectOnDie(CThing* parent)
    : CTCBase_Calls(parent), StartedNonRecoverableFadeout(false), Effect(0), Timer(0), FadeOutTimer(0),
      DeathSoundCriteria(), Pausable(true), Paused(false) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCEffectOnDie)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCEffectOnDie* e = new (u.raw) CTCEffectOnDie(&thing);
    if (g_baseParent != &thing || e->StartedNonRecoverableFadeout || e->Effect != 0 || e->Timer != 0 || e->FadeOutTimer != 0
        || g_stringCtors != 1 || e->DeathSoundCriteria.m_pData != 0 || !e->Pausable || e->Paused)
        { std::printf("007e0b50_TEST FAIL\n"); return 1; }
    std::printf("007e0b50_TEST PASS\n");
    return 0;
}
