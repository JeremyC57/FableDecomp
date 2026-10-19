// Behaviour test for CInputProcessSetRangedWeaponMode::ProcessInput (0x0068F180): the held/action tests, the gated
// component flag and the live-hero test decide whether game event 0x31 is queued.
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
class CInputEvent { public: int m_held, m_action; };
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    int m_count; long m_last;
    void AddGameEvent(const CGameEvent& event) { ++m_count; m_last = event.Type; }
    __forceinline void AddGameEvent(long type)
    {
        CGameEvent gameEvent(type);
        AddGameEvent(gameEvent);
    }
};
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const
    {
        STCEntry_Retail* it = m_pFirst;
        while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
        return it;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTC81 {                                     // component kind 0x81
    unsigned char _pad00[0x15];
    bool          m_15;                            // +0x15
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline bool HasTC(long kind) const
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** out, long kind) const   // the out-parameter lookup
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
class CPlayer {
public:
    bool m_fp; CThing* m_hero;
    bool IsFirstPersonTargeting() { return m_fp; }
    CThing* GetHeroThing() { return m_hero; }
};
bool g_rangedWeaponModeGate;
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
    bool IsEventButtonHeld(const CInputEvent& event) { return event.m_held != 0; }
    bool IsEventGameAction(long action, const CInputEvent& event) { return event.m_action == action; }
};
class CInputProcessSetRangedWeaponMode : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
};

void CInputProcessSetRangedWeaponMode::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    if (IsEventButtonHeld(event) && IsEventGameAction(0x2d, event)) {
        if (g_rangedWeaponModeGate && !MyPlayer->IsFirstPersonTargeting()) {
            CTC81* tc;
            if (MyPlayer->GetHeroThing()->GetInterface(&tc, 0x81) && !tc->m_15)
                return;
        }
        CThing* hero = MyPlayer->GetHeroThing();
        if (hero && !hero->Dead)
            input.AddGameEvent(0x31);
    }
}

static int Run(CInputProcessSetRangedWeaponMode& p, CInputEvent& e, CProcessedInput& in)
{
    in.m_count = 0; in.m_last = 0; p.ProcessInput(e, in); return in.m_count;
}
int main()
{
    static CThing hero; memset(&hero, 0, sizeof(hero));
    static CTC81 tc; memset(&tc, 0, sizeof(tc));
    STCEntry_Retail entries[1] = { { 0x81, (CTCBase_Retail*)&tc } };
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 1; hero.m_tcs.m_pEnd = entries + 1;
    static CPlayer player; player.m_fp = false; player.m_hero = &hero;
    CInputProcessSetRangedWeaponMode p; p.MyPlayer = &player;
    CInputEvent e; e.m_held = 1; e.m_action = 0x2d; CProcessedInput in;
    int bad = 0;
    g_rangedWeaponModeGate = false;
    if (Run(p, e, in) != 1 || in.m_last != 0x31) bad = 1;
    e.m_held = 0; if (Run(p, e, in) != 0) bad = 2;
    e.m_held = 1; e.m_action = 3; if (Run(p, e, in) != 0) bad = 3;
    e.m_action = 0x2d; g_rangedWeaponModeGate = true; hero.m_tcMask[4] = 2;
    if (Run(p, e, in) != 0) bad = 4;              // gated: component flag clear
    tc.m_15 = true; if (Run(p, e, in) != 1) bad = 5;
    tc.m_15 = false; player.m_fp = true; if (Run(p, e, in) != 1) bad = 6;
    player.m_fp = false; hero.m_tcMask[4] = 0; if (Run(p, e, in) != 1) bad = 7;
    hero.Dead = 1; if (Run(p, e, in) != 0) bad = 8;
    hero.Dead = 0; g_rangedWeaponModeGate = false; player.m_hero = 0; if (Run(p, e, in) != 0) bad = 9;
    if (bad) { std::printf("0068f180_TEST FAIL\n"); return 1; }
    std::printf("0068f180_TEST PASS\n");
    return 0;
}