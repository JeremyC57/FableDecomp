// Behaviour test for CTCTavernGame::ChangeBet (0x005eb810): first press steps once, held repeats accelerate, clamps hold.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad08[0x110 - 0x08];
    long MinBet;                                        // +0x110
    long MaxBet;                                        // +0x114
    long BetIncrement;                                  // +0x118
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out);   // 0x005ED4A0
};
void* FindDef_CDefClassBase_GetSubDef_005ed4a0_0();
bool CDefClassBase::GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_005ed4a0_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
long __fastcall GFGetCurrentFrame();                    // 0x0049D870
extern long g_BetRepeatDelayFrames;                     // 0x0137D38C (6)
extern long g_BetRepeatSlowPeriod;                      // 0x0137D390 (2)
extern long g_BetRepeatFastPeriod;                      // 0x0137D394 (1)

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x34 - 0x08];
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xdc - 0x44];
    long FrameBetWasLastChanged;                        // +0xdc
    long FrameBetChangeStarted;                         // +0xe0
    long Mult;                                          // +0xe4
    bool BetIncreasing;                                 // +0xe8
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void ChangeBet(bool increase);
};

void CTCTavernGame::ChangeBet(bool increase)
{
    CDefOut<CTavernGameDef> defOut;
    ParentThing->PDef->GetSubDef_005ed4a0(defOut);
    bool step = false;
    if (FrameBetWasLastChanged == GFGetCurrentFrame() - 1) {
        long held = GFGetCurrentFrame() - FrameBetChangeStarted - g_BetRepeatDelayFrames;
        if (held == 0 || (held >= 0 && held % g_BetRepeatSlowPeriod == 0) ||
            (held >= g_BetRepeatDelayFrames && held % g_BetRepeatFastPeriod == 0))
            step = true;
        if (held != 0 && held % 30 == 0)
            Mult *= 2;
    } else {
        FrameBetChangeStarted = GFGetCurrentFrame();
        step = true;
        Mult = 1;
    }
    FrameBetWasLastChanged = GFGetCurrentFrame();
    CDefHolder<CTavernGameDef> def(defOut.Object);
    bool changed = false;
    if (increase) {
        if (!BetIncreasing || BaseBet >= def->MaxBet || !step || BaseBet >= CurrentMoney)
            Mult = 1;
        changed = true;
        BaseBet += def->BetIncrement * Mult;
        BetIncreasing = true;
    } else {
        if (BetIncreasing || BaseBet == def->MinBet || !step)
            Mult = 1;
        BaseBet -= def->BetIncrement * Mult;
        if (BaseBet < def->MinBet)
            BaseBet = def->MinBet;
        else
            changed = true;
        BetIncreasing = false;
    }
    if (BaseBet > CurrentMoney) {
        BaseBet = CurrentMoney ? CurrentMoney : def->MinBet;
        changed = false;
    }
    if (BaseBet > def->MaxBet)
        BaseBet = def->MaxBet;
    else if (changed) {
        if (BetIncreasing)
            PlaySound("TAVERN_GAME_BET_INCREMENT");
        else
            PlaySound("TAVERN_GAME_BET_DECREMENT");
    }
}

static long g_frame = 0;
static int g_destroyed = 0;
static const char* g_lastSound = 0;
static int g_sounds = 0;
long g_BetRepeatDelayFrames = 6;
long g_BetRepeatSlowPeriod = 2;
long g_BetRepeatFastPeriod = 1;
long __fastcall GFGetCurrentFrame() { return g_frame; }
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
void CTCTavernGame::v00() { }
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() { }
unsigned long CTCTavernGame::PlaySound(const CCharString& name) { g_lastSound = (const char*)name.PStringData; ++g_sounds; return 0; }
static CTavernGameDef g_def;
void* FindDef_CDefClassBase_GetSubDef_005ed4a0_0() { ++g_def.RefCount; return &g_def; }
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++fails; } }
int main()
{
    g_def.RefCount = 1; g_def.MinBet = 10; g_def.MaxBet = 100; g_def.BetIncrement = 5;
    CDefClassBase cls; CThing thing; thing.PDef = &cls;
    CTCTavernGame g; std::memset(&g.BaseBet, 0, sizeof(long) * 4);
    g.ParentThing = &thing; g.BaseBet = 10; g.CurrentMoney = 1000;
    g.FrameBetWasLastChanged = -100; g.FrameBetChangeStarted = 0; g.Mult = 7; g.BetIncreasing = false;
    g_frame = 50;
    g.ChangeBet(true);                                   // fresh press: one step, multiplier reset
    check(g.BaseBet == 15 && g.Mult == 1 && g.BetIncreasing, "first increase");
    check(g_sounds == 1 && std::strcmp(g_lastSound, "TAVERN_GAME_BET_INCREMENT") == 0, "increment sound");
    check(g.FrameBetChangeStarted == 50 && g.FrameBetWasLastChanged == 50, "frames");
    check(g_def.RefCount == 1, "def ref balanced");
    g_frame = 51; g.ChangeBet(true);                     // held, not yet repeating: Mult reset, still adds once
    check(g.BaseBet == 20 && g.Mult == 1, "held before delay");
    g.BaseBet = 98; g_frame = 52; g.ChangeBet(true);     // over max -> clamped, no sound
    check(g.BaseBet == 100 && g_sounds == 2, "clamp to max");
    g.BaseBet = 12; g_frame = 200; g.ChangeBet(false);   // fresh decrease below min clamps to min, no sound
    check(g.BaseBet == 10 && !g.BetIncreasing && g_sounds == 2, "clamp to min");
    g.BaseBet = 50; g_frame = 300; g.ChangeBet(false);
    check(g.BaseBet == 45 && g_sounds == 3 && std::strcmp(g_lastSound, "TAVERN_GAME_BET_DECREMENT") == 0, "decrement");
    g.CurrentMoney = 30; g.BaseBet = 40; g_frame = 400; g.ChangeBet(true);   // more than money -> money, no sound
    check(g.BaseBet == 30 && g_sounds == 3, "clamp to money");
    g.CurrentMoney = 0; g.BaseBet = 40; g_frame = 500; g.ChangeBet(false);  // broke -> min bet
    check(g.BaseBet == 10, "broke uses min bet");
    check(g_def.RefCount == 1 && g_destroyed == 0, "refs balanced at end");
    if (fails == 0) std::printf("005eb810_TEST PASS\n");
    return fails;
}