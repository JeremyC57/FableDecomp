// CTCSummonSpell::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSummonSpell@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079BD00, thiscall, `ret`.
// Identity: slot 8 of CTCSummonSpell's vtable 0x0126C2B4 (defined here: the slot holds this body in 1 vtable(s), all of CTCSummonSpell or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSummonSpell@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCSummonSpell.h (opt-ins as in the landed row CTCSummonSpell_GetValidTeleportPos_0079aa90.cpp).
#define FABLE_RETAIL_CTCSUMMONSPELL_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CTCSummonSpell.h"
bool CTCSummonSpell::HasFrameUpdate() const
{
    return true;
}
