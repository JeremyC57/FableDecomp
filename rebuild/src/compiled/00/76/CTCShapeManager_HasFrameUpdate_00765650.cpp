// CTCShapeManager::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCShapeManager@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00765650, thiscall, `ret`.
// Identity: slot 8 of CTCShapeManager's vtable 0x0126A2A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCShapeManager or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCShapeManager@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCShapeManager.h (opt-ins as in the landed row NCameraModesCTrack_OnInitLeaf_0087b840.cpp).
#define FABLE_RETAIL_CTCSHAPEMANAGER_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_TRACK
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCShapeManager.h"
bool CTCShapeManager::HasFrameUpdate() const
{
    return true;
}
