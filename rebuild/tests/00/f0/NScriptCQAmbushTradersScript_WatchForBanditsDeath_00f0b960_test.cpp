// Behaviour test for 00f0b960: the model below is a copy of src/00f0b960.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
class CScriptThing { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {}
    virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {}
    virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {}
    virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {}
    virtual void v070() {} virtual void v071() {} virtual void v072() {} virtual void v073() {} virtual void v074() {}
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp_Data;                                    // +0x04 (CCountedPointer<CScriptThingImp>)
    void* PImp_Info;                                    // +0x08
};

class CGameScriptInterface { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};

namespace std {
template <class T> struct vector {                      // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
}

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char _flags048[0x0a];                      // +0x48 .. +0x52 (QuestFailed .. ConvoyTrigger2)
    bool AllBanditsDead;                                // +0x52
    unsigned char _pad053[0x55];                        // +0x53 .. +0xa8 (flags, counters, speech timers)
    std::vector<CScriptThing> AllBandits;               // +0xa8
    void WatchForBanditsDeath();
};

void CQ_AmbushTradersScript::WatchForBanditsDeath()
{
    while (!IsActiveThreadTerminating()) {
        long deadCount = 0;
        for (unsigned long i = 0; i < AllBandits.size(); ++i) {
            if (!AllBandits[i].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                ++deadCount;
            }
        }
        if (deadCount == AllBandits.size()) {
            if (IsActiveThreadTerminating())
                return;
            AllBanditsDead = true;
            IsActiveThreadTerminating();
            return;
        }
        GSI->NewScriptFrame();
    }
}
}

// ---- test doubles and driver ----
// each bandit dies on the frame stored in its PImp_Info word (as an offset from a null char pointer)
static int g_frames = 0, g_termAt = 1000, g_aliveCalls = 0;
static long DeathFrame(const CScriptThing* t) { return (long)((const char*)t->PImp_Info - (const char*)0); }
bool CScriptThing::IsAlive() const { ++g_aliveCalls; return g_frames < DeathFrame(this); }
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; }
bool NScript::CScriptBase::IsActiveThreadTerminating() { return g_frames >= g_termAt; }

static bool Run(const long* deaths, int n, int termAt, int expectFrames, bool expectDead)
{
    static CGameScriptInterface gsi;
    static CScriptThing bandits[8];
    for (int i = 0; i < n; ++i) { bandits[i].PImp_Data = 0; bandits[i].PImp_Info = (void*)((const char*)0 + deaths[i]); }
    NScript::CQ_AmbushTradersScript s;
    std::memset(s._flags048, 0, sizeof(s._flags048)); s.AllBanditsDead = false;
    s.GSI = &gsi; s.PMasterData = 0;
    s.AllBandits._M_start = bandits; s.AllBandits._M_finish = bandits + n; s.AllBandits._M_end_of_storage = bandits + n;
    g_frames = 0; g_termAt = termAt; g_aliveCalls = 0;
    s.WatchForBanditsDeath();
    return g_frames == expectFrames && s.AllBanditsDead == expectDead;
}

int main()
{
    bool ok = true;
    const long d1[3] = { 2, 5, 3 };
    ok = ok && Run(d1, 3, 1000, 5, true);          // the last bandit dies on frame 5
    ok = ok && Run(d1, 3, 4, 4, false);            // terminated on frame 4 with one bandit standing
    ok = ok && Run(d1, 0, 1000, 0, true);          // no bandits: all dead at once
    const long d2[2] = { 0, 0 };
    ok = ok && Run(d2, 2, 1000, 0, true);          // already dead
    ok = ok && Run(d2, 2, 0, 0, false);            // terminating at entry
    if (!ok) { std::printf("00f0b960_TEST FAIL\n"); return 1; }
    std::printf("00f0b960_TEST PASS\n");
    return 0;
}