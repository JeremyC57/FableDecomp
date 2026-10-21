// Behaviour test for 0x0079D110 CTCBulletTime::~CTCBulletTime: the slowdown and loop sound stop, the screen filter is
// removed (mode 0 on a cutscene cancel, else the def's mode plus the def's end sound), a live looping effect is killed,
// the def reference is dropped and the primitive handle's last reference is released.
#include <cstdio>

static int g_slowdownRemoved = 0, g_stopped = 0, g_filterMode = -1, g_played = 0, g_killed = 0, g_defDestroyed = 0;
static int g_primLast = 0, g_primDestroyed = 0, g_baseDtor = 0;

struct CDef { long RefCount; long EndSoundStringID; long FilterRemoveMode; };
struct CPrim { long RefCount; };
struct CEmitter { bool Dead; void Kill() { Dead = true; ++g_killed; } };
struct CTCBase { int* thingBits; virtual ~CTCBase() { ++g_baseDtor; } };

struct CTCBulletTime : CTCBase {
    CPrim* Primitive;
    CDef* PDef;
    CEmitter* LoopingEffect;
    unsigned long LoopSound;
    void* ScreenFilterKey;
    bool CutSceneCancel;
    void PlaySound(long id) { g_played = (int)id; }
    virtual ~CTCBulletTime()
    {
        ++g_slowdownRemoved;
        if (*thingBits & 0x10000)
            g_stopped = (int)LoopSound;
        if (CutSceneCancel) {
            g_filterMode = 0;
        } else {
            g_filterMode = (int)PDef->FilterRemoveMode;
            PlaySound(PDef->EndSoundStringID);
        }
        if (LoopingEffect && !LoopingEffect->Dead) {
            LoopingEffect->Kill();
            LoopingEffect = 0;
        }
        if (PDef) {
            if (--PDef->RefCount == 0)
                ++g_defDestroyed;
            PDef = 0;
        }
        if (Primitive && Primitive->RefCount == 1)
            ++g_primLast;
        if (Primitive) {
            if (--Primitive->RefCount == 0)
                ++g_primDestroyed;
            Primitive = 0;
        }
    }
};

int main()
{
    int bits = 0x10000;
    CDef def = { 1, 77, 3 };
    CPrim prim = { 1 };
    CEmitter emitter = { false };
    CTCBulletTime* b = new CTCBulletTime;
    b->thingBits = &bits; b->Primitive = &prim; b->PDef = &def; b->LoopingEffect = &emitter;
    b->LoopSound = 42; b->ScreenFilterKey = 0; b->CutSceneCancel = false;
    CTCBase* base = b;
    delete base;
    int ok = g_slowdownRemoved == 1 && g_stopped == 42 && g_filterMode == 3 && g_played == 77 && g_killed == 1
        && g_defDestroyed == 1 && g_primLast == 1 && g_primDestroyed == 1 && g_baseDtor == 1;

    int noBits = 0;
    CDef def2 = { 2, 5, 9 };
    CEmitter dead = { true };
    g_played = 0; g_stopped = 0;
    CTCBulletTime* c = new CTCBulletTime;
    c->thingBits = &noBits; c->Primitive = 0; c->PDef = &def2; c->LoopingEffect = &dead;
    c->LoopSound = 7; c->ScreenFilterKey = 0; c->CutSceneCancel = true;
    base = c;
    delete base;
    ok = ok && g_stopped == 0 && g_filterMode == 0 && g_played == 0 && g_killed == 1 && def2.RefCount == 1
        && g_defDestroyed == 1 && g_baseDtor == 2;
    if (!ok) {
        std::printf("0079d110_TEST FAIL\n");
        return 1;
    }
    std::printf("0079d110_TEST PASS\n");
    return 0;
}
