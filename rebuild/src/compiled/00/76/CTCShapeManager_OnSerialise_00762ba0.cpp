// 00762ba0 CTCShapeManager::OnSerialise(CPersistContext& context, bool) -- persist the shape manager: the "IsCoordsRelativeToMap" flag (default false, preset true), and when it is set the map position of the map the owner thing stands on (the offset every shape point is stored relative to); then "NumShapes"; on load reset ClosedShapeBounds to the empty box (+1e10 / -1e10) and load each shape through NShape::CShape::StatOnSerialise, on save serialise every persistent shape with its index; finally reset SelectedShape to end().
// Leaf: ?OnSerialise@CTCShapeManager@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00762BA0, 468 bytes, thiscall `ret 8`, `and esp,-8` frame; CTCShapeManager vtable 0x0126A2A4 slot 1.
//   Ego_r.pdb 0x00773C30 ?OnSerialise@CTCShapeManager@@UAEXAAVCPersistContext@@_N@Z (public virtual) + the CTCBase slot 1 signature.
// Flags: none
// Callees: CPersistContext::Transfer<bool> 0x004045C0 and Transfer<long> 0x00410BE0 (through the two-argument member template that
//          forwards GetDefaultVal<T>(): the default temporaries are stored after the pushes); the inline CPersistContext::IsSaving
//          (Mode 1 or 3, read three times); CThing::GetPos 0x004C73D0; CTCBase::PeekWorldMap 0x00686D20 (called twice);
//          CWorldMap::GetMapNumberAt(const C3DVector&) (vslot 16, +0x40) and CWorldMap::GetMapPos(long) (vslot 23, +0x5c, C2DCoordI by
//          hidden pointer); NShape::CShape::StatOnSerialise 0x00762620 (static __fastcall, same TU, out of line);
//          NShape::CShape::OnSerialise (vslot 2, +0x08); the inline CCountedPointer copy and destructor (DeleteFunc call + operator
//          delete 0x00BFE9BC).
// Shape: both arms of the first IsSaving test are the same Transfer call (retail keeps two push sequences that differ only in the
//        scratch registers); the offset is a C2DCoordI (integer zero stores through edi); the load arm comes first (`if (!IsSaving())`)
//        and the bounds are stored BRX, BRY (shared eax) then TLX, TLY; the single `SelectedShape = end()` statement is duplicated
//        by the compiler into the four exits (load loop end, save loop end, empty save, zero-count load).
// Inferred: the duplicated first branch is written as an if / else with the same statement in both arms (the bytes prove two
//        separate call set-ups; what distinguished them in the original source, if anything, left no bytes).
// Headers: FABLE_RETAIL_CTCSHAPEMANAGER_ONSERIALISE (engine/CTCShapeManager.h) and FABLE_RETAIL_NSHAPE_CSHAPE_STATONSERIALISE
//        (engine/NShape_CClosedShapeBase.h) are new opt-ins from this row's overlay.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES
#define FABLE_RETAIL_CTCSHAPEMANAGER_ONSERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_NSHAPE_CSHAPE_STATONSERIALISE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CPersistContext.h"
#include "engine/CWorldMap.h"
#include "engine/CTCShapeManager.h"
#include "engine/NShape_CClosedShapeBase.h"
template<> inline bool FABLE_FASTCALL CPersistContext::GetDefaultVal<bool>() { return false; }
template<> inline long FABLE_FASTCALL CPersistContext::GetDefaultVal<long>() { return 0; }
void CTCShapeManager::OnSerialise(CPersistContext& context, bool)
{
    
    bool isCoordsRelativeToMap = true;
    if (context.IsSaving())
        context.Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap);
    else
        context.Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap);
    C2DCoordI offset(0, 0);
    if (isCoordsRelativeToMap) {
        C3DVector pos = ParentThing->GetPos();
        offset = PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos));
    }
    long numShapes;
    if (context.IsSaving())
        numShapes = (long)PersistantShapes.size();
    context.Transfer("NumShapes", numShapes);
    if (!context.IsSaving()) {ClosedShapeBounds.BRX = -1e10f; ClosedShapeBounds.BRY = -1e10f; ClosedShapeBounds.TLX = 1e10f; ClosedShapeBounds.TLY = 1e10f;
        for (long i = 0; i != numShapes; ++i)
            NShape::CShape::StatOnSerialise(*this, context, i, offset, ClosedShapeBounds);
    } else {
        long index = 0;
        for (std::vector<CCountedPointer<NShape::CShape> >::iterator it = PersistantShapes.begin(); it != PersistantShapes.end(); ++it) {
            CCountedPointer<NShape::CShape> shape = *it;
            shape->OnSerialise(context, index, offset);
            ++index;
        }
    }
    SelectedShape = PersistantShapes.end();
}