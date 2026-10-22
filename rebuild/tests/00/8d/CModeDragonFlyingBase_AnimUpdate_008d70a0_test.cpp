#include <stdio.h>
#include <string.h>
// CModeDragonFlyingBase::AnimUpdate(CAnimateThing&) -- keep the dragon's FLY_LOOP main anim and FLYING_LEG_POSE combination anim playing
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D70A0, 289 bytes, `ret 4`.
// Shape: the blend-in thing is a named re-read of ParentThing taken right after the FLY_LOOP lookup (retail loads it into
//   the dead `this` register ebx before the CAnimInit build); the combination anim found after AddCombinationAnim is a
//   named local, so its lookup call runs before the SetInternalAlpha argument pushes.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThing;
struct CAnimationEntry {
    long Index;                                // +0x00
    unsigned long GetAnimateThingFlags();      // 0x005DC390
    long GetTimeToBlendIn(CThing* thing);      // 0x005DC230
};
struct CThing { };
struct CThingCreatureBase : CThing {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};
struct CBlendPair {                            // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                             // 0x2c bytes
    long          Mode;                        // +0x00
    unsigned char AnimType;                    // +0x04
    bool          Flag05;                      // +0x05
    long          AnimIndex;                   // +0x08
    float         Scales[3];                   // +0x0c
    CBlendPair    Blend;                       // +0x18
    unsigned long Flags;                       // +0x20
    long          Arg24;                       // +0x24
    long          Percent;                     // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimSet {
    void SetInternalAlpha(long index, float alpha, bool b);   // 0x0070C280
};
struct CAnimateThing {
    unsigned char _pad00[0x04];
    void*         m_queueBegin;                // +0x04
    void*         m_queueEnd;                  // +0x08
    unsigned char _pad0c[0x50 - 0x0c];
    unsigned char CurrentAnimType;             // +0x50 (main CAnimSet)
    unsigned char CurrentAnimFlags;            // +0x51
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);   // 0x0070D580
    CAnimSet* FindCombinationAnim(long type);  // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
};
struct CModeBase {                             // CTCCreatureModeManager::CModeBase
    virtual void v00();
    CThingCreatureBase* ParentThing;           // +0x04
};
struct CModeDragonFlyingBase : CModeBase {
    void AnimUpdate(CAnimateThing& animate);
};

void CModeDragonFlyingBase::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = ParentThing;
    if (animate.m_queueBegin == animate.m_queueEnd || (animate.CurrentAnimFlags & 2) || animate.CurrentAnimType != 9) {
        CCharString flyLoop("FLY_LOOP");
        CAnimationEntry* entry = creature->GetAnimFromName(flyLoop, 0);
        CThingCreatureBase* parent = ParentThing;
        animate.SetMainAnim(CAnimInit(9, 0, entry->Index, 1, false, entry->GetAnimateThingFlags(), 100), entry->GetTimeToBlendIn(parent), 0);
    }
    CCharString legPose("FLYING_LEG_POSE");
    CAnimationEntry* pose = creature->GetAnimFromName(legPose, 0);
    if (pose->Index != -1 && animate.FindCombinationAnim(0x22) == 0) {
        animate.AddCombinationAnim(CAnimInit(0x22, 0, pose->Index, 0, true, 0, 100), 0, 0, false, false);
        CAnimSet* combination = animate.FindCombinationAnim(0x22);
        combination->SetInternalAlpha(0, 0.0f, false);
    }
}

// ---- test stubs ----
static char g_log[512];
static void logs(const char* t) { strcat(g_log, t); strcat(g_log, ";"); }
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; }
CCharString::~CCharString() { }
static CAnimationEntry g_fly, g_pose;
static unsigned long g_flyFlags = 0x40;
unsigned long CAnimationEntry::GetAnimateThingFlags() { return this == &g_fly ? g_flyFlags : 0; }
static CThing* g_blendThing;
long CAnimationEntry::GetTimeToBlendIn(CThing* thing) { g_blendThing = thing; return 7; }
static CThingCreatureBase* g_queried;
CAnimationEntry* CThingCreatureBase::GetAnimFromName(const CCharString& name, long index)
{
    g_queried = this;
    const char* t = (const char*)name.m_pData;
    if (index != 0) return 0;
    if (strcmp(t, "FLY_LOOP") == 0) { logs("getfly"); return &g_fly; }
    if (strcmp(t, "FLYING_LEG_POSE") == 0) { logs("getpose"); return &g_pose; }
    return 0;
}
void CAnimInit::Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
{
    AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent;
}
static CAnimInit g_main(0, 0, 0, 0, false, 0, 0), g_comb(0, 0, 0, 0, false, 0, 0); static long g_mainBlend, g_mainDelay; static int g_combArgsOk;
static CAnimSet g_set; static int g_haveComb, g_findCalls; static long g_alphaIdx; static float g_alpha; static int g_alphaB = -1;
void CAnimSet::SetInternalAlpha(long index, float alpha, bool b) { g_alphaIdx = index; g_alpha = alpha; g_alphaB = b; logs(this == &g_set ? "alpha" : "alpha?"); }
void CAnimateThing::SetMainAnim(const CAnimInit& init, long blendIn, long delay) { g_main = init; g_mainBlend = blendIn; g_mainDelay = delay; logs("setmain"); }
CAnimSet* CAnimateThing::FindCombinationAnim(long type) { g_findCalls++; return (type == 0x22 && g_haveComb) ? &g_set : 0; }
void CAnimateThing::AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d)
{
    g_comb = init; g_combArgsOk = (a == 0 && b == 0 && !c && !d); g_haveComb = 1; logs("addcomb");
}
void CModeBase::v00() { }
static int fails;
static void check(int ok, const char* what) { if (!ok) { fails++; printf("FAIL %s log=%s\n", what, g_log); } }
static void reset() { g_log[0] = 0; g_haveComb = 0; g_findCalls = 0; g_blendThing = 0; g_alphaB = -1; memset(&g_main, 0, sizeof g_main); memset(&g_comb, 0, sizeof g_comb); }
int main()
{
    CThingCreatureBase dragon; CModeDragonFlyingBase mode; mode.ParentThing = &dragon;
    CAnimateThing anim; memset(&anim, 0, sizeof anim);
    g_fly.Index = 12; g_pose.Index = 5;
    // 1: empty queue -> main anim set; no combination yet -> added and alpha zeroed
    reset(); anim.m_queueBegin = anim.m_queueEnd = 0; anim.CurrentAnimType = 9;
    mode.AnimUpdate(anim);
    check(strcmp(g_log, "getfly;setmain;getpose;addcomb;alpha;") == 0, "seq1");
    check(g_main.AnimType == 9 && g_main.AnimIndex == 12 && g_main.Mode == 1 && !g_main.Flag05 && g_main.Flags == 0x40 && g_main.Percent == 100 && g_main.Arg24 == 0, "main");
    check(g_mainBlend == 7 && g_mainDelay == 0 && g_blendThing == &dragon, "blend");
    check(g_comb.AnimType == 0x22 && g_comb.AnimIndex == 5 && g_comb.Mode == 0 && g_comb.Flag05 && g_comb.Flags == 0 && g_comb.Percent == 100 && g_combArgsOk, "comb");
    check(g_alphaIdx == 0 && g_alpha == 0.0f && g_alphaB == 0 && g_findCalls == 2, "alpha");
    // 2: queue busy, flying main anim type 9 without flag 2 -> no main; combination exists -> nothing
    reset(); anim.m_queueBegin = (void*)1; anim.m_queueEnd = (void*)2; anim.CurrentAnimFlags = 0; anim.CurrentAnimType = 9; g_haveComb = 1;
    mode.AnimUpdate(anim);
    check(strcmp(g_log, "getpose;") == 0 && g_findCalls == 1, "steady");
    // 3: flag 2 forces the main anim
    reset(); anim.CurrentAnimFlags = 2; g_haveComb = 1; mode.AnimUpdate(anim); check(strcmp(g_log, "getfly;setmain;getpose;") == 0, "flag2");
    // 4: another main type forces it too
    reset(); anim.CurrentAnimFlags = 0; anim.CurrentAnimType = 3; g_haveComb = 1; mode.AnimUpdate(anim); check(strcmp(g_log, "getfly;setmain;getpose;") == 0, "type");
    // 5: pose index -1 -> no combination lookup at all
    reset(); anim.CurrentAnimType = 9; g_pose.Index = -1; mode.AnimUpdate(anim); check(strcmp(g_log, "getpose;") == 0 && g_findCalls == 0, "nopose");
    if (!fails) printf("008d70a0_TEST PASS\n");
    return fails;
}