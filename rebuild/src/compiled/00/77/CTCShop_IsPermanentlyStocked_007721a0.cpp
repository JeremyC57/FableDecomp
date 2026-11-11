// CTCShop::IsPermanentlyStocked() const -- true when the shop thing's CShopDef sub-def says the stock display is permanent or the shop is always open
// Leaf: ?IsPermanentlyStocked@CTCShop@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007721A0, 170 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef<CShopDef> 0x005130A0; the defs' Release (virtual Delete on the last reference).
// Layout: CThing::DefGlobalIndex (unsigned short) +0x8c; CShopDef::IsStockDisplayPermanent +0x98, AlwaysOpen +0x9c (engine/CShopDef.h).
// Shape: two CDefPointer locals whose destructors release them; GetDef / GetSubDef are the headers' noinline template bodies
// (called out of line in retail), so each def is read once into a register.
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefClassBase / CTCShop stand-ins onto the shared
// headers, so the callees bind the PDB spellings ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (was
// ?GetDefinitionManager@CTCShop@@QBEPAVCDefinitionManager@@XZ), GetDef<CThingBaseDef> (was GetDef_0042af3c) and GetSubDef<CShopDef>.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCSHOP
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"

bool CTCShop::IsPermanentlyStocked() const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef))
        return shopDef->IsStockDisplayPermanent || shopDef->AlwaysOpen;
    return false;
}
