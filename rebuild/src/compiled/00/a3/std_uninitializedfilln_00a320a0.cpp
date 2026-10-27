// std::__uninitialized_fill_n(pair<unsigned long, C3DDummyObject>* first, unsigned int n, const pair<unsigned long, C3DDummyObject>& x, const __false_type&) -- STLport placement fill: copy-construct x into n raw slots, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@KVC3DDummyObject@@@std@@IU12@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A320A0, 49 bytes, /Gr `__fastcall` (first in ecx, n in edx; &x and the tag reference on the stack),
//   `ret 8`.
// Identity (proven): only caller vector<pair<unsigned long, C3DDummyObject> >::_M_insert_overflow 0x00A33E80 (bound ?_M_insert_overflow@?$vector@U?$pair@KVC3DDummyObject@@@std@@@std@@...; reached from the landed CVectorMap<unsigned long, C3DDummyObject>::Insert 0x00A349B0); 56-byte stride = the key + Ego_r.pdb C3DDummyObject (CMatrix3x4 Transform, long BoneIndex: 13 dwords).
// Shape (family K022, representative 0x00758720): STLport 4.6 `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);
//   return __cur;` (unsigned n: `test edx,edx ; jbe`), the placement new's null test, STLport's memberwise pair copy
//   (the key word, then the trivially copyable record as one `rep movsd`).
inline void* operator new(unsigned int, void* where) { return where; }
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {                                   // Ego_r.pdb, 0x34 bytes
public:
    CMatrix3x4 Transform;                                // +0x00
    long BoneIndex;                                      // +0x30
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
typedef pair<unsigned long, C3DDummyObject> _Elem;
template _Elem* __fastcall __uninitialized_fill_n<_Elem*, unsigned int, _Elem>(_Elem*, unsigned int, const _Elem&, const __false_type&);
} // namespace std