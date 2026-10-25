// std::vector::vector(const vector& __x) -- (vector<std::pair<C3DVector, C3DVector> >) STLport copy constructor: allocate size() elements, uninitialized-copy the source into them
// Leaf: ??0?$vector@U?$pair@VC3DVector@@V1@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076FB00, 104 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: std::vector<std::pair<C3DVector, C3DVector> >::vector(const vector&) -- proven; evidence: its callers are the push_back/insert/fill helpers of vector<vector<pair<C3DVector, C3DVector> > > (0x00770BB0, 0x00770CD0, 0x0076FCE0, 0x0076FD10) and 0x0076EA60, which loops over the weapon trails, fills a local vector<pair<C3DVector,C3DVector> > through 0x0076E4D0 and push_backs a copy into its out-parameter: CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints(vector<vector<pair<C3DVector,C3DVector> > >&) const (Ego_r name set), called from CTCMeleeTrap::PerformPossibleTrapDamage 0x0078891D; its element copy 0x0076F8E0 moves six dwords (two C3DVectors).
//   Family G068 (STLport vector copy constructors of 12/20/24-byte class elements; catalogue label "std::vector<uint[6]>::vector" is
//   propagated/behavioural).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy<const T*, T*>(first, last, result, const
//   __false_type&) 0x0076F8E0 (out of line, __fastcall, `ret 8`).
// Shape: STLport 4.6 verbatim: _Vector_base(__x.size(), __x.get_allocator()) zero-initialises the three pointers before the
//   allocation; the __false_type tag (_IsPODType of a class element) is homed in the dead __x argument slot.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
class C3DVector { public: float X, Y, Z; };               // 12 bytes
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                         // +0
    _Tp* _M_finish;                                        // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +8
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    typedef __false_type _IsPODType;                       // __type_traits<_Tp>::is_POD_type of a class element
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(this->_M_end_of_storage); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, _IsPODType());
    }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template vector<pair<C3DVector, C3DVector> >::vector(const vector<pair<C3DVector, C3DVector> >&);
}