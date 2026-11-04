// std::__uninitialized_fill_n(CEngineCamera::CDebugScriptElement* first, unsigned int n, const CEngineCamera::CDebugScriptElement& x, const __false_type&) -- STLport placement-copy of one camera debug-script element into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAUCDebugScriptElement@CEngineCamera@@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31A90, length 80 bytes, `ret 8` (/Gr: first in ecx -> eax as `cur`, n
//   in edx moved to ecx and counted down; &x and the empty __false_type tag on the stack), the end pointer in eax.
// Caller: vector<CEngineCamera::CDebugScriptElement>::_M_insert_overflow @ 0x00B31B10 (call 0x00B31BB2, the n > 1 arm),
//   reached from push_back @ 0x00B31C10 <- CEngineCamera::ConsoleAddDebugScript @ 0x00B317A0 (`DebugScript` at
//   CEngineCamera +0x208, the vtable 0x012A12A4 owner).
// Element (Ego_r.pdb): CEngineCamera::CDebugScriptElement, 0x20 bytes = C3DVector WorldPos +0, C3DVector LookVector
//   +0x0C, float FOV +0x18, float TimeToNextElement +0x1C; its implicit copy constructor is memberwise over the
//   C3DVector user copy constructors, which keeps the destination through a second +0x14-biased induction register.
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned size_t: `test edx,edx ; jbe`), the placement-new null test inline.
// Label: catalog "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" is a propagated label.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

#include "engine/C3DVector.h"

class CEngineCamera {
public:
    struct CDebugScriptElement {                           // PDB sizeof 0x20
        C3DVector WorldPos;                                 // +0x00
        C3DVector LookVector;                               // +0x0C
        float     FOV;                                      // +0x18
        float     TimeToNextElement;                        // +0x1C
    };
};

namespace std {
struct __false_type { };
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

template CEngineCamera::CDebugScriptElement* __fastcall std::__uninitialized_fill_n<CEngineCamera::CDebugScriptElement*,
    unsigned int, CEngineCamera::CDebugScriptElement>(CEngineCamera::CDebugScriptElement*, unsigned int,
                                                      const CEngineCamera::CDebugScriptElement&, const std::__false_type&);