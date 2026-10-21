// CThing::WorldUpdate() -- frame-update every live (not pending-removal) component on the update list while the thing is alive; then sweep the global pending-removal list: drop entries whose thing is gone or dead, and for this thing's entries unhook the component (driver pointer, update list, interface map), delete it and drop the entry; always false
// Leaf: WorldUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C8050, length 393 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?WorldUpdate@CThing@@UAE_NXZ`).
// Callees: CTCBase virtual slots 9 (+0x24, FrameUpdate), 21 (+0x54, GetInterfaceType, as the landed RemoveTC 0x004C9840),
//   24 (+0x60, IsDriver: the PDB's `?IsDriver@CTCBase@@UBE_NXZ`, and the hit clears PTCDriver) and 0 (scalar deleting
//   destructor, flag 1); CBaseIntelligentPointer::GetPItem 0x00A01B50 and ~CBaseIntelligentPointer 0x00A01C10;
//   list<CTCBase*>::remove(const value_type&) 0x004CC150 (update list); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   0x0040F020; free 0x00BFEA14 (STLport node of the pending-removal list).
// Data: CThing::TCsToBeRemoved 0x013B8A24 (static list<STCRemoval>; the landed RemoveTC's g_TCRemovalList).
// Layout: retail CThing (Ego_r.pdb minus 4 for the CVectorMap, minus 8 more for the 4-byte STLport list): Components +0x44
//   (first +0x44, last +0x48), UpdateComponentsList +0x54, PTCDriver +0x5C, Dead +0x91 bit 0 (as landed); CTCBase
//   pending-removal flag +0x08 (as landed); STCRemoval {CIntelligentPointer<CThing> +0x00, CTCBase* +0x08}.
// Shape: the update loop tests Dead at the head of every pass; the sweep re-reads the list head each pass and tests the
//   entry through an inline "thing alive" helper (`if (!alive) erase; else if (mine) {..; erase} else ++it` -- written
//   out, VC7.1 keeps the merged erase at the bottom instead of at the stale test); both erases are STLport list::erase
//   (unlink, element destructor, free); the component is unhooked from the update list through a same-TU noinline
//   list::remove stand-in (the component stays in ebp across it) and from the interface map through the inline find
//   (key copy in the shared frame word, the type in ebx), a forward scan over equal keys and STLport vector::erase
//   (`copy(pos + 1, finish, pos)`, --finish); the same-TU LowerBound stand-in shares the key word with the component.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>
};

struct CTCBase {
    virtual ~CTCBase();                                     // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                             // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual long GetInterfaceType() const;                  // slot 21 (+0x54)
    virtual void v22(); virtual void v23();
    virtual bool IsDriver() const;                          // slot 24 (+0x60)
    CThing* ParentThing;                                    // +0x04
    bool    PendingRemoval;                                 // +0x08
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {                   // STLport _List_iterator
    SListNode<T>* _M_node;
    CListIterator() { }
    CListIterator(SListNode<T>* n) : _M_node(n) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    CListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const CListIterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const CListIterator& other) const { return _M_node != other._M_node; }
};
template <class T> struct CRetailList {                     // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    CListIterator<T> begin() { return CListIterator<T>(_M_node->_M_next); }
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    CListIterator<T> erase(CListIterator<T> position)
    {
        SListNode<T>* next = position._M_node->_M_next;
        SListNode<T>* prev = position._M_node->_M_prev;
        SListNode<T>* n = position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~T();
        free(n);
        return CListIterator<T>(next);
    }
    void remove(const T& value);                            // 0x004CC150 (T = CTCBase*)
};
#pragma auto_inline(off)
// Same-TU stand-in for list<CTCBase*>::remove 0x004CC150 (STLport's body): VC7.1 then knows the value's address does
// not escape, so the component stays in a register across the call (retail's LTCG knowledge).
template <class T> __declspec(noinline) void CRetailList<T>::remove(const T& value)
{
    CListIterator<T> first = begin();
    CListIterator<T> last = end();
    while (first != last) {
        CListIterator<T> next = first;
        ++next;
        if (*first == value)
            erase(first);
        first = next;
    }
}
#pragma auto_inline(on)

struct STCRemoval {
    CIntelligentPointer PThing;                             // +0x00
    CTCBase*            PTC;                                // +0x08
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    SEntry* end_of_storage;                                 // +0x08
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    SEntry* end() { return last; }
    SEntry* find(long type)
    {
        long key = type;
        SEntry* it = LowerBound(&key);
        if (it == last || type < it->type)
            it = last;
        return it;
    }
    static __forceinline SEntry* copy(SEntry* first, SEntry* lastIt, SEntry* result)
    {
        for (int n = int(lastIt - first); n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    __forceinline SEntry* erase(SEntry* position)
    {
        if (position + 1 != last)
            copy(position + 1, last, position);
        --last;
        return position;
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries; the landed
// 0x00848730 idiom): the key's address does not escape, so its frame word is shared.
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

class CThing {
public:
    virtual bool WorldUpdate();
    unsigned char           _pad04[0x44 - 0x04];
    CInterfaceMap           Components;                     // +0x44
    unsigned char           _pad50[4];
    CRetailList<CTCBase*>   UpdateComponentsList;           // +0x54
    unsigned char           _pad58[4];                      // +0x58 SerialisationSectionName
    CTCBase*                PTCDriver;                      // +0x5c
    unsigned char           _pad60[0x91 - 0x60];
    unsigned char           m_flags;                        // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    static CRetailList<STCRemoval> TCsToBeRemoved;          // 0x013B8A24
};

// The thing behind an intelligent pointer is still there and not dead (descriptive name; retail inlines it).
inline bool IsThingAlive(const CBaseIntelligentPointer& pointer)
{
    return pointer.GetPItem() != 0 && !pointer.GetPItem()->IsDead();
}

bool CThing::WorldUpdate()
{
    for (CListIterator<CTCBase*> it = UpdateComponentsList.begin(); it != UpdateComponentsList.end(); ++it) {
        if (IsDead())
            return false;
        CTCBase* tc = *it;
        if (!tc->PendingRemoval)
            tc->FrameUpdate();
    }
    if (IsDead())
        return false;
    CListIterator<STCRemoval> it = TCsToBeRemoved.begin();
    while (it != TCsToBeRemoved.end()) {
        if (!IsThingAlive(it->PThing))
            it = TCsToBeRemoved.erase(it);
        else if (it->PThing.GetPItem() == this) {
            CTCBase* tc = it->PTC;
            if (tc != 0) {
                if (tc->IsDriver())
                    PTCDriver = 0;
                UpdateComponentsList.remove(tc);
                long type = tc->GetInterfaceType();
                CInterfaceMap::SEntry* entry = Components.find(type);
                while (entry != Components.end() && entry->component != tc && entry->type == type)
                    ++entry;
                Components.erase(entry);
                delete tc;
                it->PTC = 0;
            }
            it = TCsToBeRemoved.erase(it);
        } else
            ++it;
    }
    return false;
}