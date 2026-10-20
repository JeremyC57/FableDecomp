// CThing::RemoveTC(long type) -- deferred removal of a thing component: find the TC of `type`; unless already flagged, flag it, clear its presence bit, queue {this, tc} on the global pending-removal list and notify the TC
// Leaf: RemoveTC
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9840, 224 bytes incl. padding (ret 4). Manifest label "CBaseObject::OnPropertyChanged"
//          is propagated; the body is CThing::RemoveTC(long) (GOTCHAS wave 25 chunks 140/163).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the TC's virtual slot 21 (+0x54, interface type),
//          std::bitset<274>::reset(pos) @ 0x004CC0D0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
//          GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//          list<STCRemoval>::insert(iterator, const STCRemoval&) @ 0x004CCC80 on the global list @ 0x013B8A24 (push_back),
//          the TC's virtual slot 2 (+8).
// Layout (CThing): interface presence bitset +0x20, interface map {first +0x44, last +0x48} of {type, CTCBase*};
//          CTCBase: pending-removal flag +8.
// Shape: the inline find (lower bound, then end unless the key matches; `type` re-read from its slot); push_back = insert(end(), v)
//          with the end iterator built in its by-value slot (user copy ctor) and the returned iterator in the dead `type` slot;
//          the node value is {CIntelligentPointer<CThing>, CTCBase*} built from a CIntelligentPointer(this) temporary.
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

struct CTCBase {
    virtual void v00(); virtual void v01();
    virtual void OnRemove();                    // slot 2 (+0x08)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual long GetInterfaceType();            // slot 21 (+0x54)
    void* PThing;                               // +0x04
    bool  PendingRemoval;                       // +0x08
};

struct STCRemoval {
    CIntelligentPointer PThing;                 // +0x00
    CTCBase*            PTC;                    // +0x08
    STCRemoval(const CIntelligentPointer& thing, CTCBase* tc) : PThing(thing), PTC(tc) { }
};

struct STCRemovalNode { STCRemovalNode* next; STCRemovalNode* prev; STCRemoval value; };
struct STCRemovalIterator {
    STCRemovalNode* node;
    STCRemovalIterator(STCRemovalNode* n) : node(n) { }
    STCRemovalIterator(const STCRemovalIterator& o) : node(o.node) { }
};
struct CTCRemovalList {
    STCRemovalNode* head;
    STCRemovalIterator end() { return STCRemovalIterator(head); }
    struct SInsertResult { STCRemovalNode* node; };
    SInsertResult insert(STCRemovalIterator where, const STCRemoval& value);   // 0x004CCC80
    void push_back(const STCRemoval& value) { insert(end(), value); }
};
extern CTCRemovalList g_TCRemovalList;          // 0x013B8A24

struct CTCBitset {
    unsigned long m_words[9];
    CTCBitset& reset(unsigned int pos);         // 0x004CC0D0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                              // +0x00
    SEntry* last;                               // +0x04
    SEntry* LowerBound(const long* type);       // 0x0040F020
    SEntry* find(const long& type)
    {
        SEntry* it = LowerBound(&type);
        if (it == last || type < it->type)
            it = last;
        return it;
    }
    SEntry* end() const { return last; }
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    void*         __vftable;                    // +0x00
    unsigned char _pad04[0x1c];
    CTCBitset     m_presentBits;                // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    void RemoveTC(long type);
};

void CThing::RemoveTC(long type)
{
    CInterfaceMap::SEntry* it = m_interfaces.find(type);
    if (it == m_interfaces.end())
        return;
    CTCBase* tc = it->component;
    if (tc->PendingRemoval)
        return;
    tc->PendingRemoval = true;
    m_presentBits.reset(tc->GetInterfaceType());
    g_TCRemovalList.push_back(STCRemoval(CIntelligentPointer(this), tc));
    tc->OnRemove();
}