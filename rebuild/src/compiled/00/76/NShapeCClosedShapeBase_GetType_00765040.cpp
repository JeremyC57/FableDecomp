// NShape::CClosedShapeBase::GetType() const -- returns (EShape)0
// Leaf: ?GetType@CClosedShapeBase@NShape@@UBE?AW4EShape@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00765040, thiscall, `ret`.
// Identity: slot 1 of NShape::CBasicShape's vtable 0x0126A31C (defined here: the slot holds this body in 3 vtable(s), all of NShape::CClosedShapeBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 3 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CClosedShapeBase@NShape@@UBE?AW4EShape@2@XZ; `ret` matches the signature.
// Header: engine/NShape_CClosedShapeBase.h (opt-ins as in the landed row NShapeCClosedShapeBase_EditorDrawPreparePrimitivesForRendering_00761560.cpp).
#define FABLE_RETAIL_CIENGINE_SCREENPOS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_DECLARATION
#include "engine/NShape_CClosedShapeBase.h"
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CWorldMap.h"
NShape::EShape NShape::CClosedShapeBase::GetType() const
{
    return (EShape)0;
}
