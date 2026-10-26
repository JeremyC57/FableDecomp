// Behaviour test for 008d6f20: body copied from src/008d6f20.cpp (CModeRockTrollVulnerable::AnimUpdate), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_ctor = 0, g_dtor = 0, g_lookups = 0, g_flagsCalls = 0, g_blendCalls = 0, g_setMain = 0;
static const char* g_lastName = 0; static long g_lastFlags = -1; static const void* g_blendThing = 0;
static long g_setType = 0, g_setAnim = 0, g_setLoop = 0, g_setPri = 0, g_setFlags = 0, g_blend = -1, g_delay = -1;
class CThing;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1) { m_pData = (void*)text; ++g_ctor; }
    ~CCharString() { ++g_dtor; }
};
class CAnimationEntry { public:
    long BankIndex; unsigned char _rest[0xc];
    unsigned long GetAnimateThingFlags() const { ++g_flagsCalls; return 0x55; }
    long GetTimeToBlendIn(CThing* thing) const { ++g_blendCalls; g_blendThing = thing; return 9; }
};
class CAnimFrameInfo { public: long m_frame; long m_subFrame; CAnimFrameInfo() : m_frame(0), m_subFrame(0) { } };
class CAnimInit { public:
    long UpdateMode; unsigned char Type; bool Looping; long Anim; float Speed; float Alpha; float LastAlpha;
    CAnimFrameInfo Frame; unsigned long Flags; unsigned long TypeFlags; long Priority;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    { Set(type, typeFlags, anim, updateMode, looping, flags, priority); }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    { Type = type; TypeFlags = typeFlags; Anim = anim; UpdateMode = updateMode; Looping = looping; Flags = flags; Priority = priority; }
};
class CAnimateThing { public:
    long UpdateMode; short* m_animsFirst; short* m_animsLast; unsigned char _pad0c[0x44]; unsigned char Type;
    unsigned char Looping : 1; unsigned char Finished : 1;
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay)
    { ++g_setMain; g_setType = init.Type; g_setAnim = init.Anim; g_setLoop = init.Looping; g_setPri = init.Priority; g_setFlags = init.Flags; g_blend = blendIn; g_delay = delay; }
};
static CAnimationEntry g_entry = { 77 };
class CThingCreatureBase { public:
    CAnimationEntry* GetAnimFromName(const CCharString& name, long flags)
    { ++g_lookups; g_lastName = (const char*)name.m_pData; g_lastFlags = flags; return &g_entry; }
};
struct CModeBase {
    virtual void v00() {}
    CThingCreatureBase* ParentThing;
    unsigned char _body[0x14];
    CThingCreatureBase* GetParent() const { return ParentThing; }
};
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static void reset() { g_ctor = g_dtor = g_lookups = g_flagsCalls = g_blendCalls = g_setMain = 0; g_lastName = 0; g_lastFlags = -1; g_blendThing = 0; }
namespace NCreatureMode {

struct CModeRockTrollVulnerable : CModeBase {
    long                RecoveryCount;     // +0x1c
    unsigned char       _filter[0xc];      // +0x20 CAnimFilterOfGroup Filter
    CThingCreatureBase* PCreature;         // +0x2c
    bool                bRecovering;       // +0x30
    void AnimUpdate(CAnimateThing& anim);
};

void CModeRockTrollVulnerable::AnimUpdate(CAnimateThing& anim)
{
    if (anim.m_animsFirst == anim.m_animsLast || anim.Finished) {
        CAnimationEntry* entry;
        {
            CCharString name("VULNERABLE_LOOP");
            entry = PCreature->GetAnimFromName(name, 0L);
        }
        CThing* thing = (CThing*)ParentThing;
        anim.SetMainAnim(CAnimInit(0x15, 0, entry->BankIndex, 1, false, entry->GetAnimateThingFlags(), 100), entry->GetTimeToBlendIn(thing), 0);
    }
}

}
int main()
{
    CThingCreatureBase creature, other; NCreatureMode::CModeRockTrollVulnerable mode; memset(&mode, 0, sizeof(mode)); mode.ParentThing = &creature; mode.PCreature = &other; short anims[2];
    CAnimateThing anim; memset(&anim, 0, sizeof(anim));
    reset(); anim.m_animsFirst = anims; anim.m_animsLast = anims + 1; anim.Finished = 0; anim.Type = 0x15; mode.AnimUpdate(anim); CHECK(g_setMain == 0 && g_lookups == 0);
    reset(); anim.m_animsLast = anims; mode.AnimUpdate(anim);
    CHECK(g_setMain == 1 && g_lookups == 1 && g_flagsCalls == 1 && g_blendCalls == 1 && g_ctor == 1 && g_dtor == 1);
    CHECK(strcmp(g_lastName, "VULNERABLE_LOOP") == 0 && g_lastFlags == 0 && g_blendThing == (CThing*)&creature);
    CHECK(g_setType == 0x15 && g_setAnim == 77 && g_setLoop == 0 && g_setPri == 100 && g_setFlags == 0x55 && g_blend == 9 && g_delay == 0);
    reset(); anim.m_animsLast = anims + 1; anim.Finished = 1; mode.AnimUpdate(anim); CHECK(g_setMain == 1);
    reset(); anim.Finished = 0; anim.Type = 3; mode.AnimUpdate(anim); CHECK(g_setMain == 0);
    if (fails == 0) printf("008d6f20_TEST PASS\n"); else printf("008d6f20_TEST FAIL\n");
    return fails;
}