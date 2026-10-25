// Behaviour test for 0x007676A0 CTCObjectAugmentations::~CTCObjectAugmentations(): the body runs
// ClearAllParticleEffects, then members are torn down in reverse declaration order and the CTCBase part last.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

static char g_Log[64];
static int g_LogLen = 0;
static void Log(char c) { if (g_LogLen < 63) { g_Log[g_LogLen++] = c; g_Log[g_LogLen] = 0; } }

struct Def {
    int refs;
    int destroyed;
    void Destroy() const { const_cast<Def*>(this)->destroyed = 1; }
    void ReleaseReference() const { if (--const_cast<Def*>(this)->refs == 0) Destroy(); }
};

struct DefPtr {
    const Def* Object;
    explicit DefPtr(const Def* d) : Object(d) { if (Object) ++const_cast<Def*>(Object)->refs; }
    ~DefPtr() { if (Object) { Object->ReleaseReference(); Object = 0; } Log('P'); }
};

struct Tagged { char c; explicit Tagged(char x) : c(x) { } ~Tagged() { Log(c); } };

struct Base { ~Base() { Log('B'); } };

struct Augs : Base {
    DefPtr PDef;                                            // +0x0C
    Tagged Augmentations;                                   // +0x14
    Tagged InitSounds;                                      // +0x20
    Tagged LoopingSounds;                                   // +0x2C
    Tagged ParticleEffects;                                 // +0x38
    Tagged AttackParticleEffects;                           // +0x44
    Tagged CurrentRunningAugEffect;                         // +0x50
    Tagged ProjectileTrailEffect;                           // +0x65
    explicit Augs(const Def* d)
        : PDef(d), Augmentations('a'), InitSounds('i'), LoopingSounds('l'), ParticleEffects('p'),
          AttackParticleEffects('k'), CurrentRunningAugEffect('r'), ProjectileTrailEffect('t') { }
    void ClearAllParticleEffects() { Log('C'); }
    ~Augs() { ClearAllParticleEffects(); }
};

int main()
{
    int ok = 1;
    Def d = { 0, 0 };
    {
        Augs a(&d);
        if (d.refs != 1) ok = 0;
    }
    if (std::strcmp(g_Log, "CtrkpliaPB") != 0) ok = 0;     // body, then reverse member order, then the base
    if (d.refs != 0 || d.destroyed != 1) ok = 0;            // the def pointer's last reference destroys the def
    Def shared = { 0, 0 };
    DefPtr keep(&shared);
    g_LogLen = 0;
    {
        Augs b(&shared);
    }
    if (shared.refs != 1 || shared.destroyed != 0) ok = 0;   // still referenced elsewhere: only released
    if (!ok) { std::printf("007676a0_TEST FAIL\n"); return 1; }
    std::printf("007676a0_TEST PASS\n");
    return 0;
}