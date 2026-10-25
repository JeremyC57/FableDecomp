// std::__uninitialized_fill_n(pair<CCharString,C3DHelperPoint>* first, unsigned int n, const pair<CCharString,C3DHelperPoint>& x, const __false_type&) -- copy-construct n copies of a helper-point map entry into raw storage; returns the end of the filled run
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6EDE0, 72 bytes (+8 int3), /Gr `__fastcall` (first in ecx, n in edx,
//   the value and the tag references on the stack, `ret 8`).
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30.
// Label correction: catalogued as `fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>` (propagated). The 20-byte
//   stride with a CCharString copy at +0 is the CPhysicsMesh::HelperPoints entry pair<CCharString, C3DHelperPoint>
//   (as the swap 0x00A6E5E0 of the same TU).
// Shape: STLport `_ForwardIter __cur = __first; for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return
//   __cur;` with the unsigned count (`test edx,edx ; jbe`), the placement-new null test, and STLport's explicit pair
//   copy constructor: the string copy, then the nested C3DHelperPoint copy (C3DVector's user memberwise copy + the
//   bone index) through a second induction register (`lea esi,[edi+0Ch]`, stores at esi-8..esi+4).
inline void* __cdecl operator new(unsigned int, void* p) throw() { return p; }   // placement new (null-tested result)

class C3DVector {                                       // user memberwise copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

namespace std {

class __false_type { public: __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template THelperEntry* __fastcall __uninitialized_fill_n<THelperEntry*, unsigned int, THelperEntry>(THelperEntry*, unsigned int, const THelperEntry&, const __false_type&);

}