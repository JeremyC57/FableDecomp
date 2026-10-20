// CTCInventoryQuests::vfunc_70() -- take the selected quest card: when the screen allows it and has cards, activate the selected category's card with the quest manager (as a feat card when it has a feat index, else as a quest card), drop it from the trading thing's hero centre and rebuild the card list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006223C0, length 242 bytes (+ int3 padding), bool in al. Vtable 0x012585B4
// (CTCInventoryQuests) slot 70 (catalog "CTCInventoryQuests::vfunc_70"; the PDB name of the slot is not established).
// Callees: own virtual slot 75 (+0x12C, a bool gate; name descriptive), CTCInventoryBase::GetSelectedCategoryIndex @ 0x005BC4B2,
// GetCategoryWithIndex(long) @ 0x005BCE3C, CCategory::GetAssociatedThing @ 0x005BD1AE, the component-map LowerBound const @
// 0x004365B0 / non-const @ 0x0040F020, CQuestManager::ActivateFeatCard(CThing*) @ 0x004B1590 / ActivateQuestCard(CThing*,
// bool) @ 0x004B4AA0 on g_pQuestManager (0x013B89FC), CTCInventoryBase::GetTradingThing() @ 0x005BC4C4,
// CTCHeroCentre::RemoveQuestCard(CThing*) @ 0x0077D400, CTCInventoryQuests::InitialiseQuestCardList() @ 0x00621A20.
// Shape: the component pointers are the landed GetTCIfPresent form (left unset when the thing lacks the component: retail
// reads the lookup-key word), TC 0x6C (quest card, UsingComponents[3] bit 12, const lookup) and TC 0x6A (hero centre,
// bit 10, non-const lookup); the quest manager is loaded before the feat-index test so both activation arms share it;
// the outer tests are nested ifs (callee-saved pushes at entry, one shared false epilogue) while the two activation arms
// return false on their own (a merged `bool activated` lets VC7.1 tail-merge the two calls into one test).
// Layout: retail CTCInventoryQuests (CTCInventoryBase 0x130, 12-byte vectors): QuestCardCount +0x150 (PDB +0x170);
// CTCQuestCard feat index +0x14. No engine header for the class.
struct CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                       // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTCIfPresent(CThing* thing, long kind)
{
    CTCBase_Retail* tc;                         // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
struct CTCQuestCard {
    unsigned char _pad00[0x14];
    long FeatIndex;                             // +0x14 (-1 for an ordinary quest card)
};
struct CTCHeroCentre {
    void RemoveQuestCard(CThing* card);         // 0x0077D400
};
struct CQuestManager {
    bool ActivateFeatCard(CThing* card);                    // 0x004B1590
    bool ActivateQuestCard(CThing* card, bool fromScript);  // 0x004B4AA0
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC
namespace NInventory {
struct CCategory {
    CThing* GetAssociatedThing();               // 0x005BD1AE
};
}

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCInventoryQuests {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual bool vfunc_70();                    // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool CanTakeSelectedItem();         // slot 75 (+0x12C)
    unsigned char _pad04[0x150 - 0x04];
    long QuestCardCount;                        // +0x150
    long GetSelectedCategoryIndex();                        // 0x005BC4B2
    NInventory::CCategory* GetCategoryWithIndex(long index);   // 0x005BCE3C
    CThing* GetTradingThing();                  // 0x005BC4C4
    void InitialiseQuestCardList();             // 0x00621A20
};

bool CTCInventoryQuests::vfunc_70()
{
    if (CanTakeSelectedItem() && QuestCardCount > 0) {
        CThing* card = GetCategoryWithIndex(GetSelectedCategoryIndex())->GetAssociatedThing();
        if (card) {
            CTCQuestCard* questCard = (CTCQuestCard*)GetTCIfPresentConst(card, 0x6c);
            if (questCard) {
                if (questCard->FeatIndex > -1) {
                    if (!g_pQuestManager->ActivateFeatCard(card))
                        return false;
                } else if (!g_pQuestManager->ActivateQuestCard(card, false)) {
                    return false;
                }
                CThing* trader = GetTradingThing();
                if (trader)
                    ((CTCHeroCentre*)GetTCIfPresent(trader, 0x6a))->RemoveQuestCard(card);
                InitialiseQuestCardList();
                return true;
            }
        }
    }
    return false;
}