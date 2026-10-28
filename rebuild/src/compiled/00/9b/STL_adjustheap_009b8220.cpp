// std::__adjust_heap(pair<unsigned long,CDefClassInfo>* first, int holeIndex, int len, pair<unsigned long,CDefClassInfo> val, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp) -- STLport heap sift-down of the def-class registry: walk the hole to a leaf along the larger CRC key, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B8220, 272 bytes (+ int3 padding), `ret 20h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 24-byte value and the empty comparator on the stack: 4 + 0x18 + 4 = 0x20).
// Element: the 24-byte pair<unsigned long, CDefinitionManager::CDefClassInfo> (`lea eax,[esi+esi*2]` * 8):
//   CRC key +0, CDefClassInfo +4 (Ego_r.pdb: CCharString Name +0, AllocFunc +4, CArray<unsigned long>
//   GlobalIndices +8; the retail CArray is the 12-byte STLport vector, not the PDB's 16).
// Callees: CCharString::operator= @ 0x0099EFB0, vector<unsigned long>::operator= @ 0x0045BC09 (the implicit
//   element assignment: key, Name, AllocFunc dword, GlobalIndices), CCharString(const CCharString&) @ 0x0099EC30,
//   vector<unsigned long>(const vector&) @ 0x00535470 (the memberwise pair copy built in __push_heap's argument
//   slot), __push_heap @ 0x009B77B0 (same instantiation), free @ 0x00BFEA14 (inline ~_Vector_base of val),
//   ~CCharString @ 0x0099EAE0 (val.second.Name).
// Callers: __pop_heap 0x009B8770, __make_heap 0x009B8820 (same STLport sort family).
// Shape: STLport 4.6 `__adjust_heap`; topIndex is homed in the `push ecx` slot; the key compare is UNSIGNED
//   (`jae` over the `dec esi`). The catalog label `_Adjust_heap<...CDefClassInfo...>` (Dinkumware spelling) is
//   this STLport `__adjust_heap`.
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

namespace std {
template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector(const vector<_Tp, _Alloc>& __x);                            // vector<unsigned long>: 0x00535470
    ~vector() {}
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // vector<unsigned long>: 0x0045BC09
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
};

class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                               // Ego_r.pdb (24 bytes; retail 20: CArray 16 -> 12)
    public:
        CCharString            Name;                    // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();          // +0x04
        CArray<unsigned long>  GlobalIndices;           // +0x08
    };
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x009B77B0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CDefClassInfoPair*, int, int, CDefClassInfoPair,
                                       CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}