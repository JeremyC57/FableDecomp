// 00761980 CTCShapeManager::DrawShapes -- editor draw of the manager's shapes: every persistent shape in blue plus a red line from its start point (lifted 0.15) to the owner thing; then the selected shape in sand with its points highlighted white.
// Leaf: ?DrawShapes@CTCShapeManager@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00761980, 389 bytes, `ret 8`. Ego_r.pdb 0x00772C30 + FableWin.pdb (private, const references).
// Flags: none
// Callees: CTCBase::PeekWorld 0x00686A80 and CWorld::PeekWorldMap (vslot 13, +0x34); NShape::CShape::EditorDrawPreparePrimitivesForRendering
//          (vslot 3, +0x0c); the inline NShape::CShape::GetStartPoint (FableWin.pdb name, expanded: `cmp begin,end ; je` + the 12-byte
//          block copy); the inline CTCBase::GetParentThing (FableWin.pdb, `mov ecx,[edi+4]`); CThing::GetPos 0x004C73D0;
//          CEnginePrimitiveLine ctor 0x004C3090 (inline in the header, called out of line here: its real body is visible through
//          FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE); CIEngine::AddPrimitive (vslot 20, +0x50);
//          NShape::CShape::EditorDrawHighlightPoints 0x007617A0.
// Shape: the two colours are built first (B,G,R,A bytes; 0xff lives in bl), the world map reference is kept in a frame slot, the
//        shape iterator lives in a frame slot too (ebx holds the constant, ebp `info`, esi the shape, edi this). The start point comes
//        through GetStartPoint(C3DVector&) -- the same inline accessor reproduces sibling GetPrincipalVector 0x00760910 byte-exact.
//        The owner thing is named in a reference BEFORE the lifted vector is built: retail's scratch registers go round
//        eax (source pointer), ecx, edx, eax for the block copy, then ecx for the parent (0x00761A39 `mov ecx,[edi+4]`), then edx
//        for start.X and eax for start.Y (0x00761A31 / 0x00761A21, both reloaded from the frame because the parent's ecx and X's
//        edx overwrote the registers that still held them), then edx again for info.Engine. With the parent read at the call
//        instead (`ParentThing->GetPos()` in the argument list) VC7.1 forwards the copied X and Y in registers and the function is
//        8 bytes short (381 v 389).
// Inferred: the statement `CThing& parent = GetParentThing();` and its place before `top`. The bytes prove only that the parent
//        pointer was evaluated between the start-point copy and the X / Y reads of the lifted vector; a reference local is the
//        plain spelling of that order (`CThing* parent = ParentThing;` and `CThing& parent = *ParentThing;` give the same bytes).
//        Behaviour is identical to reading the parent in the argument list.
//        The red line colour is built in the dead `info` argument slot and the white highlight colour in the dead `viewPos` slot.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES
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
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CRGBColour.h"
void CTCShapeManager::DrawShapes(const CEnginePrimitivePrepareInfo& info, const C3DVector& viewPos)
{
    CRGBColour shapeColour(0x50, 0x50, 0xff, 0xff);
    CRGBColour selectedColour(0xb4, 0xa0, 0x50, 0xff);
    const CWorldMap& map = PeekWorld().PeekWorldMap();
    for (std::vector<CCountedPointer<NShape::CShape> >::iterator it = PersistantShapes.begin(); it != PersistantShapes.end(); ++it) {
        NShape::CShape* shape = it->Get();
        shape->EditorDrawPreparePrimitivesForRendering(info, map, viewPos, shapeColour);
        C3DVector start;
        if (shape->GetStartPoint(start)) {
            CThing& parent = GetParentThing();
            C3DVector top(start.X, start.Y, start.Z + 0.15f);
            info.Engine->AddPrimitive(CEnginePrimitiveLine(0, top, parent.GetPos(), CRGBColour(0xff, 0, 0, 0xff), 0.05f,
                                                           (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        }
    }
    if (SelectedShape != PersistantShapes.end()) {
        NShape::CShape* shape = SelectedShape->Get();
        shape->EditorDrawPreparePrimitivesForRendering(info, map, viewPos, selectedColour);
        shape->EditorDrawHighlightPoints(info, map, viewPos, CRGBColour(0xff, 0xff, 0xff, 0xff));
    }
}