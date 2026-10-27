// std::vector::push_back(const QuadricOptimiserInternals::COptimisedBone& __x) -- (vector<QuadricOptimiserInternals::COptimisedBone >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@VCOptimisedBone@QuadricOptimiserInternals@@V?$allocator@VCOptimisedBone@QuadricOptimiserInternals@@@std@@@std@@QAEXABVCOptimisedBone@QuadricOptimiserInternals@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD66E0, 84 bytes, thiscall, `ret 4`.
// Identity (proven): no reference (dead COMDAT); its overflow call is vector<QuadricOptimiserInternals::COptimisedBone>::_M_insert_overflow 0x00AD5F80 (whose moves are the __uninitialized_copy<COptimisedBone*> 0x00AD16C0 of family M083); the inline copy is COptimisedBone's {BoneName, long Parent, bool Remapped, long NewIndex}.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the element's implicit copy constructor inline),
//   vector::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00AD5F80.
// Shape (family M087, representative 0x004BB720): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);`; speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCharString {                                         // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
namespace QuadricOptimiserInternals {
class COptimisedBone {                                    // Ego_r.pdb, 16 bytes
public:
    CCharString BoneName;                                 // +0x0
    long Parent;                                          // +0x4
    bool Remapped;                                        // +0x8
    long NewIndex;                                        // +0xc
};
}
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00AD5F80
};
template void vector<QuadricOptimiserInternals::COptimisedBone >::push_back(const QuadricOptimiserInternals::COptimisedBone& __x);
}