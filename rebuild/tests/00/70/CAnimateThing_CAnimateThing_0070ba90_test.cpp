// Behaviour test for CAnimateThing::~CAnimateThing @ 0x0070ba90: members are destroyed in reverse declaration order --
// the anim-from-file path, the four blend states (each drops its shared data reference, deleting it on the last one), the
// delayed animation, the delayed animation queue, then the current complete animation.
#include <stdio.h>
static char g_log[64];
static void logc(char c) { int n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
class CSharedData { public:
    unsigned int IVRefCount; char Tag;
    void Delete() const { logc(Tag); }
};
class CIVCountedPointer { public:
    CSharedData* Object;
    ~CIVCountedPointer() { CSharedData* o = Object; if (o) { if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class C3DAnimationBlendState : public CBaseClass { public: CIVCountedPointer Data; };
class CWideString { public: ~CWideString() { logc('w'); } };
class CAnimFromFileInfo { public: bool Active; CWideString PathName; };
class CAnimSetList { public: char Tag; ~CAnimSetList() { logc(Tag); } };
class CAnimateThing { public:
    class CCompleteAnimation { public: ~CCompleteAnimation() { logc('C'); } };
    class CDelayedAnimation { public: CAnimSetList CombinationAnims; };
    CCompleteAnimation      CurrentAnim;
    CAnimSetList            DelayedAnimQueue;
    CDelayedAnimation       DelayedAnim;
    C3DAnimationBlendState  StartAnimState;
    C3DAnimationBlendState  EndAnimState;
    C3DAnimationBlendState  LastStartAnimState;
    C3DAnimationBlendState  LastEndAnimState;
    float                   Scale;
    CAnimFromFileInfo       AnimFromFileInfo;
    ~CAnimateThing() { }
};
int main()
{
    CSharedData a, b, c, d; a.IVRefCount = 1; a.Tag = '1'; b.IVRefCount = 2; b.Tag = '2'; c.IVRefCount = 1; c.Tag = '3'; d.IVRefCount = 1; d.Tag = '4';
    {
        CAnimateThing t;
        t.DelayedAnimQueue.Tag = 'q'; t.DelayedAnim.CombinationAnims.Tag = 'd';
        t.StartAnimState.Data.Object = &a; t.EndAnimState.Data.Object = &b;
        t.LastStartAnimState.Data.Object = &c; t.LastEndAnimState.Data.Object = 0;
    }
    const char* want = "w31dqC";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { printf("0070ba90_TEST FAIL\n"); return 1; }
    if (b.IVRefCount != 1 || a.IVRefCount != 0) { printf("0070ba90_TEST FAIL\n"); return 1; }
    printf("0070ba90_TEST PASS\n");
    return 0;
}