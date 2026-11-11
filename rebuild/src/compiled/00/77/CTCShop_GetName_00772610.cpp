// CTCShop::GetName() const -- fetch the parent thing's definition by its index, take its shop sub-definition and return the shop name word at +0x28 (0 when the thing has no shop def)
// Leaf: ?GetName@CTCShop@@QBEKXZ
// Retail: Fable.exe 0x00772610, 151 bytes (plain `ret`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetName@CTCShop@@QBEKXZ (engine/CTCShop.h FABLE_RETAIL_CTCSHOP_NAME: unsigned long).
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef<CShopDef> 0x005130A0; the defs' Release (virtual Delete at zero).
// Layout: CThing::DefGlobalIndex +0x8C; CShopDef::Name +0x28 (engine/CShopDef.h).
// Shape: both defs live in CDefPointer locals whose destructors release them; the found path returns `shopDef->Name` directly,
// so its null test on the sub-def is dropped after the dereference while the not-found path keeps it. GetDef / GetSubDef are the
// headers' noinline template bodies (out of line in retail).
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefClassBase / CTCShop stand-ins onto the shared
// headers: the callees now bind ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (was ?GetDefinitions@CTCShop@@...),
// GetDef<CThingBaseDef> and GetSubDef<CShopDef> (was ?GetSubDef@CDefClassBase@@QAE_NPAV?$CDefPointer@VCShopDef@@@@@Z); the row's own
// symbol is the PDB ?GetName@CTCShop@@QBEKXZ (was ?GetName@CTCShop@@QBEJXZ, long).
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
#define FABLE_RETAIL_CTCSHOP_NAME
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"

unsigned long CTCShop::GetName() const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef))
        return shopDef->Name;
    return 0;
}
