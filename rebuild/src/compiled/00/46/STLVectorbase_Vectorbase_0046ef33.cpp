// std::_Vector_base::_Vector_base(unsigned int n, const allocator<pair<unsigned long,CDefClassBase::CSubDefInfo> >& a) -- allocate room for exactly n sub-def map entries (12 bytes each) and leave the vector empty
// Instantiation: std::_Vector_base<pair<unsigned long,CDefClassBase::CSubDefInfo>,allocator<...> >::_Vector_base
// Leaf: ??0?$_Vector_base@U?$pair@KVCSubDefInfo@CDefClassBase@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EF33, 56 bytes (size-optimised def TU, no padding), thiscall, `ret 8`.
// Identity: called only by vector<pair<unsigned long, CDefClassBase::CSubDefInfo> >::vector(const vector&) @ 0x0046EECF
//   (call 0x0046EEEB, n = other.size()). Element = Ego_r.pdb pair<unsigned long, CDefClassBase::CSubDefInfo> (12 bytes), the
//   CParentDefClassBase SubDefInfoMap vector. The catalog name `std::vector_allocate_3xlong` is descriptive.
// Callees: malloc @ 0x00BFEA0E (Fable's STLport allocator: `n != 0 ? malloc(n * 12) : 0`).
// Shape: STLport 4.6 `_Vector_base(n, a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { _M_start =
//   _M_end_of_storage.allocate(n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + n; }` -- the three zero
//   stores from one `xor eax,eax`, the allocation and the three pointer stores in source order; `optimize("s", on)` like the
//   def module's other landed bodies (allocate `__forceinline`).
// (no frame pointer: "s" alone)
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
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

typedef pair<unsigned long, CDefClassBase::CSubDefInfo> _SubDefEntry;
template _Vector_base<_SubDefEntry, allocator<_SubDefEntry> >::_Vector_base(unsigned int, const allocator<_SubDefEntry>&);
}