// CTCShopKeeper::GetCustomer() const -- the creature the shopkeeper is currently serving
// Leaf: ?GetCustomer@CTCShopKeeper@@QBEPAVCThingCreatureBase@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798D80, 8 bytes: `add ecx,0Ch ; jmp 0x00A01B50` (Customer.GetPItem(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetCustomer@CTCShopKeeper@@QBEPAVCThingCreatureBase@@XZ, the reader of the
//   CIntelligentPointer<CThingCreatureBase> Customer (Ego_r struct_layouts +0x0C, the offset forwarded here); both callers are the
//   shop-stall state group CAIStateGroup_MinionHawkingFromStall::IsRunnable / IsStillValid (0x0094BD2D / 0x0094BE7D). The catalog's
//   "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/CTCShopKeeper.h (new), engine/CThingCreatureBase.h (FABLE_RETAIL_CTHINGCREATUREBASE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCShopKeeper.h"
CThingCreatureBase* CTCShopKeeper::GetCustomer() const
{
    return Customer.Get();
}