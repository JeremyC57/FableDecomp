// Behaviour test for 008d6780: body copied from src/008d6780.cpp (CModeExplosiveForcePushed::AnimUpdate), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_ctor = 0, g_dtor = 0, g_lookups = 0, g_filterCalls = 0, g_setMain = 0;
static const char* g_lastName = 0; static const void* g_lastFilter = 0; static long g_lastFlags = -1;
static long g_setType = 0, g_setAnim = 0, g_setLoop = 0, g_setPri = 0, g_blend = -1, g_delay = -1;
struct CThing;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1) { m_pData = (void*)text; ++g_ctor; }
    ~CCharString() { ++g_dtor; }
};
struct CAnimFilterBase { int tag; };
struct CAnimationEntry { long BankIndex; unsigned char _rest[0xc]; };
struct CAnimFrameInfo { long m_frame; long m_subFrame; CAnimFrameInfo() : m_frame(0), m_subFrame(0) { } };
struct CAnimInit {
    long UpdateMode; unsigned char Type; bool Looping; long Anim; float Speed; float Alpha; float LastAlpha;
    CAnimFrameInfo Frame; unsigned long Flags; unsigned long TypeFlags; long Priority;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    { Set(type, typeFlags, anim, updateMode, looping, flags, priority); }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    { Type = type; TypeFlags = typeFlags; Anim = anim; UpdateMode = updateMode; Looping = looping; Flags = flags; Priority = priority; }
};
struct CAnimateThing {
    long UpdateMode; short* m_animsFirst; short* m_animsLast; unsigned char _pad0c[0x44]; unsigned char Type; unsigned char m_stateBits;
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay)
    { ++g_setMain; g_setType = init.Type; g_setAnim = init.Anim; g_setLoop = init.Looping; g_setPri = init.Priority; g_blend = blendIn; g_delay = delay; }
};
static CAnimationEntry g_entry = { 77 };
struct CThingCreatureBase {
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase* filter, long flags)
    { ++g_lookups; g_lastName = (const char*)name.m_pData; g_lastFilter = filter; g_lastFlags = flags; return &g_entry; }
};
static CAnimFilterBase g_filter = { 5 };
struct CModeBase {
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual const CAnimFilterBase* GetAnimFilter() { ++g_filterCalls; return &g_filter; }
    CThingCreatureBase* ParentThing;
};
namespace NCreatureMode {
struct CModeExplosiveForcePushed : CModeBase { void AnimUpdate(CAnimateThing& anim); };
void CModeExplosiveForcePushed::AnimUpdate(CAnimateThing& anim)
{
    long type = anim.Type;
    if (type == 1) {
        if (!(anim.m_stateBits & 2))
            return;
    }
    else if (type == 0x1d)
        return;
    CThingCreatureBase* parent = ParentThing;
    CAnimationEntry* entry;
    {
        CCharString name("ST_IDLE_SUBTLE");
        entry = parent->GetAnimFromName(name, GetAnimFilter(), 0);
    }
    anim.SetMainAnim(CAnimInit(0x1d, 0, entry->BankIndex, 1, true, 0, 100), 0, 0);
}
}
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static void reset() { g_ctor = g_dtor = g_lookups = g_filterCalls = g_setMain = 0; g_lastName = 0; g_lastFilter = 0; g_lastFlags = -1; }
int main()
{
    CThingCreatureBase creature; NCreatureMode::CModeExplosiveForcePushed mode; mode.ParentThing = &creature;
    CAnimateThing anim; memset(&anim, 0, sizeof(anim));
    // type 1 still running -> skip
    reset(); anim.Type = 1; anim.m_stateBits = 0; mode.AnimUpdate(anim); CHECK(g_setMain == 0 && g_lookups == 0);
    // type 1 finished -> start idle
    reset(); anim.Type = 1; anim.m_stateBits = 2; mode.AnimUpdate(anim);
    CHECK(g_setMain == 1 && g_lookups == 1 && g_filterCalls == 1 && g_ctor == 1 && g_dtor == 1);
    CHECK(strcmp(g_lastName, "ST_IDLE_SUBTLE") == 0 && g_lastFilter == &g_filter && g_lastFlags == 0);
    CHECK(g_setType == 0x1d && g_setAnim == 77 && g_setLoop == 1 && g_setPri == 100 && g_blend == 0 && g_delay == 0);
    // already force-pushed -> skip
    reset(); anim.Type = 0x1d; anim.m_stateBits = 0; mode.AnimUpdate(anim); CHECK(g_setMain == 0 && g_lookups == 0);
    // any other type -> start idle
    reset(); anim.Type = 7; mode.AnimUpdate(anim); CHECK(g_setMain == 1 && g_lookups == 1);
    if (fails == 0) printf("008d6780_TEST PASS\n"); else printf("008d6780_TEST FAIL\n");
    return fails;
}