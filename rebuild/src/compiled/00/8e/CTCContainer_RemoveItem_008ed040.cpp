// CTCContainer::RemoveItem(long) -- remove the first container item whose definition index matches
// Leaf: ?RemoveItem@CTCContainer@@
// Retail 0x008ED040, 0x4D bytes, thiscall `ret 4`. Manifest label "std_vector_erase_single<CTCSkeletalMorph>" is
//   propagated; the PDB name set has `?RemoveItem@CTCContainer@@QAEXJ@Z` and the landed caller
//   CGameScriptInterface::RemoveItemFromContainer 0x0089E810 already names it.
// Callees: _STL::__find_if<pair<long, CCharString>*, pred> __fastcall @ 0x008ECEF0 (defined in this TU, chunk row),
//   _STL::__copy<pair<long, CCharString>*> __fastcall @ 0x0074F640 (element assignment through CCharString::operator=
//   0x0099EFB0), CCharString::~CCharString @ 0x0099EAE0.
// Layout: retail CTCContainer = CTCBase (0xC) + ContainerItems vector<pair<long, CCharString>> at +0x0C (Ego_r.pdb
//   struct layout; STLport vector = start/finish/end-of-storage, 12 bytes); each item is 8 bytes (definition index,
//   CCharString).
// Shape: `find_if(begin, end, pred) != end` then STLport 4.x `erase(pos)` (`pos + 1 != end()` -> __copy_ptrs ->
//   __copy(first, last, result, random_access_iterator_tag(), (int*)0); `--_M_finish; _Destroy(_M_finish)`). The end
//   pointer read before the call (edi) is reused for both compares and the copy's `last` because __find_if's BODY is in
//   the TU: VC7.1 then knows the call writes no memory. With only a declaration the lane re-reads [esi+10h] after the
//   call (78v77). Both empty tags are homed in the dead `defIndex` argument slot.
class CCharString {
public:
    void* PStringData;                          // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                             // 0x0099EAE0
};

namespace _STL {
struct __false_type { __false_type() {} };
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);       // 0x0074F640 for pair<long, CCharString>

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)           // 0x008ECEF0
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

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
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
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
}

// The container's key predicate (PDB: ??0CIsItemWithDefIndex@@QAE@J@Z, ??RCIsItemWithDefIndex@@QBE_NABU?$pair@JVCCharString@@@std@@@Z).
class CIsItemWithDefIndex {
public:
    long DefIndex;                              // +0x00
    CIsItemWithDefIndex(long defIndex) : DefIndex(defIndex) {}
    bool operator()(const _STL::pair<long, CCharString>& item) const { return item.first == DefIndex; }
};

class CTCContainer {
public:
    unsigned char _base[0xc];                   // +0x00 CTCBase (vptr, parent thing, flags)
    _STL::vector<_STL::pair<long, CCharString> > ContainerItems;   // +0x0c
    void RemoveItem(long defIndex);
};

void CTCContainer::RemoveItem(long defIndex)
{
    _STL::vector<_STL::pair<long, CCharString> >::iterator it =
        _STL::find_if(ContainerItems.begin(), ContainerItems.end(), CIsItemWithDefIndex(defIndex));
    if (it != ContainerItems.end())
        ContainerItems.erase(it);
}