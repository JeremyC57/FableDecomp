// CTCShapeManager::GetPrincipalVector(C3DVector& out) const -- when a shape is selected and has a start point, out = the owner thing's position minus that start point; returns whether it was set
// Leaf: ?GetPrincipalVector@CTCShapeManager@@QBE_NAAVC3DVector@@@Z
// Retail: Fable.exe 0x00760910, 121 bytes (`ret 4`). Ego_r.pdb 0x00772130 + FableWin.pdb.
// Flags: none
// Callees: CThing::GetPos() const @ 0x004C73D0; the inline NShape::CShape::GetStartPoint (FableWin.pdb, expanded: `cmp begin,end ; je`
//          + the 12-byte block copy); C3DVector::operator-(const C3DVector&) const (engine/C3DVector.h, inline).
// Name-fix pass (2026-11-12): rebuilt on the shared headers (engine/CTCShapeManager.h GETPRINCIPALVECTOR + DRAWSHAPES opt-ins,
//          engine/NShape_CClosedShapeBase.h GETSTARTPOINT) in place of the local C3DVector with a by-value operator- (no such PDB
//          member: ??GC3DVector takes const C3DVector&) and the local CTCShapeManager / NShape::CShape models; the same inline
//          accessor reproduces sibling DrawShapes 0x00761980.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES
#define FABLE_RETAIL_CTCSHAPEMANAGER_GETPRINCIPALVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#define FABLE_RETAIL_NSHAPE_CSHAPE_EDITORDRAWHIGHLIGHTPOINTS
#define FABLE_RETAIL_NSHAPE_CSHAPE_GETSTARTPOINT
#include "engine/CTCShapeManager.h"
#include "engine/NShape_CClosedShapeBase.h"
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CRGBColour.h"
bool CTCShapeManager::GetPrincipalVector(C3DVector& out) const
{
    if (SelectedShape != PersistantShapes.end()) {
        C3DVector start;
        if ((*SelectedShape)->GetStartPoint(start)) {
            out = ParentThing->GetPos() - start;
            return true;
        }
    }
    return false;
}
