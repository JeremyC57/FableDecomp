// std::vector::vector(const vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >& other) -- copy constructor of the parent def's sub-def info map vector: allocate other.size() entries and placement-copy every (key, sub-def info) pair
// Instantiation: std::vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >::vector
// Leaf: ??0?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EECF, 100 bytes (size-optimised def TU, no padding), thiscall, `ret 4`.
// Identity: called by the landed CParentDefClassBase copy constructor 0x0046EE98 (SubDefInfoMap's vector) and six more
//   CParentDefClassBase-derived copy constructors (0x00624608, 0x006249C9, 0x006250D5, 0x0067C6F5, ...). Ego_r.pdb names it
//   `??0?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@...@QAE@ABV01@@Z`. The catalog name
//   `std::vector_assign_triplet_uint` is descriptive.
// Callees: _Vector_base(unsigned int n, const allocator&) @ 0x0046EF33 (out of line).
// Shape: STLport 4.6 `vector(const _Self& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }`: size() is a signed divide by 12, the
//   get_allocator() temporary is homed in the dead top byte of the `x` slot (`lea eax,[ebp+0Bh]`), the uninitialized copy
//   (`__forceinline` under "s") is the placement-new loop with its null test, the pair copy constructor reading `second`
//   through a second induction pointer; x's finish is parked in the `x` slot. Frame pointer: `optimize("y", off)` +
//   `optimize("s", on)` as the module's other landed frame bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CDefClassBase {
public:
    class CSubDefInfo {                                     // Ego_r.pdb, 8 bytes
    public:
        long DefIndex;                                      // +0x0
        long OriginalParentDefIndex;                        // +0x4
    };
};

namespace std {
struct __false_type { };

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    _Vector_base(unsigned int __n, const _Alloc& __a);      // 0x0046EF33
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef _Alloc allocator_type;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return (const allocator_type&)this->_M_end_of_storage; }
    vector(const _Self& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    }
};

typedef pair<unsigned long, CDefClassBase::CSubDefInfo> _SubDefEntry;
template vector<_SubDefEntry, allocator<_SubDefEntry> >::vector(const vector<_SubDefEntry, allocator<_SubDefEntry> >&);
}