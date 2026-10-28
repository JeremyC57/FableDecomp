// Behaviour test for 0x00747330 CTCStealth::InitialiseSneakSpeed (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
class CCharString { public:
    CCharStringData* m_pData; CCharStringData store;
    CCharString() : m_pData(0) {}
    CCharString(const char* t, long = -1) { store.m_pData = t; store.m_length = (unsigned long)std::strlen(t); m_pData = &store; }
    const CCharString& operator=(const CCharString& o) { if (o.m_pData) { store = *o.m_pData; m_pData = &store; } else m_pData = 0; return *this; }
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
static unsigned long Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{ const unsigned char* p = (const unsigned char*)data; for (unsigned long i = 0; i < length; ++i) seed = seed * 33 + p[i]; return seed; }
class CAnimFilterBase { public: virtual ~CAnimFilterBase() {} };
class CAnimFilterOfGroup : public CAnimFilterBase { public:
    CCharString Group; unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& group) : Group() { Group = group; GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength()); }
};
class CAnimationEntry { public: long AnimID; };
static CAnimationEntry g_entry = { 17 };
static float g_scale = 2.0f, g_length = 1.5f, g_duration = 0.5f;
static char g_askName[32], g_askGroup[32]; static unsigned long g_askCRC = 0;
struct CTCGraphicAppearance { float GetScale() { return g_scale; } };
class CThingCreatureBase {
public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase* filter, long)
    {
        const CAnimFilterOfGroup* f = (const CAnimFilterOfGroup*)filter;
        std::strcpy(g_askName, name.GetPString()); std::strcpy(g_askGroup, f->Group.GetPString()); g_askCRC = f->GroupCRC;
        return &g_entry;
    }
    float GetAnimMovementLength(long anim) { return anim == 17 ? g_length : -1.0f; }
    float GetAnimDuration(long anim) { return anim == 17 ? g_duration : -1.0f; }
};
static long g_framesPerSecond = 30;
class CTCStealth { public:
    CThingCreatureBase* ParentThing;
    float MaxSneakingSpeed;
    void InitialiseSneakSpeed();
};
void CTCStealth::InitialiseSneakSpeed()
{
    CThingCreatureBase* creature = ParentThing;
    long anim;
    {
        CCharString group("CROUCH", -1);
        CCharString name("ST_WALK", -1);
        CAnimFilterOfGroup filter(group);
        anim = creature->GetAnimFromName(name, &filter, 0)->AnimID;
    }
    CTCGraphicAppearance* appearance = creature->PTCGraphicAppearance;
    float length = creature->GetAnimMovementLength(anim) * appearance->GetScale();
    float duration = creature->GetAnimDuration(anim);
    if (length > 0.0f && duration > 0.0f)
        MaxSneakingSpeed = length / (duration * g_framesPerSecond);
    else
        MaxSneakingSpeed = 0.1f;
}
static int fail() { std::printf("00747330_TEST FAIL\n"); return 1; }
int main()
{
    CTCGraphicAppearance app; CThingCreatureBase creature; creature.PTCGraphicAppearance = &app;
    CTCStealth s; s.ParentThing = &creature; s.MaxSneakingSpeed = -5.0f;
    s.InitialiseSneakSpeed();
    float expect = 3.0f / (0.5f * 30);                       // (1.5 * 2) / (0.5 * 30) = 0.2
    if (s.MaxSneakingSpeed != expect || std::strcmp(g_askName, "ST_WALK") != 0 || std::strcmp(g_askGroup, "CROUCH") != 0
        || g_askCRC != Calc_004014a0(0, "CROUCH", 6)) return fail();
    g_duration = 0.0f; s.InitialiseSneakSpeed();
    if (s.MaxSneakingSpeed != 0.1f) return fail();
    g_duration = 0.5f; g_scale = -1.0f; s.InitialiseSneakSpeed();
    if (s.MaxSneakingSpeed != 0.1f) return fail();
    std::printf("00747330_TEST PASS\n");
    return 0;
}