// CTCShop::GetCurrentBuyPrice(long itemDefIndex, long count) const -- the whole-gold price the shop asks for count items: the basic price scaled by the shop def's buy multiplier and the attitude multiplier, truncated per item
// Leaf: ?GetCurrentBuyPrice@CTCShop@@QBEJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007742C0, 231 bytes (`ret 8`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetCurrentBuyPrice@CTCShop@@QBEJJJ@Z (const; the landed caller 0x005A3436 binds that spelling).
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CTCShop::GetAttitudeBuyPriceMultiplier const 0x007735D0,
// CTCShop::GetCurrentBasicPrice(long, long) const 0x007727E0 (float), __ftol2 0x00BFEA70; the defs' Release.
// Layout: CThing::DefGlobalIndex (unsigned short) +0x8c; CShopDef::BuyPriceMultiplier +0x58 (engine/CShopDef.h).
// Shape: both defs are dtor-carrying CDefPointer locals; GetDef and GetSubDef are the headers' noinline template bodies, so the
// thing def is read once into ebp and the shop def is re-read once on the found path (its release there has no null test); the
// price total is declared just before the loop, so its zero is the out slots' zero register (edi) and `push ebx` sinks into the
// found branch; the multiplier is a named float local.
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefClassBase / CTCShop stand-ins onto the shared
// headers: GetDefinitionManager / GetDef<CThingBaseDef> / GetSubDef<CShopDef> bind the PDB spellings (were
// ?GetDefinitionManager@CTCShop@@QBEPAVCDefinitionManager@@XZ, GetDef_0042af3c, GetSubDef_005130a0), GetCurrentBasicPrice binds the
// PDB const ?GetCurrentBasicPrice@CTCShop@@QBEMJJ@Z (was QAEMJJ) and the row's own symbol is the PDB const QBEJJJ (was QAEJJJ).
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
#define FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"

long CTCShop::GetCurrentBuyPrice(long itemDefIndex, long count) const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return 0;
    float multiplier = shopDef->BuyPriceMultiplier;
    multiplier = multiplier * GetAttitudeBuyPriceMultiplier();
    long total = 0;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}
