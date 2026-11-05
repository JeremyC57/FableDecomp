// std::_Destroy(CTCSmokeGenerator::CGenerationPos* first, CTCSmokeGenerator::CGenerationPos* last) -- STLport range destroy: run each element's destructor
// Leaf: ??$_Destroy@VCGenerationPos@CTCSmokeGenerator@@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722E80, 34 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret`.
// Identity (proven): its only caller is vector<CGenerationPos>::operator= 0x00722F30, tail-called by CTCSmokeGenerator vtable 0x01265D74 slot 12 (0x00722570) on this+0x10 = Ego_r.pdb CTCSmokeGenerator::GenerationPositions (vector<CTCSmokeGenerator::CGenerationPos>, 20-byte element with PEmitter at +0x0C).
// Callees: the element destructor's one call -- ~CBaseIntelligentPointer 0x00A01C10 on PEmitter +0x0C.
// Shape (family K118, representative 0x004D0EE0): STLport 4.6 `_Destroy(first, last)` -> `__destroy_aux(..., __false_type)` loop,
//   emitted out of line (the vector operator= calls it); the element's implicit destructor inlines to the member's call.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
#include "engine/C3DVector.h"
class CTCSmokeGenerator {
public:
    class CGenerationPos {                               // Ego_r.pdb 20 bytes
    public:
        C3DVector Pos;                                   // +0x00
        CIntelligentPointer<CThing> PEmitter;            // +0x0C
    };
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void __fastcall _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
}
template void __fastcall std::_Destroy<CTCSmokeGenerator::CGenerationPos >(CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*);