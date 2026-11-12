// 007fd650 CTCDPhysicalObstruction::FrameUpdate -- when the thing manager's combat-collision debug draw is on, show the obstruction as three blue persistent lines (a 2-unit post at each end and the bar joining the post tops); then push creatures off the line (UpdateAdditionalImpulse) and count the life timer down, killing the obstruction thing when it reaches zero.
// Leaf: ?FrameUpdate@CTCDPhysicalObstruction@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x007FD650, 448 bytes, plain `ret`. Ego_r.pdb ?FrameUpdate@CTCDPhysicalObstruction@@UAEXXZ
//         (0x007F8900, right after UpdateAdditionalImpulse as in retail); vtable 0x01241C0C slot 9 (the CTCBase FrameUpdate slot).
// Flags: none
// Callees: CTCBase::GetThingManager 0x00686AE0 (returns a reference; the inline CThingManager::IsDrawCombatCollisionDebug reads
//          the bool at +0x78); the inline chain CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine() (global 0x013B86A0,
//          +0x28, +0x2C), re-read for each line; CEnginePrimitiveLine ctor 0x004C3090 (inline in the header, called out of line
//          here: its real body is visible through FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE);
//          CIEngine::SetPersistentPrimitive (vslot 23, +0x5C; three arguments, the last 0);
//          CTCDPhysicalObstruction::UpdateAdditionalImpulse 0x007FD580; CThing::Kill(bool) 0x004C9B80.
// Constants: 2.0f @ 0x0122DCDC, 0.05f immediate (line width).
// Shape: the lifted points are `Pos + C3DVector(0.0f, 0.0f, 2.0f)` through the inline C3DVector::operator+ -- VC7.1 folds the two
//        `+ 0.0f` (X and Y travel as integer moves, only Z is `fld ; fadd 2.0f`), and because operator+ takes `this` = &StartPos /
//        &EndPos the two addresses live in esi / edi (pushed as the line's start argument, then reused for the third line's
//        component reads `[esi+8]` / `[edi+8]`), which leaves ebp for `this` and ebx for the zero. Spelling the lift as
//        `C3DVector(StartPos.X, StartPos.Y, StartPos.Z + 2.0f)` reads the members off `this` instead and is one byte long with
//        `this` in esi. Every line temporary has its own 0x3C frame slot (sub esp,0xD8).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGMANAGER_ISDRAWCOMBATCOLLISIONDEBUG
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_FRAMEUPDATE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingManager.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE
#include "engine/CEnginePrimitiveLine.h"
void CTCDPhysicalObstruction::FrameUpdate()
{
    if (GetThingManager().IsDrawCombatCollisionDebug()) {
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line1, CEnginePrimitiveLine(0, StartPos, StartPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line2, CEnginePrimitiveLine(0, EndPos, EndPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line3, CEnginePrimitiveLine(0, StartPos + C3DVector(0.0f, 0.0f, 2.0f), EndPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
    }
    UpdateAdditionalImpulse();
    if (LifeTimer > 0) {
        if (--LifeTimer == 0)
            ParentThing->Kill(true);
    }
}