// Grade: DIFFER(1694v1690) 129 diff lines (base 383): templated GetTC<T> returning the typed TC (no map pointer); left: TC store through edx not eax, map base lea vs add esi,44h, g_pPlayerGui load register rotation. Tried: named quick-access local (no change), map-pointer GetTC (back to base). Next: model the full CVectorMap::LowerBound inline as 0x00747DA0's GetTC (retry wave 2 sec2c)
// Was: DIFFER(1714v1690) -- same blocks and order; register allocation only: retail rotates eax/edx/ecx for the g_pPlayerGui loads across the nine slot blocks and stores the TC through eax, and cross-jumps the ExpressionShiftMode = true tail from the held branch
// CInputProcessControlCreature::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- the hero's direct
//   controls: pressed actions raise the expression event, toggle the live GUI or pick quick-access slots 0..8 (hero TC
//   0x11 +0x140), toggle the quest screen; held actions raise the spell-mode / expression-shift events and toggle the
//   player's SpellMode / ExpressionShiftMode; a released action raises event 0x56
// Leaf: ProcessInput
// Flags: /GS
// Layout: engine/CInputProcessGameBase.h layout (MyPlayer +0x18, PGamePlayerInterface +0x24) and CPlayer modes +0x218 /
//   +0x219 as the landed CInputProcessControlCreature::Update 0x006881D0; g_pPlayerGui 0x013B8790 (LiveGuiMode +0x2BE).
class CInputEvent;
class CProcessedInput;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
struct CTCBase { };
struct CTCQuickAccess : CTCBase {
    unsigned char _pad00[0x140];
    long          SelectedSlot;                    // +0x140 (descriptive)
};
struct CTCMapEntry { long Kind; CTCBase* TC; };
struct CTCMap {
    CTCMapEntry* m_pFirst;
    CTCMapEntry* m_pLast;
    CTCMapEntry* m_pEnd;
    CTCMapEntry* LowerBound(const long* kind);     // 0x0040F020
    __forceinline CTCBase* Find(long kind)
    {
        long key = kind;
        CTCMapEntry* it = LowerBound(&key);
        return ((it == m_pLast || kind < it->Kind) ? m_pLast : it)->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
    template <class T> __forceinline T* GetTCT(long kind)
    {
        long key = kind;
        CTCMapEntry* it = m_tcs.LowerBound(&key);
        return (T*)((it == m_tcs.m_pLast || kind < it->Kind) ? m_tcs.m_pLast : it)->TC;
    }
};
class CPlayer {
public:
    unsigned char _pad[0x218];
    bool          SpellMode;                       // +0x218
    bool          ExpressionShiftMode;             // +0x219
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CGamePlayerInterface {
public:
    bool IsSpellModeForced() const;                // 0x00445B90
    bool IsExpressionShiftModeForced() const;      // 0x00445BB0
};
struct CPlayerGui {
    unsigned char _pad[0x2be];
    bool          LiveGuiMode;                     // +0x2BE
    void ToggleQuestScreen();                      // 0x004378D0 (descriptive)
    void ToggleLiveGui();                          // 0x00438310
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent;
class CInputProcessGameBase {
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
};
class CInputProcessControlCreature : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    __forceinline void SelectSlot(CThing* hero, long slot)
    {
        if (hero->HasTC(0x11))
            hero->GetTCT<CTCQuickAccess>(0x11)->SelectedSlot = slot;
    }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessControlCreature::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetHeroCreature();
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(3, event)) {
            if (hero->HasTC(0xc6)) {
                CGameEvent ev(0x1a);
                ev.AddLong(0x17);
                ev.AddBool(true);
                processed.AddGameEvent(ev);
            }
        } else if (IsEventGameAction(0x48, event)) {
            if (!g_pPlayerGui->LiveGuiMode)
                SelectSlot(hero, -1);
            g_pPlayerGui->ToggleLiveGui();
            processed.SetHandled();
        } else if (IsEventGameAction(4, event)) {
            g_pPlayerGui->ToggleQuestScreen();
            processed.SetHandled();
        } else if (IsEventGameAction(0x5a, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 0); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x5b, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 1); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x5c, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 2); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x5d, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 3); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x64, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 4); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x61, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 5); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x62, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 6); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x63, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 7); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        } else if (IsEventGameAction(0x60, event)) {
            if (!g_pPlayerGui->LiveGuiMode) { SelectSlot(hero, 8); g_pPlayerGui->ToggleLiveGui(); processed.SetHandled(); }
        }
        bool handled = false;
        if (IsEventGameAction(0x56, event) && !PGamePlayerInterface->IsSpellModeForced()) {
            MyPlayer->SpellMode = true;
            handled = true;
        }
        if (IsEventGameAction(0x57, event) && !PGamePlayerInterface->IsExpressionShiftModeForced()) {
            MyPlayer->ExpressionShiftMode = true;
            processed.SetHandled();
            return;
        }
        if (handled)
            processed.SetHandled();
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x55, event)) {
            CGameEvent ev(0x52);
            processed.AddGameEvent(ev);
        } else if (IsEventGameAction(0x5f, event)) {
            CGameEvent ev(0x54);
            processed.AddGameEvent(ev);
        } else {
            bool handled = false;
            if (PGamePlayerInterface->IsSpellModeForced() && IsEventGameAction(0x56, event)) {
                handled = true;
                if (!MyPlayer->SpellMode)
                    MyPlayer->SpellMode = true;
                else if (PGamePlayerInterface->IsSpellModeForced())
                    MyPlayer->SpellMode = false;
            }
            if (PGamePlayerInterface->IsExpressionShiftModeForced() && IsEventGameAction(0x57, event)) {
                MyPlayer->ExpressionShiftMode = !MyPlayer->ExpressionShiftMode;
                if (!MyPlayer->ExpressionShiftMode) {
                    MyPlayer->ExpressionShiftMode = true;
                    processed.SetHandled();
                    return;
                }
                if (PGamePlayerInterface->IsExpressionShiftModeForced())
                    MyPlayer->ExpressionShiftMode = false;
                handled = true;
            }
            if (handled)
                processed.SetHandled();
        }
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x71, event)) {
            CGameEvent ev(0x56);
            processed.AddGameEvent(ev);
        }
    }
}
