// CTCShop::GetCurrentSellPrice(long itemDefIndex, long count) const -- the whole-gold price the shop pays for count items: the basic price scaled by the shop def's sell multiplier, the attitude sell multiplier and a constant, truncated per item
// Leaf: ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007743B0, 238 bytes (`ret 8`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z (engine/CTCShop.h FABLE_RETAIL_CTCSHOP_SELLPRICE; the landed
// caller 0x005AF64E binds it). The catalog's GetCurrentBuyPrice label is propagated from the 0x007742C0 twin; this body reads
// SellPriceMultiplier (+0x5c) and the attitude SELL multiplier.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CTCShop::GetAttitudeSellPriceMultiplier const 0x00773790, the float constant
// 0x013833D4, CTCShop::GetCurrentBasicPrice(long, long) const 0x007727E0 (float), __ftol2 0x00BFEA70; the defs' Release.
// Layout: CThing::DefGlobalIndex (unsigned short) +0x8c; CShopDef::SellPriceMultiplier +0x5c (engine/CShopDef.h).
// Shape: as the 0x007742C0 twin: the headers' noinline GetDef / GetSubDef bodies keep the thing def in ebp and re-read the shop
// def once on the found path; the total's zero is the out slots' zero register.
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefClassBase / CTCShop stand-ins onto the shared
// headers: GetDefinitionManager / GetDef<CThingBaseDef> / GetSubDef<CShopDef> bind the PDB spellings (were
// ?GetDefinitionManager@CTCShop@@QBEPAVCDefinitionManager@@XZ, GetDef_0042af3c, GetSubDef_005130a0), GetCurrentBasicPrice binds the
// PDB const QBEMJJ and the row's own symbol is the PDB const QBEJJJ (was QAEJJJ). The 0x013833D4 global keeps the row's
// spelling (its name is unproven; other rows spell it g_TradingPriceMult / g_tradingPriceMult).
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
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"

extern const float g_sellPriceScale_013833d4;           // 0x013833D4

long CTCShop::GetCurrentSellPrice(long itemDefIndex, long count) const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return 0;
    float multiplier = shopDef->SellPriceMultiplier;
    multiplier = multiplier * GetAttitudeSellPriceMultiplier();
    multiplier = g_sellPriceScale_013833d4 * multiplier;
    long total = 0;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}
