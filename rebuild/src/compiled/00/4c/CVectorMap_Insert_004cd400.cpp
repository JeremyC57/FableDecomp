// CVectorMap::Insert(const K& key, const V& value) -- (CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >) append the (key, value) pair to the unsorted vector map and mark it dirty
// Leaf: ?Insert@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEXABW4ETCInterfaceType@@ABQAVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CD400, 94 bytes (+ int3 padding to 96), thiscall `ret 8`.
// Identity (proven): CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::Insert; evidence: its grow callee 0x004CCCE0 is otherwise called by CThing::AddTC 0x004C9D60, which inlines this Insert (pair push, Dirty at +0x0D) on the vector map at this+0x44 -- Ego_r.pdb's CThing::Components (+0x48 there), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >, CThing's only vector map -- and by 0x004CD2F0; FableWin.pdb names this Insert
// Label: the manifest's "std_vector_push_back_pair_64bit" is propagated/behavioural (family H069: six Insert bodies over 8-byte pairs of two 4-byte
//   halves; retail has no identical-code folding, so each address is one instantiation). No call, jump or data reference reaches
//   this COMDAT: its users inline it.
// Callee: vector<pair<K, V> >::_M_insert_overflow(pos, x, tag, fill_len, atend) @ 0x004CCCE0 (landed: std_vectorReallocAndCopy_004ccce0, element descriptive there).
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D (as the landed
//   CVectorMap<unsigned __int64, bool>::Insert 0x00A83770); local classes, Review at landing.
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the pair temporary in an 8-byte frame (key in ecx, value in edx),
//   STLport push_back's placement copy from the registers behind the null test and `++_M_finish`, or _M_insert_overflow with the
//   empty tag homed in the dead `key` slot; Dirty set on both exits. The register forwarding needs the same-TU stand-in body of
//   _M_insert_overflow (reads x, no escape); declared only, VC7.1 re-reads the temporary from the frame.
inline void* operator new(unsigned int, void* where) { return where; }
enum ETCInterfaceType { };
class CTCBase;

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x004CCCE0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in body of the out-of-line grow 0x004CCCE0 (it reads x and never lets its address escape).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    for (; __fill_len != 0; --__fill_len, ++__position)
        _Construct(__position, __x);
    if (__atend)
        this->_M_finish = __position;
}
#pragma auto_inline(on)
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    void Insert(const K& key, const V& value)
    {
        push_back(std::pair<K, V>(key, value));
        Dirty = true;
    }
};

template void CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase* > >::Insert(const ETCInterfaceType& key, CTCBase* const& value);