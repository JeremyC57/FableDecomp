// CTCAssassinRush::CleanUpEffects() -- end the rush effects: drop the parent's rush creature mode (0x25), restore full alpha on the parent and on every living faded item, clear the faded-item list, and end the player's special-move mode when the parent is a living player-controlled thing
// Leaf: ?CleanUpEffects@CTCAssassinRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AA370, 268 bytes (+4 int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCCreatureModeManager, type
//   0x31), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, CTCGraphicAppearance::SetAlpha(unsigned char)
//   0x004BFAC0, CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90, CThing::GetPlayer 0x004C7A30, the
//   player's mode-0x12 exit 0x00487150 (RemoveMode(0x12) plus a flag clear; descriptive name, manifest label
//   CHTG_Executioner::OnPredicateFail is propagated).
// Layout: CTCBase ParentThing +4; ItemAlphaList +0x64 (12-byte STLport vector of 8-byte CIntelligentPointer<CThing>;
//   Ego_r.pdb +0x64); CThing state byte +0x10 (2 = player controlled), graphic appearance +0x64, dead bit +0x91 bit 0.
// Shape: the parent is a named local (spilled for the final test); the mode lookup is the out-parameter GetInterface;
//   the alpha restore is an inline CThing::SetAlpha (one appearance load for the test and the call); the alpha loop
//   caches the end pointer and uses the inline IS_THING_ALIVE; copy/_Destroy are `__forceinline`; the list is cleared through STLport
//   erase(begin(), end()) = the inlined counted copy (intelligent-pointer operator=), the virtual destructor of each
//   dropped element (`push 0 ; call [vtbl]`) and the finish store.
// Count shape: clear() names its two iterator locals in evaluation order (`iterator last = end(); iterator first =
//   begin();`) and copies from `last.p` to `_M_finish`: the finish load then defines the copy-source variable (esi) and the
//   `finish - last` count is a copy of it (`mov esi,[ebx+68h] ; mov edi,[ebx+64h] ; mov eax,esi ; sub eax,esi`); through
//   erase(begin(), end()) the load is the count temp and the variable its copy (`mov eax,.. ; mov esi,eax ; sub eax,eax`).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
struct CPlayer {
    void EndSpecialMoveMode();                  // 0x00487150
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pGraphicAppearance; // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    CPlayer* GetPlayer_004c7a30();                       // 0x004C7A30
    void SetAlpha(unsigned char alpha)
    {
        if (m_pGraphicAppearance)
            m_pGraphicAppearance->SetAlpha(alpha);
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
template <class T> __forceinline T* copy(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    struct iterator {                           // the iterator object (not CSE'd with the member loads)
        T* p;
        explicit iterator(T* q) : p(q) {}
    };
    T* begin_ptr() { return _M_start; }
    T* end_ptr() { return _M_finish; }
    iterator begin() { return iterator(_M_start); }
    iterator end() { return iterator(_M_finish); }
    __forceinline iterator erase(iterator first, iterator last)
    {
        T* i = copy(last.p, _M_finish, first.p);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    __forceinline void clear()
    {
        iterator last = end();
        iterator first = begin();
        T* i = copy(last.p, _M_finish, first.p);
        _Destroy(i, _M_finish);
        _M_finish = i;
    }
};

class CTCAssassinRush {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x64 - 0x08];
    CRetailVector< CIntelligentPointer<CThing> > ItemAlphaList;   // +0x64
    void CleanUpEffects();
};

void CTCAssassinRush::CleanUpEffects()
{
    CThing* parent = ParentThing;
    CTCCreatureModeManager* modes;
    if (parent->GetInterface(&modes, 0x31))
        modes->RemoveMode(0x25);
    parent->SetAlpha(0xff);
    for (CIntelligentPointer<CThing>* it = ItemAlphaList.begin_ptr(), *last = ItemAlphaList.end_ptr(); it != last; ++it) {
        if (IS_THING_ALIVE(*it))
            it->GetPItem()->SetAlpha(0xff);
    }
    ItemAlphaList.clear();
    if (!(parent->m_stateFlags & 1) && parent->m_state == 2)
        parent->GetPlayer_004c7a30()->EndSpecialMoveMode();
}