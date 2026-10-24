// std::vector<pair<CCharString,CCharString> >::push_back(const pair<CCharString,CCharString>& x) -- STLport push_back for the weapon-trail helper-name pairs: copy-construct x at the end, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007706A0, 78 bytes, thiscall `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the pair's first and second),
//   vector<pair<CCharString,CCharString> >::_M_insert_overflow(pos, x, const __false_type&, 1, true) @ 0x007704A0.
// Element: std::pair<CCharString,CCharString> (8 bytes) -- CTCWeaponTrail::HelperNames (Ego_r.pdb); the same element as
//   the fill 0x0076F740 and the overflow 0x007704A0.
// Label correction: catalogued as `Vector_PushBack_String`; the body is STLport `vector::push_back`.
// Shape: STLport 4.x verbatim: `if (_M_finish != _M_end_of_storage._M_data) { _Copy_Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement copy null-tests
//   the slot, STLport's pair copy constructor copies first then second, `++_M_finish` re-reads the member (`add
//   [edi+4],8`), and the tag temporary takes the dead `x` argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x007704A0
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Copy_Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template class vector<pair<CCharString, CCharString> >;

}