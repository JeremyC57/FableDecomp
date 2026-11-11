// CTCStealth::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCStealth@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749410, thiscall, `ret`.
// Identity: slot 8 of CTCStealth's vtable 0x012689EC (defined here: the slot holds this body in 1 vtable(s), all of CTCStealth or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCStealth@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCStealth.h (opt-ins as in the landed row CTCStealth_GetPositionToMoveToForStabOn_00746ec0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCStealth.h"
bool CTCStealth::HasFrameUpdate() const
{
    return true;
}
