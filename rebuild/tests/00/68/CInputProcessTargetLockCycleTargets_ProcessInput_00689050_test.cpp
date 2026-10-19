// Behaviour test for CInputProcessTargetLockCycleTargets::ProcessInput (0x00689050): click detection, the pulse timer
// update, the stored pressure and the next-target call for a live hero with a targeting component.
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
class CInputEvent { public: long m_action; int m_released; float m_pressure; };
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
    STCEntry_Retail* LowerBound(const unsigned long& kind)
    {
        STCEntry_Retail* it = m_pFirst;
        while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
        return it;
    }
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
struct CTCTargetingPlayer {
    int m_next;
    void SetAsTargetNextThing() { ++m_next; }
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
    CThing* m_hero;
    CThing* GetHeroThing() { return m_hero; }
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00() {}
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonReleased(const CInputEvent& event) { return event.m_released != 0; }
    bool IsEventGameAction(long action, const CInputEvent& event) { return event.m_action == action; }
    float GetButtonPressureForEvent(const CInputEvent& event) { return event.m_pressure; }
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

int main()
{
    static CThing hero; memset(&hero, 0, sizeof(hero));
    static CTCTargetingPlayer targeting; targeting.m_next = 0;
    STCEntry_Retail entries[1] = { { 8, (CTCBase_Retail*)&targeting } };
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 1; hero.m_tcs.m_pEnd = entries + 1;
    hero.m_tcMask[0] = 1UL << 8;
    static CPlayer player; player.m_hero = &hero;
    CInputProcessTargetLockCycleTargets p; p.MyPlayer = &player;
    p.LastButtonPressure = 0.2f; p.ButtonPressureThresholdForClick = 0.5f; p.ClickAcceptancePulseTimer = 3; p.AllowPulsesOnly = true;
    CInputEvent e; e.m_action = 1; e.m_released = 1; e.m_pressure = 0.9f;
    CProcessedInput* in = 0;
    int bad = 0;
    p.ProcessInput(e, *in);                         // click: 0.2 < 0.5 < 0.9; timer counts down (last below threshold)
    if (targeting.m_next != 1 || p.ClickAcceptancePulseTimer != 2 || p.LastButtonPressure != 0.9f) bad = 1;
    e.m_pressure = 0.1f;
    p.ProcessInput(e, *in);                         // no click; last 0.9 above threshold -> timer 10
    if (targeting.m_next != 1 || p.ClickAcceptancePulseTimer != 10 || p.LastButtonPressure != 0.1f) bad = 2;
    p.ClickAcceptancePulseTimer = -1; e.m_pressure = 0.8f;
    p.ProcessInput(e, *in);                         // timer closed: no click, pulses-only count down
    if (targeting.m_next != 1 || p.ClickAcceptancePulseTimer != -2) bad = 3;
    p.AllowPulsesOnly = false; p.ClickAcceptancePulseTimer = 4; p.LastButtonPressure = 0.1f; hero.Dead = 1;
    p.ProcessInput(e, *in);                         // click but dead hero; timer untouched
    if (targeting.m_next != 1 || p.ClickAcceptancePulseTimer != 4) bad = 4;
    e.m_released = 0; p.LastButtonPressure = 0.3f; p.ProcessInput(e, *in);
    if (p.LastButtonPressure != 0.3f) bad = 5;
    if (bad) { std::printf("00689050_TEST FAIL\n"); return 1; }
    std::printf("00689050_TEST PASS\n");
    return 0;
}