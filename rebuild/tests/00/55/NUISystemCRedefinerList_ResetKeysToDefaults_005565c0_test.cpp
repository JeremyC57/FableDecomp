// Behaviour test for 005565c0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_volumeCalls = 0;
static unsigned long g_masks[8];
class CSoundManagerLH {
public:
    virtual ~CSoundManagerLH() { }
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime)   // stub for 0x00A3E9D0
    {
        CHECK(volume == 0.0f && fadeTime == 0.0f);
        g_masks[g_volumeCalls++ & 7] = groupMask;
    }
};
static CSoundManagerLH* g_pSoundSystem = 0;
static bool SoundResetOk(int base) { return g_masks[base] == 0x10 && g_masks[base + 1] == 0x80 && g_masks[base + 2] == 0x40; }

static int g_resets = 0, g_resetsWASD = 0;
class CUserProfileManager {
public:
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }   // stub for 0x0040D2A0
    void ResetAssignedInputs() { ++g_resets; }                                      // stub for 0x004085F0
    void ResetAssignedInputsWASD() { ++g_resetsWASD; }                              // stub for 0x00408820
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
class CComponentRoot {
public:
    CRGBColour Colour;
    virtual ~CComponentRoot() { }
    virtual void SetColour(const CRGBColour& colour) { Colour = colour; }
    virtual CRGBColour GetColour() const { return Colour; }
};
class CObserver {
public:
    bool Observing;
    int ObserveCalls;
    CObserver() : Observing(false), ObserveCalls(0) { }
    virtual ~CObserver() { }
    virtual bool IsObservingEvent(long eventType) { return eventType == 0x19 && Observing; }
    virtual void ObserveEvent(long eventType) { CHECK(eventType == 0x19); Observing = true; ++ObserveCalls; }
};
class CComponent : public CComponentRoot, public CObserver {
};
class CRedefinerList {
public:
    bool        IsActivated;
    bool        Selecting;
    float       FadeTime;
    CComponent* PAcceptButton;
    float       HiddenWith;
    CRedefinerList() : IsActivated(true), Selecting(false), FadeTime(0), PAcceptButton(0), HiddenWith(-1) { }
    virtual ~CRedefinerList() { }
    static CRedefinerList* s_pActiveList;
    void HideList(float time) { HiddenWith = time; }                                // stub for 0x005559D0
    void FadeList(float time, bool selecting)
    {
        HideList(time);
        FadeTime = time;
        Selecting = selecting;
    }
    static void ResetKeysToDefaults(bool wasd)
    {
        if (wasd)
            CUserProfileManager::Get().ResetAssignedInputsWASD();
        else
            CUserProfileManager::Get().ResetAssignedInputs();
        if (s_pActiveList != 0)
            s_pActiveList->FadeList(0.1f, true);
        if (s_pActiveList != 0) {
            CComponent* acceptButton = s_pActiveList->PAcceptButton;
            if (acceptButton != 0 && !acceptButton->IsObservingEvent(0x19)) {
                acceptButton->ObserveEvent(0x19);
                CRGBColour colour = acceptButton->GetColour();
                colour.A = 0xff;
                acceptButton->SetColour(colour);
            }
        }
        if (g_pSoundSystem != 0) {
            g_pSoundSystem->SetGroupVolume(0x10, 0.0f, 0.0f);
            g_pSoundSystem->SetGroupVolume(0x80, 0.0f, 0.0f);
            g_pSoundSystem->SetGroupVolume(0x40, 0.0f, 0.0f);
        }
    }
};
CRedefinerList* CRedefinerList::s_pActiveList = 0;
static void RunTest()
{
    CRedefinerList::ResetKeysToDefaults(false);               // no list, no sound: only the reset
    CHECK(g_resets == 1 && g_resetsWASD == 0 && g_volumeCalls == 0);
    CRedefinerList list;
    CComponent accept;
    accept.Colour.B = 1; accept.Colour.G = 2; accept.Colour.R = 3; accept.Colour.A = 0x40;   // greyed out
    list.PAcceptButton = &accept;
    CRedefinerList::s_pActiveList = &list;
    CSoundManagerLH sound;
    g_pSoundSystem = &sound;
    CRedefinerList::ResetKeysToDefaults(true);
    CHECK(g_resetsWASD == 1 && g_resets == 1);
    CHECK(list.HiddenWith == 0.1f && list.FadeTime == 0.1f && list.Selecting);
    CHECK(accept.ObserveCalls == 1 && accept.Colour.A == 0xff && accept.Colour.R == 3 && accept.Colour.B == 1);
    CHECK(g_volumeCalls == 3 && SoundResetOk(0));
    accept.Colour.A = 0x40;
    CRedefinerList::ResetKeysToDefaults(false);               // already observing: the colour is left alone
    CHECK(accept.ObserveCalls == 1 && accept.Colour.A == 0x40 && g_resets == 2 && g_volumeCalls == 6);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("005565c0_TEST FAIL\n");
        return 1;
    }
    std::printf("005565c0_TEST PASS\n");
    return 0;
}