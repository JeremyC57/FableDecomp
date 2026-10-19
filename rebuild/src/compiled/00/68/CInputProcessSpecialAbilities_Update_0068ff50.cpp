// CInputProcessSpecialAbilities::Update(CProcessedInput& input) -- keep the special-ability buttons on screen: in spell mode show them (both flags) and latch the player's trigger lock; when the spell-mode game action is queued show them (first flag) and latch; otherwise, for a live hero whose kind-0xf6 menu component is not active, show them (second flag); then clear the latch
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068FF50, 210 bytes, `ret 4`.
// Callees: CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool, bool) @ 0x00437580 on g_pPlayerGui (0x013B8790),
//   CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType) @ 0x00687D50 (landed name),
//   CPlayer::GetHeroThing @ 0x00487DC0, the const component-map LowerBound @ 0x004365B0, the kind-0xf6 menu component's
//   IsActive() @ 0x00625930 (+0x0c; label "GetCanItemBeQuickAssigned" is propagated).
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18), local model (the class carries virtuals); CPlayer (Ego_r.pdb,
//   the PlayerModes list 4 bytes in retail): RightTriggerTargetLocked +0x20c, SpellMode +0x218; retail CThing component
//   bitset +0x20 (kind 0xf6 = word +0x3c bit 22), map +0x44, Dead bit +0x91.
// Shape: two early-return arms each ending in the latch store; the menu pointer is preset to null and set only when
//   the component is present (`xor ecx,ecx` before the test), then called unconditionally.
struct CPlayerGui {
    void SetSpecialAbilityButtonsAsDisplaying(bool a, bool b);   // 0x00437580
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
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
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCMenu {                                   // component kind 0xf6
    bool IsActive();                               // 0x00625930
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
    CThing* GetHeroThing();                        // 0x00487DC0
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsGameActionEventInQueue(long action, long buttonEventType);   // 0x00687D50
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