// CItemDispatcher::RunAllItems() -- collect every item of the container at +0x10 into a local typed list (stack visitor through the container's enumerate slot), then drain the list: unlink the first item and run a stack {dispatcher, item} action on it until the list is empty
// Flags: /EHsc
// Leaf: RunAllItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10400, 228 bytes (`ret`). Built under /EHsc. Descriptive names (no PDB covers
//   this library): the manifest's "Combat_InitializeStrafeDef" is propagated. No code xrefs. Sibling of 0x00C104F0
//   (the handler the landed CEventQueue_00c08940::Process calls at +0x3c), which drains the same local list type with
//   the key-matching visitors 0x012B2614 / 0x012B261C.
// Callees: the typed list's impl constructor @ 0x00C185B0 (argument 1) and destructor @ 0x00C17B10, the container's
//   Enumerate (slot 10, +0x28), the impl's Remove(owner, item) (slot 6, +0x18) and GetFirst (slot 11, +0x2c) through
//   the list's inlined forwarders (out of line: 0x00C09C00 / 0x00C09C40), CItemAction::CItemAction @ 0x00C1EE00 and
//   CItemAction::Run @ 0x00C1F480.
// Vtables: the local list 0x012B1B78 (base 0x012B1AB0, dead in both directions), the collect-all visitor 0x012B2624
//   (Visit 0x00C10720 = m_pList->Add(item); out-of-line ctor 0x00C10700) over the visitor base 0x012B260C.
// Shape: the 8-byte action whose address is taken inside the loop gives the `and esp,-8` aligned frame.
struct CListItem;

struct CListImpl {                              // the list's implementation object (+0x8 of the typed list)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Add(void* owner, CListItem* item);        // slot 5 (+0x14)
    virtual bool Remove(void* owner, CListItem* item);     // slot 6 (+0x18)
    virtual void Slot7(); virtual void Slot8(); virtual void Slot9(); virtual void Slot10();
    virtual CListItem* GetFirst();                         // slot 11 (+0x2c)
    void* m_data;
    CListImpl(long mode);                       // 0x00C185B0
    ~CListImpl();                               // 0x00C17B10
};

struct CTypedListBase {                         // vtable 0x012B1AB0
    virtual void Slot0();
    CListImpl* m_pImpl;                         // +0x4
    CTypedListBase(CListImpl* impl) : m_pImpl(impl) { }
    ~CTypedListBase() { }
};

struct CItemList : public CTypedListBase {      // vtable 0x012B1B78
    CListImpl m_impl;                           // +0x8
    virtual void Slot0();
    CItemList(long mode) : CTypedListBase(&m_impl), m_impl(mode) { }
    ~CItemList() { }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CItemVisitor {                           // vtable 0x012B260C
    virtual void Visit(CListItem* item) = 0;    // slot 0
    virtual ~CItemVisitor() { }                 // slot 1
};

struct CCollectAllItems : public CItemVisitor { // vtable 0x012B2624
    CItemList* m_pList;                         // +4
    CCollectAllItems(CItemList* list) : m_pList(list) { }
    virtual void Visit(CListItem* item);        // 0x00C10720
};

struct CItemContainer {                         // typed container at +0x10
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Enumerate(CItemVisitor* visitor);         // slot 10 (+0x28)
};

struct CItemDispatcher;

struct CItemAction {                            // 8-byte {dispatcher, item} stack object
    CItemDispatcher* m_pDispatcher;
    CListItem* m_pItem;
    CItemAction(CItemDispatcher* dispatcher, CListItem* item);   // 0x00C1EE00
    void Run();                                 // 0x00C1F480
};

struct CItemDispatcher {
    unsigned char _pad00[0x10];
    CItemContainer m_items;                     // +0x10
    void RunAllItems();
};

void CItemDispatcher::RunAllItems()
{
    CItemList list(1);
    CCollectAllItems visitor(&list);
    m_items.Enumerate(&visitor);
    CListItem* item = list.GetFirst();
    while (item) {
        list.Remove(item);
        CItemAction action(this, item);
        action.Run();
        item = list.GetFirst();
    }
}