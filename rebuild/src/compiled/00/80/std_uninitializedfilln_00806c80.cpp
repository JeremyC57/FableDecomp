// std::__uninitialized_fill_n(NPlayerTargeting::CTargetPoint* first, unsigned int n, const NPlayerTargeting::CTargetPoint& x, const __false_type&) -- STLport placement copy-construct n 28-byte records from x, return the end of the run
// Leaf: ??$__uninitialized_fill_n@PAVCTargetPoint@NPlayerTargeting@@IV12@@std@@YIPAVCTargetPoint@NPlayerTargeting@@PAV12@IABV12@ABU__false_type@0@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806C80, 98 bytes, __fastcall (first in ecx, n in edx, &x and the empty
//   __false_type tag reference on the stack), `ret 8`.
// Identity: its only caller is 0x00809366 in the landed vector<NPlayerTargeting::CTargetPoint>::_M_insert_overflow 0x008092D0; reloc_bindings.tsv binds 0x00806C80 to exactly this symbol.
//   Element = Ego_r.pdb NPlayerTargeting::CTargetPoint (class) {CIntelligentPointer<CThing> PThing +0; Index +8; C3DVector TargetingPosition +0xC; FrameRejected +0x18}; implicit copy constructor, the record of the landed __uninitialized_copy 0x00806C00 and _M_insert_overflow 0x008092D0.
//   The catalog label "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" is descriptive / propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50 (on x), SetPItem 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C (RTTI .?AV?$CIntelligentPointer@VCThing@@@@).
// Shape: STLport 4.6 _uninitialized.h verbatim (`cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`,
//   unsigned size_t -> `test edx,edx ; jbe`), placement-new null test, the element's implicit copy: the intelligent pointer first (base ctor, vtable, Get/SetPItem), then Index, the vector and FrameRejected.
//   Register lever: the intelligent pointer's __forceinline copy constructor names its item
//   (`CBaseObject* item = other.GetPItem(); SetPItem(item);`, the landed 0x007B8230 model): that alone gives retail's
//   cur-in-ebx / x-in-edi assignment (the unnamed `SetPItem(other.GetPItem())` swaps them -- the earlier parked residue).
#include "fable_compiler.h"
inline void* operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    FABLE_FORCEINLINE CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { CBaseObject* item = other.GetPItem(); SetPItem(item); }
    virtual ~CIntelligentPointer() { }                      // the class's own vtable 0x01238C6C (as the landed 0x007B8230 model)
};
#include "engine/C3DVector.h"
namespace NPlayerTargeting {
class CTargetPoint {                                       // Ego_r.pdb, 28 bytes
public:
    CIntelligentPointer<CThing> PThing;                    // +0x00
    long                        Index;                     // +0x08
    C3DVector                   TargetingPosition;         // +0x0C
    long                        FrameRejected;             // +0x18
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
template NPlayerTargeting::CTargetPoint* __fastcall __uninitialized_fill_n<NPlayerTargeting::CTargetPoint*, unsigned int, NPlayerTargeting::CTargetPoint>(
    NPlayerTargeting::CTargetPoint*, unsigned int, const NPlayerTargeting::CTargetPoint&, const __false_type&);
}