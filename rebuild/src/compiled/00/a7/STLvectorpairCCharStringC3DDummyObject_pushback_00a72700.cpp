// std::vector<pair<CCharString,C3DDummyObject>>::push_back(const pair<CCharString,C3DDummyObject>& x) -- append one named dummy object: copy-construct it at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72700, 86 bytes, thiscall `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the in-place pair copy), vector::
//   _M_insert_overflow 0x00A720E0 (thiscall: position, x, the __false_type tag by reference, fill count 1, at-end
//   true; `ret 14h`).
// Identity: the manifest's "Vector_PushBack_CCharString" is descriptive. The 56-byte element (the `add 38h` step and
//   the 13-dword `rep movsd` of the C3DDummyObject after the name) is pair<CCharString, C3DDummyObject>, the entry of
//   CPhysicsMesh::DummyObjects (CVectorMap's 12-byte STLport vector at +4).
// Shape: STLport 4.6 `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- _Construct's placement new is the nothrow
//   STLport one (null-tested); the empty tag temporary is homed in x's argument slot once x is in ecx.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }   // STLport placement new

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace std {

class __false_type { public: __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x00A720E0
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<pair<CCharString, C3DDummyObject> >::push_back(const pair<CCharString, C3DDummyObject>&);

}