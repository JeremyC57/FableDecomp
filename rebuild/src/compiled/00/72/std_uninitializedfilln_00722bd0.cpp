// std::__uninitialized_fill_n(CTCSmokeGenerator::CGenerationPos* first, unsigned int n, const CTCSmokeGenerator::CGenerationPos& x, const __false_type&) -- STLport placement copy-construct n 20-byte records from x, return the end of the run
// Leaf: ??$__uninitialized_fill_n@PAVCGenerationPos@CTCSmokeGenerator@@IV12@@std@@YIPAVCGenerationPos@CTCSmokeGenerator@@PAV12@IABV12@ABU__false_type@0@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722BD0, 86 bytes, __fastcall (first in ecx, n in edx, &x and the empty
//   __false_type tag reference on the stack), `ret 8`.
// Identity: its only caller is 0x007231D2 in vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow 0x00723110 (bound ?_M_insert_overflow@?$vector@VCGenerationPos@CTCSmokeGenerator@@...; the landed push_back 0x00723270 calls it).
//   Element = Ego_r.pdb CTCSmokeGenerator::CGenerationPos (class) {C3DVector Pos +0; CIntelligentPointer<CThing> PEmitter +0xC}, the record of the landed __uninitialized_copy 0x00722B70 of the same vector.
//   The catalog label "Vector_Fill_CountedPtr_Pairs" is descriptive / propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50 (on x), SetPItem 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C (RTTI .?AV?$CIntelligentPointer@VCThing@@@@).
// Shape: STLport 4.6 _uninitialized.h verbatim (`cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`,
//   unsigned size_t -> `test edx,edx ; jbe`), placement-new null test, the element's implicit copy: the C3DVector (user copy, three dwords addressed off the pointer member) then the intelligent pointer at +0xC.
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
class CTCSmokeGenerator {
public:
    class CGenerationPos {
    public:
        C3DVector Pos;                                    // +0x00
        CIntelligentPointer<CThing> PEmitter;             // +0x0C
    };
};
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
template CTCSmokeGenerator::CGenerationPos* __fastcall __uninitialized_fill_n<CTCSmokeGenerator::CGenerationPos*, unsigned int, CTCSmokeGenerator::CGenerationPos>(
    CTCSmokeGenerator::CGenerationPos*, unsigned int, const CTCSmokeGenerator::CGenerationPos&, const __false_type&);
}