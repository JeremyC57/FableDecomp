// CTCTavernGame::ChangeBet(bool increase) -- step the bet up or down by the def's increment times an accelerating multiplier (auto-repeat while the button is held), clamp it to the money and the def's limits and play the step sound
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB810, 462 bytes (+ int3 padding), `ret 4`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0; GFGetCurrentFrame 0x0049D870 (four calls, never cached);
//   CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0; CTCTavernGame::PlaySound 0x005EA950; the def's virtual slot 1 on release.
// Layout: CTCTavernGame BaseBet +0x34, CurrentMoney +0x40, FrameBetWasLastChanged +0xdc, FrameBetChangeStarted +0xe0,
//   Mult +0xe4, BetIncreasing +0xe8 (retail field stores in the body); CTavernGameDef MinBet/MaxBet/BetIncrement
//   +0x110/+0x114/+0x118; repeat tunables are data globals 0x0137D38C/0x0137D390/0x0137D394 (6, 2, 1).
// Shape: in the increase branch `changed = true` is written before the `+=` (VC7.1 then schedules `mov dl,1` ahead of the
//   `add ecx,eax`); the reverse source order puts the add first.
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
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_005ed4a0_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
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