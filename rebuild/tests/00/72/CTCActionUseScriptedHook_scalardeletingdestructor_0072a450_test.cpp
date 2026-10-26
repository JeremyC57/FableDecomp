#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
static bool Expect(const int* want, int n) { int i; if (g_n != n) return false; for (i = 0; i < n; ++i) if (g_log[i] != want[i]) return false; return true; }
class CThing;
class CTCBase { public:
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(true) {}
    virtual ~CTCBase() { Log(100); }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    void RemoveFromGlobalTCs() { IsInGlobalUpdateTCs = false; Log(1); }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* PItem; int m_tag;
    CBaseIntelligentPointer() : PItem(0), m_tag(0) {}
    ~CBaseIntelligentPointer() { Log(m_tag); }
};
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CCharString { public: int m_tag; CCharString() : m_tag(0) {} ~CCharString() { Log(m_tag); } };
class CTCActionUseScriptedHook : public CTCBase { public:
    bool Usable, ForceConfirmation, WaitingForConfirmation, PerformAction, TeleportToRegionEntrance;
    u64 EntranceConnectedToUID; CIntelligentPointer PEntranceConnectedTo; bool EntranceLocked;
    u64 CameraTrackUID; CIntelligentPointer PCameraTrack;
    CCharString SoundName; CCharString AnimationName; long ReplacementObject; bool ReversedOnMiniMap, HiddenOnMiniMap;
    virtual ~CTCActionUseScriptedHook() { RemoveFromGlobalTCs(); }
};
CTCActionUseScriptedHook* NewCTCActionUseScriptedHook() { return new CTCActionUseScriptedHook; }
int main()
{
    int bad = 0;
    CTCActionUseScriptedHook* h = NewCTCActionUseScriptedHook();
    h->PEntranceConnectedTo.m_tag = 32; h->PCameraTrack.m_tag = 56; h->SoundName.m_tag = 64; h->AnimationName.m_tag = 68;
    CTCBase* p = h;
    delete p;
    static const int want[] = { 1, 68, 64, 56, 32, 100 };
    if (!Expect(want, 6)) bad = 1;
    if (bad) { std::printf("0072a450_TEST FAIL\n"); return 1; }
    std::printf("0072a450_TEST PASS\n");
    return 0;
}