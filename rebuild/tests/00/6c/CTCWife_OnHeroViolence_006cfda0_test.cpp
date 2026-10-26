// Behaviour test for CTCWife::OnHeroViolence (0x006CFDA0): a wife married after the current world frame
// (courting still pending) is blocked and loses her love attitude; an established wife applies the two
// hate-spike opinion offsets (attitudes 4 and 3) with the def's peak/persist values and the run-out seconds
// converted to frames.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static long g_worldFrame = 100;
static long g_framesPerSecond = 30;
struct SCall { long attitude; float peak; long unused; long frames; float persist; long flag; };
static SCall g_calls[4]; static int g_nCalls = 0;
static int g_iconCalls = 0; static bool g_iconShow = true; static float g_iconScale = 0; static unsigned char g_iconAlpha = 0;
class CTCCreatureOpinionOfHero { public:
    void ApplyOpinionTransientOffset(long attitude, float peak, long unused, long runOutFrames, float persist, bool flag)
    { SCall c = { attitude, peak, unused, runOutFrames, persist, flag }; g_calls[g_nCalls++] = c; }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CWorld { public: static long GetWorldFrame() { return g_worldFrame; } };
class CHeroMarriageDef { public:
    unsigned char _pad00[0x90];
    float SecondsRunOutWifeOnViolenceHateSpike;
    float PeakEffectWifeOnViolenceHateSpike;
    float PersistEffectWifeOnViolenceHateSpike;
};
class CTCWife { public:
    virtual void v00() {}
    CThing*          ParentThing;
    unsigned char    _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;
    unsigned char    _pad_0x10[5];
    bool             CourtingBlocked;
    unsigned char    _pad_0x16[0x20 - 0x16];
    long             FrameGotMarriedToThePlayer;
    unsigned char    _pad_0x24[0x8c - 0x24];
    float            LoveAttitudeValue;
    void SetAttractionIcon(bool show, float scale, unsigned char alpha) { ++g_iconCalls; g_iconShow = show; g_iconScale = scale; g_iconAlpha = alpha; }
    void OnHeroViolence();
};
void CTCWife::OnHeroViolence()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame()) {
        CourtingBlocked = true;
        LoveAttitudeValue = 0.0f;
        SetAttractionIcon(false, 1.0f, 0xff);
        return;
    }
    CTCCreatureOpinionOfHero* opinion;
    ParentThing->GetInterface(&opinion, 0x4f);
    opinion->ApplyOpinionTransientOffset(4, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)g_framesPerSecond),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
    opinion->ApplyOpinionTransientOffset(3, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)g_framesPerSecond),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
}
static int fail(const char* why) { std::printf("006cfda0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CTCCreatureOpinionOfHero opinion;
    static CInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0x4f, (CTCBase*)&opinion }, { 0x60, 0 } };
    static CThing thing; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    thing.m_presentBits[0x4f >> 5] = 1UL << (0x4f & 31);
    static CHeroMarriageDef def; def.SecondsRunOutWifeOnViolenceHateSpike = 2.5f; def.PeakEffectWifeOnViolenceHateSpike = -0.75f; def.PersistEffectWifeOnViolenceHateSpike = -0.25f;
    static CTCWife wife; wife.ParentThing = &thing; wife.PHeroMarriageDef = &def; wife.LoveAttitudeValue = 0.9f; wife.CourtingBlocked = false;
    // 1. married in the future (frame 150 > world 100): blocked, attitude zeroed, icon hidden, no opinion calls
    wife.FrameGotMarriedToThePlayer = 150;
    wife.OnHeroViolence();
    if (!wife.CourtingBlocked || wife.LoveAttitudeValue != 0.0f) return fail("pending wife not blocked");
    if (g_iconCalls != 1 || g_iconShow || g_iconScale != 1.0f || g_iconAlpha != 0xff) return fail("icon call");
    if (g_nCalls != 0) return fail("opinion applied while blocked");
    // 2. established wife (frame 40 <= 100): two spikes, attitudes 4 then 3, 2.5s * 30fps = 75 frames
    wife.FrameGotMarriedToThePlayer = 40; wife.CourtingBlocked = false; wife.LoveAttitudeValue = 0.9f;
    wife.OnHeroViolence();
    if (wife.CourtingBlocked || wife.LoveAttitudeValue != 0.9f || g_iconCalls != 1) return fail("established wife touched the courting state");
    if (g_nCalls != 2) return fail("two opinion offsets expected");
    for (int i = 0; i < 2; ++i) {
        const SCall& c = g_calls[i];
        if (c.attitude != (i == 0 ? 4 : 3) || c.peak != -0.75f || c.unused != 0 || c.frames != 75 || c.persist != -0.25f || c.flag != 1) return fail("opinion offset arguments");
    }
    // 3. equal frames are not "after": married this very frame counts as established
    wife.FrameGotMarriedToThePlayer = 100; g_nCalls = 0;
    wife.OnHeroViolence();
    if (g_nCalls != 2 || wife.CourtingBlocked) return fail("same-frame marriage");
    std::printf("006cfda0_TEST PASS\n");
    return 0;
}