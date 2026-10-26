// CTCInventory::IsQuitMenuReceivingInput() const -- true when any menu screen has a "load confirm" entry that is in state 2
// Leaf: IsQuitMenuReceivingInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005836D2, 154 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame.
// Callees: std::list<CComponent*>::list(const allocator_type&) @ 0x0042ABAF (default allocator temporary at
//          [ebp-1]; manifest "std::list::list"), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (built in the
//          argument area), the screen's vtable slot 5 (+0x14, collect children by name into a list),
//          std::list<CComponent*>::~list() @ 0x0042ABCA (manifest "CActiveFile::OnReadFinished" is propagated), and the
//          entry's state query, a virtual of its secondary base (this + 4, slot 2, argument 2).
// Shape: the screen index lives in the frame ([ebp-0Ch], zeroed at entry) because ebx/esi/edi hold the screen's
// vtable, the node and the screen; each pass builds the list local, fills it, walks it from the head sentinel and
// destroys it on both the hit path (`mov al,1`) and the fall-through.
// Layout: retail CTCInventory screen vector {first, last} at +0x0C; the list is the retail 4-byte head-only form.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();
};

struct CStateQuery {                                 // secondary base of a menu entry
    virtual void v00();
    virtual void v01();
    virtual bool IsInState(long state);              // slot 2 (+0x08)
};

struct CComponentBase {
    virtual void v00();
};

struct CMenuEntry : CComponentBase, CStateQuery {
};

template <class T> struct CListAllocator {
    CListAllocator() {}
};

struct CEntryList {                                  // std::list<CMenuEntry*> (retail head-only)
    struct SNode { SNode* _M_next; SNode* _M_prev; CMenuEntry* _M_data; };
    SNode* _M_node;
    explicit CEntryList(const CListAllocator<CMenuEntry*>& a = CListAllocator<CMenuEntry*>());   // 0x0042ABAF
    ~CEntryList();                                   // 0x0042ABCA
};

struct CMenuScreen {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindEntries(CCharString name, CEntryList& out);   // slot 5 (+0x14)
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x4];
    CRetailVector<CMenuScreen*> Screens;             // +0x0C

    bool IsQuitMenuReceivingInput() const;
};

bool CTCInventory::IsQuitMenuReceivingInput() const
{
    for (unsigned long i = 0; i < Screens.size(); ++i) {
        CEntryList entries;
        Screens[i]->FindEntries(CCharString("PC_UI_MENU_ENTRY_LOAD_CONFIRM", -1), entries);
        for (CEntryList::SNode* node = entries._M_node->_M_next; node != entries._M_node; node = node->_M_next) {
            CMenuEntry* entry = node->_M_data;
            if (entry && entry->IsInState(2))
                return true;
        }
    }
    return false;
}