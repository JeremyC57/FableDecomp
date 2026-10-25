// std::vector::push_back(const pair<CCharString,unsigned long>& x) -- vector<pair<CCharString,unsigned long>>::push_back: copy-construct x at the end, or grow through _M_insert_overflow when full
// Flags: /Gr
// Leaf: ?push_back@?$vector@U?$pair@VCCharString@@K@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B7CA0, 72 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: the element is 8 bytes {CCharString, dword} = the CVectorMap<CCharString, unsigned long> storage whose grow
//   path 0x009B71B0 the landed CDefinitionManager::CheckDependencies 0x009B05F0 also calls (its inline push_back is this
//   body).  Catalogued as `StdVector_PushBack_CCharString`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x009B71B0
//   (the __false_type tag lives in x's dead argument slot).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- placement new keeps its null test.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace std {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __false_type _IsPODType;
    void push_back(const _Tp& __x);
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);   // 0x009B71B0
private:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};

template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<pair<CCharString, unsigned long> >::push_back(const pair<CCharString, unsigned long>&);
}