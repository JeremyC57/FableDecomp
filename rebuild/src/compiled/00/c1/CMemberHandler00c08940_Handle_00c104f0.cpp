// CMemberHandler_00c08940::Handle(CEvent_00c08940* event) -- for each of the two item kinds, collect the items of the container at +0x10 that match the event and are unclaimed into a local typed list (stack key-matching visitor through the container's enumerate slot), then drain the list: unlink the first item, hand it the handler (item slot 7) and notify it (item slot 8 with 1), until the list is empty
// Leaf: ?Handle@CMemberHandler_00c08940@@QAEXPAUCEvent_00c08940@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C104F0, 427 bytes, thiscall `ret 4`, EH-framed (8 states: two lists and two
//   visitors). Descriptive names (libpk has no PDB or RTTI): the spelling is the one the landed caller
//   CEventQueue_00c08940::Process 0x00C08940 declares and binds (`m_handler.Handle(event); // 0x00C104F0`, the member
//   at +0x3c); the manifest's "CTCHero_ProcessBlockParryStrafe" is propagated. Twin of the landed
//   CItemDispatcher::RunAllItems 0x00C10400 (same local list type and drain loop), here run twice.
// Callees: the typed list's impl constructor @ 0x00C185B0 (argument 1) and destructor @ 0x00C17B10, the container's
//   Enumerate (slot 10, +0x28), the impl's slot 10 (+0x28, result unused), GetFirst (slot 11, +0x2c) and
//   Remove(owner, item) (slot 6, +0x18) through the lists' inline forwarders; the item's slots 7 and 8.
// Vtables: the local lists 0x012B1B78 (CItemList) and 0x012B1C40 (CVtable_012b1c40), both over the dead base 0x012B1AB0;
//   the visitors 0x012B2614 (CCollectFreeMatchesA, Visit 0x00C10150) and 0x012B261C (CCollectFreeMatchesB, Visit
//   0x00C10190) over the visitor base 0x012B260C, each storing (list, event) at +4/+8.
struct CListItem;
struct CEvent_00c08940;
struct CMemberHandler_00c08940;

struct CListItem {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6();
    virtual void SetHandler(CMemberHandler_00c08940* handler);   // slot 7 (+0x1c)
    virtual void Notify(long value);                             // slot 8 (+0x20)
};

struct CListImpl {                              // the list's implementation object (+0x8 of the typed list)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Add(void* owner, CListItem* item);        // slot 5 (+0x14)
    virtual bool Remove(void* owner, CListItem* item);     // slot 6 (+0x18)
    virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual unsigned int GetCount();                       // slot 10 (+0x28)
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
    unsigned int GetCount() { return m_pImpl->GetCount(); }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CVtable_012b1c40 : public CTypedListBase {   // vtable 0x012B1C40 (the second item kind's list)
    CListImpl m_impl;                           // +0x8
    virtual void Slot0();
    CVtable_012b1c40(long mode) : CTypedListBase(&m_impl), m_impl(mode) { }
    ~CVtable_012b1c40() { }
    unsigned int GetCount() { return m_pImpl->GetCount(); }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CItemVisitor {                           // vtable 0x012B260C
    virtual void Visit(CListItem* item) = 0;    // slot 0
    virtual ~CItemVisitor() { }                 // slot 1
};

struct CCollectFreeMatchesA : public CItemVisitor {   // vtable 0x012B2614
    CItemList* m_pList;                         // +4
    CEvent_00c08940* m_key;                     // +8
    CCollectFreeMatchesA(CItemList* list, CEvent_00c08940* key) : m_pList(list), m_key(key) { }
    virtual void Visit(CListItem* item);        // 0x00C10150
};

struct CCollectFreeMatchesB : public CItemVisitor {   // vtable 0x012B261C
    CVtable_012b1c40* m_pList;                  // +4
    CEvent_00c08940* m_key;                     // +8
    CCollectFreeMatchesB(CVtable_012b1c40* list, CEvent_00c08940* key) : m_pList(list), m_key(key) { }
    virtual void Visit(CListItem* item);        // 0x00C10190
};

struct CItemContainer {                         // typed container at +0x10
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Enumerate(CItemVisitor* visitor);         // slot 10 (+0x28)
};

struct CMemberHandler_00c08940 {
    unsigned char _pad00[0x10];
    CItemContainer m_items;                     // +0x10
    void Handle(CEvent_00c08940* event);
};

void CMemberHandler_00c08940::Handle(CEvent_00c08940* event)
{
    {
        CItemList list(1);
        CCollectFreeMatchesA visitor(&list, event);
        m_items.Enumerate(&visitor);
        list.GetCount();
        CListItem* item = list.GetFirst();
        while (item) {
            list.Remove(item);
            item->SetHandler(this);
            item->Notify(1);
            item = list.GetFirst();
        }
    }
    {
        CVtable_012b1c40 list(1);
        CCollectFreeMatchesB visitor(&list, event);
        m_items.Enumerate(&visitor);
        list.GetCount();
        CListItem* item = list.GetFirst();
        while (item) {
            list.Remove(item);
            item->SetHandler(this);
            item->Notify(1);
            item = list.GetFirst();
        }
    }
}