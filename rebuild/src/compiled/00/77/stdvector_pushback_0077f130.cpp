// std::vector::push_back(const std::pair<CCharString, C3DVector>& __x) -- (vector<std::pair<CCharString, C3DVector> >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@U?$pair@VCCharString@@VC3DVector@@@std@@V?$allocator@U?$pair@VCCharString@@VC3DVector@@@std@@@2@@std@@QAEXABU?$pair@VCCharString@@VC3DVector@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077F130, 84 bytes, thiscall, `ret 4`.
// Identity (proven): its overflow call is the landed vector<pair<CCharString, C3DVector> >::_M_insert_overflow 0x0077EF30 (16-byte elements; the element named by that TU's landed sort helpers 0x0077EA10 / 0x0077E6A0); its caller is 0x0077F270 in the same TU.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the element's implicit copy constructor inline),
//   vector::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x0077EF30.
// Shape (family M087, representative 0x004BB720): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);`; speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCharString {                                         // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
class C3DVector {                                         // 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
namespace std {
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x0077EF30
};
template void vector<std::pair<CCharString, C3DVector> >::push_back(const std::pair<CCharString, C3DVector>& __x);
}