// CTCInventoryItem::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCInventoryItem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAEE6, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryItem's vtable 0x01240004 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryItem@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryItem.h (opt-ins as in the landed row CTCInventoryWeapons_RemoveWeaponsFromSheathes_005c552c.cpp).
#define FABLE_RETAIL_CTCINVENTORYITEM_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CArray.h"
#include "engine/CTCInventoryItem.h"
bool CTCInventoryItem::IsComponentSerialisable() const
{
    return true;
}
