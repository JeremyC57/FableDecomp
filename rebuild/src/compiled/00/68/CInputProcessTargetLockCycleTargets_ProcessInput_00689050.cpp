// CInputProcessTargetLockCycleTargets::ProcessInput(const CInputEvent& event, CProcessedInput& input) -- on the release of the target-lock button (game action 1): detect a click (the pressure crossing the threshold upwards while a pulse window is open), update the pulse timer (reopen at 10 above the threshold, else count it down when pulses-only), store the pressure, and on a click make a live hero's targeting component (kind 8) target the next thing
// Leaf: ?ProcessInput@CInputProcessTargetLockCycleTargets@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689050, 222 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventGameAction @ 0x00687CF0, IsEventButtonReleased @ 0x00687B30 (labelled
//   IsEventButtonHeld; see GOTCHAS chunk 144), GetButtonPressureForEvent @ 0x00687D10 (landed, returns float),
//   CPlayer::GetHeroThing @ 0x00487DC0, the component-map LowerBound @ 0x0040F020, CTCTargetingPlayer::
//   SetAsTargetNextThing @ 0x00802590.
// Layout: Ego_r.pdb CInputProcessTargetLockCycleTargets (LastButtonPressure +0x28, ButtonPressureThresholdForClick +0x2c,
//   ClickAcceptancePulseTimer +0x30, AllowPulsesOnly +0x34) over CInputProcessGameBase (MyPlayer +0x18); retail CThing
//   component bitset +0x20 (kind 8 = bit 8), map +0x44, Dead bit +0x91.
// Shape: the click flag lives in bl from the entry; the pressure stays on the FPU (`fcom` keeps it) until it is stored;
//   the targeting component comes from an inline accessor that leaves its result unset when the kind is absent (retail
//   then reads the dead key slot), so VC duplicates the call into both arms.
class CInputEvent;
class CProcessedInput;
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class CTCTargetingPlayer { public:
    void SetAsTargetNextThing();                   // 0x00802590
};
__forceinline CTCTargetingPlayer* GetTargetingIfPresent(CThing* thing)
{
    CTCTargetingPlayer* tc;                        // deliberately unset when the component is absent
    if (thing->HasTC(8))
        tc = (CTCTargetingPlayer*)thing->m_tcs.Find(8);
    return tc;
}
class CPlayer {
public:
    CThing* GetHeroThing();                        // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30 (kind 0x14)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event);        // 0x00687D10
};
class CInputProcessTargetLockCycleTargets : public CInputProcessGameBase {
public:
    float LastButtonPressure;                      // +0x28
    float ButtonPressureThresholdForClick;         // +0x2c
    long  ClickAcceptancePulseTimer;               // +0x30
    bool  AllowPulsesOnly;                         // +0x34
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
};

void CInputProcessTargetLockCycleTargets::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    bool click = false;
    if (IsEventGameAction(1, event) && IsEventButtonReleased(event)) {
        float pressure = GetButtonPressureForEvent(event);
        long timer = ClickAcceptancePulseTimer;
        if (timer >= 0 && LastButtonPressure < ButtonPressureThresholdForClick && pressure > ButtonPressureThresholdForClick)
            click = true;
        if (LastButtonPressure > ButtonPressureThresholdForClick)
            ClickAcceptancePulseTimer = 10;
        else if (AllowPulsesOnly)
            ClickAcceptancePulseTimer = timer - 1;
        LastButtonPressure = pressure;
        if (click) {
            CThing* hero = MyPlayer->GetHeroThing();
            if (hero && !hero->Dead)
                GetTargetingIfPresent(hero)->SetAsTargetNextThing();
        }
    }
}