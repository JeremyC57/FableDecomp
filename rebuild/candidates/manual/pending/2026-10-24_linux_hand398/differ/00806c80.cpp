// std::__uninitialized_fill_n(NPlayerTargeting::CTargetPoint* first, unsigned int n, const NPlayerTargeting::CTargetPoint& x, const __false_type&) -- STLport uninitialized_fill_n over target points: copy-construct n copies of x from first; returns the advanced pointer
// Grade: DIFFER(98v98) -- register allocation only: retail keeps cur in ebx and the value reference in edi, ours swaps them (same instructions otherwise); tried the STLport `__cur` copy, iterating `__first` directly, CTargetPoint's implicit copy constructor
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806C80, 98 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx, n in
//   edx, the value and tag references on the stack), `ret 8`.
// Name: STLport 4.x `__uninitialized_fill_n<CTargetPoint*, unsigned int, CTargetPoint>` (the vector<CTargetPoint>
//   fill-insert helper); the catalog's "fill<C3DAnimationStateAlpha*, C3DAnimationStateAlpha>" is propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90; the
//   CIntelligentPointer<CThing> vtable 0x01238C6C is stored after the base constructor.
// Layout: Ego_r.pdb NPlayerTargeting::CTargetPoint (0x1c): PThing +0, Index +8, TargetingPosition +0xc, FrameRejected
//   +0x18 (the landed 0x00806C00 model).
// Shape: `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- n unsigned (`test edx,edx ; jbe`, `dec ; jne`),
//   placement new null-tested per element; CTargetPoint's copy constructor (CIntelligentPointer copy, Index, C3DVector's
//   user copy constructor, FrameRejected) inlined through the strength-reduced cur+0x14 pointer.
inline void* operator new(unsigned int, void* p) { return p; }

struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template NPlayerTargeting::CTargetPoint* __fastcall __uninitialized_fill_n(NPlayerTargeting::CTargetPoint*, unsigned int,
                                                                           const NPlayerTargeting::CTargetPoint&, const __false_type&);
}
