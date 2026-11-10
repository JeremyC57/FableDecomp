// CCollectAllItems::Visit(CListItem* item) -- the collect-all visitor's Visit: append every enumerated item to the local typed list it was built for (m_pList->Add(item))
// Leaf: ?Visit@CCollectAllItems@@UAEXPAUCListItem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10720, 8 bytes, thiscall `ret 4` through the tail call: `mov ecx,[ecx+4] ; mov eax,[ecx] ;
//   jmp [eax+24h]` (the item argument stays on the stack for the list's own Add).
// Identity: slot 0 of the collect-all visitor vtable 0x012B2624 (the only reference, cell 0x012B2624), the visitor the landed
//   CItemDispatcher::RunAllItems 0x00C10400 builds on its stack (`CCollectAllItems visitor(&list)`; that row's notes name this
//   body "Visit 0x00C10720 = m_pList->Add(item)"); base vtable 0x012B260C (slot 0 _purecall, slot 1 the deleting destructor).
//   The list's slot 9 (+0x24) is CItemList::Add 0x00C09C20 (`m_pImpl->Add(this, item)`, `ret 4`) in vtable 0x012B1B78.
//   libpk has no PDB or RTTI: class names are the landed descriptive ones (0x00C10400 / 0x00C104F0), method names descriptive.
struct CListItem;

struct CItemList {                              // vtable 0x012B1B78 (the typed list RunAllItems collects into)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CListItem* item);          // slot 9 (+0x24): 0x00C09C20
};

struct CItemVisitor {                           // vtable 0x012B260C
    virtual void Visit(CListItem* item) = 0;    // slot 0
    virtual ~CItemVisitor() { }                 // slot 1
};

struct CCollectAllItems : public CItemVisitor { // vtable 0x012B2624
    CItemList* m_pList;                         // +4
    CCollectAllItems(CItemList* list) : m_pList(list) { }
    virtual void Visit(CListItem* item);        // slot 0: 0x00C10720
};

void CCollectAllItems::Visit(CListItem* item)
{
    m_pList->Add(item);
}