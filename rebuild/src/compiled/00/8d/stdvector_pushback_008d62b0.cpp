// std::vector::push_back(const CUnprovenElement_008d61a0& __x) -- (vector<CUnprovenElement_008d61a0>) append, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@VCUnprovenElement_008d61a0@@V?$allocator@VCUnprovenElement_008d61a0@@@std@@@std@@QAEXABVCUnprovenElement_008d61a0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D62B0, 58 bytes, thiscall, `ret 4`.
// Identity (descriptive): the overflow insert 0x008D61A0 is the landed descriptive vector<CUnprovenElement_008d61a0>::_M_insert_overflow; its only caller is CCreatureAction_FireMissileWeapon::PerformAction 0x008D5AA0 (a local vector of two-pointer records); the element type is not proven.
// Callees: vector<CUnprovenElement_008d61a0>::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x008D61A0.
// Shape (family M027, representative 0x004428B0): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the tag temporary is homed in
//   the x argument slot only when __false_type has a user default constructor (else a zero-filled frame byte).
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CUnprovenElement_008d61a0 { public: unsigned long Word0, Word1; };   // descriptive 8-byte element (landed 0x008D61A0)
namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
};
template void vector<CUnprovenElement_008d61a0 >::push_back(const CUnprovenElement_008d61a0& __x);
}