// CCollectFreeMatchesB::Visit(CVisitedItem* item) -- visitor callback: when the item matches the visitor's key and its owner object reports no claim (two chained virtual slot-5 queries return 0), append the item to the visitor's target list
// Leaf: Visit
// Retail: Fable.exe (TLC, VC7.1) @ 00C10190, 55 bytes (`ret 4`). Descriptive names (no PDB covers this library): the
//   manifest's "Inventory_IterateAndValidate" is propagated. Slot 0 of vtable 0x012B261C (slot 1 = deleting destructor 0x00C10840; base vtable
//   0x012B260C has _purecall in slot 0); constructor 0x00C106E0 stores (list, key) at +4/+8. The caller
//   0x00C104F0 builds it on the stack (0x00C105FD) over a local typed list (vtable 0x012B1C40), passes it to the container's
//   enumerate slot 10 and then drains the list. 0x00C10150 and 0x00C10190 are byte-identical bodies of two
//   instantiations (target lists 0x012B1B78 / 0x012B1C40).
// Callees: CVisitedItem::MatchesKey @ 0x00C1EC70, the item's slot 5 (+0x14) and the result's slot 5, the target
//   list's Add (slot 9, +0x24).
struct CClaimInfo {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual long GetClaim();                    // slot 5 (+0x14)
};

struct CVisitedItem {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual CClaimInfo* GetClaimInfo();         // slot 5 (+0x14)
    long MatchesKey(long key);                  // 0x00C1EC70
};

struct CItemList {                              // typed list (vtable 0x012B1C40)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8();
    virtual void Add(CVisitedItem* item);       // slot 9 (+0x24)
};

struct CItemVisitor {                           // base: vtable 0x012B260C
    virtual void Visit(CVisitedItem* item) = 0; // slot 0
    virtual ~CItemVisitor();                    // slot 1
};

struct CCollectFreeMatchesB : public CItemVisitor {
    CItemList* m_pList;                         // +4
    long m_key;                                 // +8
    virtual void Visit(CVisitedItem* item);
};

void CCollectFreeMatchesB::Visit(CVisitedItem* item)
{
    if (item->MatchesKey(m_key)) {
        if (item->GetClaimInfo()->GetClaim() == 0)
            m_pList->Add(item);
    }
}