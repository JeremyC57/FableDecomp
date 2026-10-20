// Behaviour test for 008d6da0: body copied from src/008d6da0.cpp (CModeConversation::AnimUpdate), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_ctor = 0, g_dtor = 0, g_lookups = 0, g_flagsCalls = 0, g_blendCalls = 0, g_transCalls = 0, g_setMain = 0, g_actionCalls = 0;
static bool g_acting = false; static bool g_lookupNull = false; static long g_blendReturn = 9;
static const char* g_lastName = 0; static bool g_lastLoaded = true; static const void* g_blendThing = 0;
static long g_setType = 0, g_setAnim = 0, g_setLoop = 0, g_setPri = 0, g_setFlags = 0, g_blend = -1, g_trans = -1;
struct CThing { unsigned char _pad0[0x50]; };
struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) { }
    CCharString(const char* text, long length = -1) { m_pData = (void*)text; ++g_ctor; }
    ~CCharString() { ++g_dtor; }
};
struct CAnimationEntry;
static CAnimationEntry* g_entry;
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction() { ++g_actionCalls; return g_acting; }
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loadedOnly) { ++g_lookups; g_lastName = (const char*)name.m_pData; g_lastLoaded = loadedOnly; return g_lookupNull ? 0 : g_entry; }
};
struct CBlendPair { float m_first; float m_second; CBlendPair() : m_first(0.0f), m_second(0.0f) { } };
struct CAnimInit {
    long Mode; unsigned char AnimType; bool Flag05; long AnimIndex; float Scales[3]; CBlendPair Blend; unsigned long Flags; long Arg24; long Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent; }
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { Set(animType, arg24, animIndex, mode, flag05, flags, percent); }
};
struct CAnimationEntry {
    long AnimIndex;
    unsigned long GetAnimateThingFlags() { ++g_flagsCalls; return 0x55; }
    long GetAnimationTransitionInTime() { ++g_transCalls; return 3; }
    long GetTimeToBlendIn(CThing* thing) { ++g_blendCalls; g_blendThing = thing; return g_blendReturn; }
};
struct CAnimateThing {
    long UpdateMode; void* AnimsBegin; void* AnimsEnd; unsigned char _pad0c[0x44]; unsigned char CurrentAnimType; unsigned char CurrentAnimFlags;
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition)
    { ++g_setMain; g_setType = init.AnimType; g_setAnim = init.AnimIndex; g_setLoop = init.Flag05; g_setPri = init.Percent; g_setFlags = init.Flags; g_blend = blendIn; g_trans = transition; }
};
struct CModeBase { virtual void v00() {} CThingCreatureBase* ParentThing; unsigned char _body[0x14]; };
struct CAnimFilterOfGroup { virtual void v00() {} CCharString Group; unsigned long GroupCRC; };
enum EConversationAttitude { CONVERSATION_ATTITUDE_NEUTRAL = 0 };
namespace NCreatureMode {

struct CModeConversation : CModeBase {
    EConversationAttitude CurrentAttitude; // +0x1c
    EConversationAttitude InitialAttitude; // +0x20
    CAnimFilterOfGroup    Filter;          // +0x24
    bool                  ResetIdleAnim;   // +0x30
    void AnimUpdate(CAnimateThing& animate);
};

void CModeConversation::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = ParentThing;
    if (creature->IsPerformingAnyAction())
        return;
    if (animate.AnimsBegin != animate.AnimsEnd && !(animate.CurrentAnimFlags & 2) && animate.CurrentAnimType == 0x1c
        && !(animate.CurrentAnimFlags & 1) && !ResetIdleAnim)
        return;
    CAnimationEntry* entry;
    {
        CCharString name("ST_IDLE");
        entry = creature->GetRandomLoadedAnimFromName(name, false);
    }
    if (entry) {
        CThing* thing = ParentThing;
        long blendIn = entry->GetTimeToBlendIn(thing);
        if (animate.CurrentAnimType != 0x1c && blendIn < 7)
            blendIn = 7;
        animate.SetMainAnim(CAnimInit(0x1c, 0, entry->AnimIndex, 1, false, entry->GetAnimateThingFlags(), 100), blendIn, entry->GetAnimationTransitionInTime());
    }
    ResetIdleAnim = false;
}

}

static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static void reset() { g_ctor = g_dtor = g_lookups = g_flagsCalls = g_blendCalls = g_transCalls = g_setMain = g_actionCalls = 0; g_lastName = 0; g_blendThing = 0; g_lookupNull = false; g_acting = false; g_blendReturn = 9; }
int main()
{
    CAnimationEntry entry = { 77 }; g_entry = &entry;
    CThingCreatureBase creature; NCreatureMode::CModeConversation mode; memset(&mode, 0, sizeof(mode)); mode.ParentThing = &creature;
    CAnimateThing anim; memset(&anim, 0, sizeof(anim)); long anims[2];
    // performing an action -> nothing, flag untouched
    reset(); g_acting = true; mode.ResetIdleAnim = true; mode.AnimUpdate(anim); CHECK(g_actionCalls == 1 && g_lookups == 0 && g_setMain == 0 && mode.ResetIdleAnim == true);
    // idle already looping, no reset asked -> nothing but the flag cleared
    reset(); anim.AnimsBegin = anims; anim.AnimsEnd = anims + 1; anim.CurrentAnimFlags = 0; anim.CurrentAnimType = 0x1c; mode.ResetIdleAnim = false;
    mode.AnimUpdate(anim); CHECK(g_lookups == 0 && g_setMain == 0 && mode.ResetIdleAnim == false);
    // reset asked -> restart idle; same type so blend-in is not clamped
    reset(); mode.ResetIdleAnim = true; mode.AnimUpdate(anim);
    CHECK(g_lookups == 1 && g_ctor == 1 && g_dtor == 1 && strcmp(g_lastName, "ST_IDLE") == 0 && g_lastLoaded == false);
    CHECK(g_setMain == 1 && g_blendCalls == 1 && g_blendThing == (CThing*)&creature && g_blend == 9 && g_trans == 3 && g_flagsCalls == 1);
    CHECK(g_setType == 0x1c && g_setAnim == 77 && g_setLoop == 0 && g_setPri == 100 && g_setFlags == 0x55 && mode.ResetIdleAnim == false);
    // other type with a short blend-in -> clamped to 7
    reset(); anim.CurrentAnimType = 5; g_blendReturn = 2; mode.AnimUpdate(anim); CHECK(g_setMain == 1 && g_blend == 7);
    // other type with a long blend-in -> kept
    reset(); g_blendReturn = 20; mode.AnimUpdate(anim); CHECK(g_setMain == 1 && g_blend == 20);
    // finished flag -> restart; no entry found -> no SetMainAnim but flag cleared
    reset(); anim.CurrentAnimType = 0x1c; anim.CurrentAnimFlags = 2; g_lookupNull = true; mode.ResetIdleAnim = true; mode.AnimUpdate(anim);
    CHECK(g_lookups == 1 && g_setMain == 0 && mode.ResetIdleAnim == false);
    // empty anim list -> restart
    reset(); anim.CurrentAnimFlags = 0; anim.AnimsEnd = anims; mode.AnimUpdate(anim); CHECK(g_setMain == 1);
    // bit 0 set -> restart
    reset(); anim.AnimsEnd = anims + 1; anim.CurrentAnimFlags = 1; mode.AnimUpdate(anim); CHECK(g_setMain == 1);
    if (fails == 0) printf("008d6da0_TEST PASS\n"); else printf("008d6da0_TEST FAIL\n");
    return fails;
}