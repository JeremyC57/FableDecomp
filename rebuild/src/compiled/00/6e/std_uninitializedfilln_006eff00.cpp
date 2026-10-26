// std::__uninitialized_fill_n(SProngVector* first, unsigned int n, const SProngVector& x, const __false_type&) -- STLport placement-copy of one (prong direction, blocked) pair into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EFF00, 52 bytes, `ret 8` (/Gr: first in ecx -> eax as `cur`, n in edx moved to
//   esi and counted down; &x and the empty __false_type tag reference on the stack).
// Caller: 0x006F32F8 in vector<pair<C3DVector,bool> >::_M_insert_overflow 0x006F3260 (the fill_len > 1 arm), itself
//   called from the landed CCombatRing::DebugGetAllProngVectors 0x006EB8A0.
// Element: pair<C3DVector,bool>, 13 bytes (Ego_r.pdb `pair<C3DVector,bool>` size 13): C3DVector is 1-aligned in this
//   module (CProng::Prong sits at +1), modelled under pack(1) as the landed 0x006EB8A0 / 0x006F14A0; the copy is the
//   vector's three dwords then the bool byte at +0xC.
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned size_t: `test edx,edx ; jbe`), the placement-new null test and the memberwise copy inline.
// Label: the catalog's `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` is a propagated label (the stride is 13).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

#pragma pack(push, 1)
class C3DVector { public:                                          // 1-aligned in retail
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

namespace std {
struct __false_type { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
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
}

typedef std::pair<C3DVector, bool> SProngVector;            // 13 bytes

template SProngVector* __fastcall std::__uninitialized_fill_n<SProngVector*, unsigned int, SProngVector>(
    SProngVector*, unsigned int, const SProngVector&, const std::__false_type&);