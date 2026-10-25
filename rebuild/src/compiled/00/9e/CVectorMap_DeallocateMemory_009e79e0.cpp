// CVectorMap::DeallocateMemory() -- CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolInfo> >::DeallocateMemory: erase every entry, then swap the map with an empty one so its block is destroyed and freed (and the map is no longer dirty)
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E79E0, 103 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb name set `?DeallocateMemory@?$CVectorMap@VCCharString@@VCSymbolInfo@CSymbolMap@@V?$CCompareString@
//   VCSymbolInfo@CSymbolMap@@@@@@QAEXXZ`; callers CSymbolMap::Clear 0x009E5250 (landed, on the SLONGMap member at +4) and
//   CSymbolMap::LoadFromFile 0x009E5C90. The catalog label "vector_swap_move" is descriptive.
// Callees: STLport __copy @ 0x009E6600 (landed; first/last in ecx/edx, result + random_access_iterator_tag + a null
//   Distance* on the stack), __destroy_aux @ 0x006FBCC0 (three CCharString destructors per entry), free @ 0x00BFEA14.
// Layout: the retail sorted-vector map (GOTCHAS): the 12-byte STLport vector +0x00, the empty Compare +0x0C, bool Dirty
//   +0x0D.
// Shape: `Map.clear()` = erase(begin, end) (`copy(end, end, begin)`, destroy the tail, the finish store), then
//   `CVectorMap empty; swap(empty);` -- STLport's pointer swaps through references and the Dirty exchange fold the
//   empty map away (its zeros are stored straight into this map, Dirty last), and its destructor destroys and frees
//   the old block; every empty tag is a one-byte temporary in the `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
};

class CSymbolMap {
public:
    class CSymbolInfo {                           // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                    // +0x00
        CCharString EntryName;                    // +0x04
        long        Val;                          // +0x08
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x009E6600
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x006FBCC0
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                // +0x00
    _Tp* _M_finish;                               // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (int*)0);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
    }
};
}

template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
};

template <class T> class CCompareString {                      // empty key order (CCharString::FastCompare)
};

template <class K, class V, class C> class CVectorMap {
public:
    CArray<std::pair<K, V> > Map;                // +0x00
    C                         Compare;            // +0x0C
    bool                      Dirty;              // +0x0D
    CVectorMap() : Dirty(false) { }
    void swap(CVectorMap& other)
    {
        Map.swap(other.Map);
        std::swap(Dirty, other.Dirty);
    }
    void DeallocateMemory()
    {
        Map.clear();
        CVectorMap<K, V, C> empty;
        swap(empty);
    }
};

template void CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolMap::CSymbolInfo> >::DeallocateMemory();