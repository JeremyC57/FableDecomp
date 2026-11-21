// CThingPhysicalSwitch::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- after the physical base, submit the current graphic as a static mesh at the interpolated position and orientation, scaled by the thing's Scale
// Leaf: ?DrawPreparePrimitivesForRendering@CThingPhysicalSwitch@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083DC80, 309 bytes (+ int3 padding), __fastcall `ret 4`, vtable slot 36. Grade: RELOCATION_MATCH (309/309). No flag.
// Identity: public virtual (UAE) in Ego_r 0x00812D10 / FableWin 0x028013F0; the CThingStaticMeshPrimitive_* label is a placeholder (NAME_FIXES.tsv). The three family
//   rows 0x0083B6B0 / 0x0083DC80 / 0x00837B30 are instruction-identical except for the graphic accessor. Ego_d.pdb: one local
//   `def_graphic` (const CEngineGraphic*), eight line records (36/26/14/186/280/4/2/16): the if, def_graphic, ASSERT(def_graphic) (186 B,
//   the debug assert), the AddPrimitive statement on ONE line (280 B), return true, return false.
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering 0x006A64D0; CThingPhysicalSwitch::GetCurrentGraphic() 0x0083DB60 (`?GetCurrentGraphic@CThingPhysicalSwitch@@IBEABVCEngineGraphic@@XZ`, returns a reference; Ego_d types def_graphic as a pointer, hence `&`);
//   the physics TC's virtual GetRHSet(float) (+0x11C, CRightHandedSet by hidden pointer); CEnginePrimitiveStaticMesh ctor 0x0077BD70
//   (out of line); the engine's AddPrimitive (+0x50); the temporary's MeshEffect release (CIVCountedPointer, Destroy at +4).
// Shape:
//   - The CEnginePrimitiveStaticMesh ctor body must be VISIBLE (header opt-in FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR): it is an
//     inline header ctor in the original, emitted as a COMDAT in FableWin tc_graphic_appearance_static_mesh.obj #10 (0x0263C1E0, 217 B debug)
//     and placed by retail at 0x0077BD70 with that TU. Still called out of line, but with the body in view c2 shares the diff / result
//     temps of the interpolated position and the uid copy (frame 0x80); declaration-only gives frame 0x8C and DIFFER(308v309).
//   - GetPosition(t) is the CTCPhysicsBase header body (O + (P - O) * t, expanded); the RH set is the virtual call.
//   - Inferred: GetUniqueID() and GetPhysicsTC() as accessors (retail keeps the uid low in ebx and loads PhysicsTC early, as the accessors
//     schedule); `Scale` read directly (GetScale() loads it before the ctor arguments, retail loads it late).
//   - The tint is a by-value CRGBColour(0xff,0xff,0xff,0xff) built in its argument slot; the theme channel a default temporary.
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_GETCURRENTGRAPHIC
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_DRAWPREPARE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#include "fable_assert.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CThing.h"
#include "engine/CEngineGraphic.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitiveStaticMesh.h"

bool CThingPhysicalSwitch::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info))
    {
        const CEngineGraphic* def_graphic = &GetCurrentGraphic();
        FABLE_RETAIL_ASSERT(def_graphic);
        info.Engine.AddPrimitive(CEnginePrimitiveStaticMesh(GetUniqueID(), def_graphic->BankIndex, GetPhysicsTC()->GetPosition(info.Interpolate.Current.WFInterpolate), GetPhysicsTC()->GetRHSet(info.Interpolate.Current.WFInterpolate), def_graphic->RenderSizeX * Scale, true, 0xff, CRGBColour(0xff, 0xff, 0xff, 0xff), 0.0f, (EEngineInterpolationSet)0, CEnginePrimitiveThemeChannelDesc(), true));
        return true;
    }
    return false;
}