// CTCContainer::ContainsItem(long defIndex) const -- whether any container item has the given definition index
// Leaf: ?ContainsItem@CTCContainer@@QBE_NJ@Z
// Retail 0x008ED010, 33 bytes, thiscall `ret 4`.
// Naming: Ego_r.pdb ?ContainsItem@CTCContainer@@QBE_NJ@Z; the next function 0x008ED040 is the landed
//   CTCContainer::RemoveItem(long) (same find_if over ContainerItems with CIsItemWithDefIndex). The callee 0x008ECF60 is
//   the const-iterator twin of the landed __find_if 0x008ECEF0 (Ego_d.pdb has both _Find_if<pair<long,CCharString>*,
//   CIsItemWithDefIndex> and _Find_if<const pair<long,CCharString>*, CIsItemWithDefIndex>): it compares each 8-byte
//   item's first dword (the definition index) with the predicate key, unrolled x4. Called only from
//   CGameScriptInterface::IsObjectInThingsPossession 0x00897220 on the owner's container component. FINDINGS chunk 405.
//   Was landed under the propagated catalog name "CTCBuyableHouse::MovedInNotification" as CTCPossessionList_008ed010::
//   ContainsThing(CThing*) (the key is a long definition index, not a thing).
// Layout: retail CTCContainer = CTCBase (0xC) + ContainerItems vector<pair<long, CCharString>> at +0x0C (Ego_r.pdb, as
//   the landed RemoveItem 0x008ED040). The retail STL namespace is std (RTTI); STLport pair is a struct.
// Shape: `find_if(begin, end, pred) != end ? true : false` (the bare compare widens through ecx: `setne cl ; mov al,cl`); the predicate passes by value (`push edx`), the empty tag is homed in the
//   dead defIndex argument slot (`lea eax,[esp+8]`), and the end pointer read before the call (esi) is reused for the
//   compare because __find_if's body is in the TU (as in RemoveItem).
class CCharString {
public:
    void* PStringData;                          // +0x00
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
};

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)           // 0x008ECF60 for const pair<long, CCharString>*
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}

// The container's key predicate (PDB: ??0CIsItemWithDefIndex@@QAE@J@Z, ??RCIsItemWithDefIndex@@QBE_NABU?$pair@JVCCharString@@@std@@@Z).
class CIsItemWithDefIndex {
public:
    long DefIndex;                              // +0x00
    CIsItemWithDefIndex(long defIndex) : DefIndex(defIndex) {}
    bool operator()(const std::pair<long, CCharString>& item) const { return item.first == DefIndex; }
};

class CTCContainer {
public:
    unsigned char _base[0xc];                   // +0x00 CTCBase (vptr, parent thing, flags)
    std::vector<std::pair<long, CCharString> > ContainerItems;   // +0x0c
    bool ContainsItem(long defIndex) const;
};

bool CTCContainer::ContainsItem(long defIndex) const
{
    return std::find_if(ContainerItems.begin(), ContainerItems.end(), CIsItemWithDefIndex(defIndex)) != ContainerItems.end()
        ? true : false;
}
