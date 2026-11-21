// NCameraModes::CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative(const CIntelligentPointer<CThing>, const C3DVector&, const C3DVector&, const C3DVector&, float, long, bool) -- constructor: the FOV zoom base, keep the thing, and record the end position's offset from the look-at against the thing's facing (Mag, Angle about Axis)
// Leaf: ??0CScriptModuleFOVZoomStayRelative@NCameraModes@@QAE@V?$CIntelligentPointer@VCThing@@@@ABVC3DVector@@11MJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00852E10, 445 bytes, thiscall, `ret 20h` (the by-value CIntelligentPointer<CThing> is two dwords and
//   is destroyed by the callee, 0x00852FBC). Vtable 0x01275A94.
// Identity: Ego_r.pdb / FableWin.pdb (0x0283B2F0) / Ego_d.pdb ??0CScriptModuleFOVZoomStayRelative@NCameraModes@@QAE@V?$CIntelligentPointer@VCThing@@@@ABVC3DVector@@11MJ_N@Z.
//   Ego_d.pdb (debug length 308): parameters thing, look_at_pos, start_pos, end_pos, fraction_of_fov_to_reach, world_frames,
//   shortenonocclusion; locals facing, rel (facing declared first).
// Callees: CScriptModuleFOVZoom::CScriptModuleFOVZoom 0x00852560; CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem
//   0x00A01B90, dtor 0x00A01C10; CTCPhysicsBase::GetFacingVector() const (virtual slot 70, +0x118); _CIacos 0x00BFEBA2 (GFArcCos).
// Ego_d lines (VC10 /Od): 384 (122 B) the base and Thing(thing) initialisers; 385 (38 B) facing from the thing's physics component (two
//   calls + the virtual call); 386 (16 B) `rel = end_pos - look_at_pos` (operator- returned straight into rel); 387 (17 B) Mag =
//   rel.GetMagnitude() (call + store at this+0x9C); 388 (8 B) rel.Normalise(); 389 (32 B) Angle = GFArcCos(facing.Dot(rel)) (two calls, the
//   double argument, the store); 390 (41 B) Axis = rel ^ facing (the cross-product call into a temporary, 16 B, + the memberwise copy
//   through `add ecx,8Ch`, 25 B); 391 (34 B) the by-value thing's destructor and the EH epilogue.
// Helper texts (Ego_d): GetMagnitude line 417 is 53 B with no local = the sum (GetSquaredMagnitude's 44 B line) + `push ecx ; fstp ;
//   call GFRoot` (9 B) -> FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF; Normalise has the one local one_on_magnitude and line 687 is 74 B (three
//   GFSqr calls) -> FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER; GFFastOneOverSqrt's locals n, seed (TFloatIntUnion, TPI 0xADDE), r ->
//   FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION; operator^'s parameter is `vector` -> FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22.
// Inferred: the parenthesised cross-product components of FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22 (/Od sizes are equal;
//   the ungrouped CROSS_ORIGINAL_FORMAL text grades 445v445: the Axis copy's integer moves no longer interleave with the next component's
//   loads, 0x00852F7C / 0x00852F98).
// Inline-budget: GFFastOneOverSqrt is FABLE_FORCEINLINE in every branch of engine/C3DVector.h (retail expands it here, 0x00852EBD..
//   0x00852F0D, as in its other callers); no other inline control is named.
#include <math.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/GFAngles.h"
#include "engine/NCameraModes_CScriptModuleFOVZoom.h"
namespace NCameraModes {
CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative(const CIntelligentPointer<CThing> thing, const C3DVector& look_at_pos,
        const C3DVector& start_pos, const C3DVector& end_pos, float fraction_of_fov_to_reach, long world_frames, bool shortenonocclusion)
    : CScriptModuleFOVZoom(look_at_pos, start_pos, end_pos, fraction_of_fov_to_reach, world_frames, shortenonocclusion), Thing(thing)
{
    C3DVector facing = thing->GetPhysicsTC()->GetFacingVector();
    C3DVector rel = end_pos - look_at_pos;
    Mag = rel.GetMagnitude();
    rel.Normalise();
    Angle = GFArcCos(facing.Dot(rel));
    Axis = rel ^ facing;
}
}