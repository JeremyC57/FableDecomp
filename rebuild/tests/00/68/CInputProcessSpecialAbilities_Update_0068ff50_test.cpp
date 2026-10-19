// Behaviour test for CInputProcessSpecialAbilities::Update (0x0068FF50): which button flags are shown and the latch.
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
struct CPlayerGui {
    int m_calls; bool m_a, m_b;
    void SetSpecialAbilityButtonsAsDisplaying(bool a, bool b) { ++m_calls; m_a = a; m_b = b; }
};
CPlayerGui* g_pPlayerGui;
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
struct CTCMenu {                                   // component kind 0xf6
    bool m_active;
    bool IsActive() { return m_active; }
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
class CPlayer {
public:
    unsigned char _pad000[0x20c];
    bool          RightTriggerTargetLocked;        // +0x20c
    unsigned char _pad20d[0x218 - 0x20d];
    bool          SpellMode;                       // +0x218
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
    bool m_queued;
    bool IsGameActionEventInQueue(long action, long buttonEventType) { return m_queued && action == 0x49 && buttonEventType == 2; }
};
class CInputProcessSpecialAbilities : public CInputProcessGameBase {
public:
    virtual void Update(CProcessedInput& input);
};

void CInputProcessSpecialAbilities::Update(CProcessedInput& input)
{
    if (MyPlayer->SpellMode) {
        g_pPlayerGui->SetSpecialAbilityButtonsAsDisplaying(true, true);
        MyPlayer->RightTriggerTargetLocked = true;
        return;
    }
    if (IsGameActionEventInQueue(0x49, 2)) {
        g_pPlayerGui->SetSpecialAbilityButtonsAsDisplaying(true, false);
        MyPlayer->RightTriggerTargetLocked = true;
        return;
    }
    CThing* hero = MyPlayer->GetHeroThing();
    if (hero && !hero->Dead) {
        CTCMenu* menu = 0;
        if (hero->HasTC(0xf6))
            menu = (CTCMenu*)hero->m_tcs.Find(0xf6);
        if (!menu->IsActive())
            g_pPlayerGui->SetSpecialAbilityButtonsAsDisplaying(false, true);
    }
    MyPlayer->RightTriggerTargetLocked = false;
}

int main()
{
    static CPlayerGui gui; g_pPlayerGui = &gui;
    static CThing hero; memset(&hero, 0, sizeof(hero));
    static CTCMenu menu; menu.m_active = false;
    STCEntry_Retail entries[1] = { { 0xf6, (CTCBase_Retail*)&menu } };
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 1; hero.m_tcs.m_pEnd = entries + 1;
    hero.m_tcMask[7] = 1UL << 22;
    static CPlayer player; memset(&player, 0, sizeof(player)); player.m_hero = &hero;
    CInputProcessSpecialAbilities p; p.MyPlayer = &player; p.m_queued = false;
    CProcessedInput* in = 0;
    int bad = 0;
    player.SpellMode = true; gui.m_calls = 0;
    p.Update(*in);
    if (gui.m_calls != 1 || !gui.m_a || !gui.m_b || !player.RightTriggerTargetLocked) bad = 1;
    player.SpellMode = false; p.m_queued = true; gui.m_calls = 0; player.RightTriggerTargetLocked = false;
    p.Update(*in);
    if (gui.m_calls != 1 || !gui.m_a || gui.m_b || !player.RightTriggerTargetLocked) bad = 2;
    p.m_queued = false; gui.m_calls = 0;
    p.Update(*in);
    if (gui.m_calls != 1 || gui.m_a || !gui.m_b || player.RightTriggerTargetLocked) bad = 3;
    menu.m_active = true; gui.m_calls = 0; player.RightTriggerTargetLocked = true;
    p.Update(*in);
    if (gui.m_calls != 0 || player.RightTriggerTargetLocked) bad = 4;
    hero.Dead = 1; menu.m_active = false; p.Update(*in);
    if (gui.m_calls != 0) bad = 5;
    if (bad) { std::printf("0068ff50_TEST FAIL\n"); return 1; }
    std::printf("0068ff50_TEST PASS\n");
    return 0;
}