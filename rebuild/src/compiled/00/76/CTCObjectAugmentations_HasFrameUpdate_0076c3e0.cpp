// CTCObjectAugmentations::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCObjectAugmentations@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076C3E0, thiscall, `ret`.
// Identity: slot 8 of CTCObjectAugmentations's vtable 0x0126A3A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCObjectAugmentations or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCObjectAugmentations@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCObjectAugmentations.h (opt-ins as in the landed row CTCObjectAugmentations_AttachAugmentationParticles_007677e0.cpp).
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#include "engine/CThing.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CMainGameComponent.h"
bool CTCObjectAugmentations::HasFrameUpdate() const
{
    return true;
}
