// CTCHeroReceiveItems::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroReceiveItems@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CBD70, thiscall, `ret`.
// Identity: slot 23 of CTCHeroReceiveItems's vtable 0x0126EBFC (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroReceiveItems or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroReceiveItems@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroReceiveItems.h (opt-ins as in the landed row CTCHeroReceiveItems_OnSerialise_007cb8b0.cpp).
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#include "fable_string.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"
bool CTCHeroReceiveItems::IsComponentSerialisable() const
{
    return true;
}
