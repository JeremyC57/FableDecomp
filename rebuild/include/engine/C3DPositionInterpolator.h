#ifndef FABLE_ENGINE_C3DPOSITIONINTERPOLATOR_H
#define FABLE_ENGINE_C3DPOSITIONINTERPOLATOR_H
// Retail opt-in FABLE_RETAIL_C3DPOSITIONINTERPOLATOR (chunk 1020, row 0x00BCF4F0 CEngineInternalPrimitive2DAnimatedMesh::Initialise):
// Ego_r TPI C3DPositionInterpolator, 28 bytes: vptr +0 (FableWin ??1C3DPositionInterpolator@@UAE@XZ is virtual), StartPosition +4,
// EndPosition +0x10. Ctors ??0C3DPositionInterpolator@@QAE@XZ / @ABVC3DVector@@0@Z (Ego_r 0x00536060) declared only.
#ifdef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#include "engine/CBaseClass.h"
// Original Ego_r base CBaseClass and native interpolation object's vtable/dtor graph; fields unchanged.
class C3DPositionInterpolator:public CBaseClass {
#else
class C3DPositionInterpolator {
#endif
public:
 C3DPositionInterpolator();
 C3DPositionInterpolator(const C3DVector& startPosition,const C3DVector& endPosition);
#ifndef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_IMPLICIT_DTOR
 virtual ~C3DPositionInterpolator();
#else
 // Opt-in (agentNF41, row 0x004BCFE0 CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos; needs _BASE): the destructor is the implicit
 // one (virtual through CBaseClass): the CEnginePrimitiveAnimation temporary's WorldPos member is destroyed by a direct ~CBaseClass
 // 0x0099A2E0 call with no vptr store, as C3DOrientationInterpolator's FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR.
#endif
#ifdef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
 // Retail opt-in (Codex pass22): both PDB GetInterpolated(float) const; native0x0077C67F..0x0077C6F7.
 C3DVector GetInterpolated(float interp) const { return StartPosition.Interpolate(EndPosition,interp); }
#endif
 C3DVector StartPosition;
 C3DVector EndPosition;
#ifdef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_GETINTERPOLATED
 // agentC30 (row 0x007E15C0): FableWin.pdb ?GetInterpolated@C3DPositionInterpolator@@QBE?AVC3DVector@@M@Z, expanded in retail
 // 0x007E17E6..0x007E1848 (`fld End ; fsub Start` x3, `fmul t`, `fadd Start`: the text of the landed out-of-line copy 0x004C2EF0).
 C3DVector GetInterpolated(float t) const { return (EndPosition - StartPosition) * t + StartPosition; }
#endif
};
FABLE_STATIC_ASSERT_AT(23, sizeof(C3DPositionInterpolator)==28);
#endif
#endif
