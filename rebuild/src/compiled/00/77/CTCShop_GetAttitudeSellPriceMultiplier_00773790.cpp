// CTCShop::GetAttitudeSellPriceMultiplier() const -- 1 for a house the player owns; else the shop def's attitude sell multiplier for the keeper's attitude to the player (default when no entry), eased toward 1 by the hero's trade skill
// Leaf: ?GetAttitudeSellPriceMultiplier@CTCShop@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00773790, 448 bytes, __fastcall, plain `ret`, float result in st0.
// Identity: Ego_r.pdb 0x00780980 / FableWin.pdb ?GetAttitudeSellPriceMultiplier@CTCShop@@QBEMXZ (engine/CTCShop.h
// FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE; the landed caller CTCShop::GetCurrentSellPrice 0x007743B0 binds it at 0x00774430). The body
// reads CShopDef::AttitudeSellPriceMultiplierDefault (+0x70) and the AttitudeSellPriceMultiplier map (+0x74); 0x007735D0 is the Buy twin.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (house, inline PeekTC) and non-const 0x0040F020 (hero,
// inline GetTC), CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3, inline CIntelligentPointer::Get),
// CTCBase::PeekWorld 0x00686A80 (+ inline CWorld::PeekOpinionReactionManager, +0x74),
// COpinionReactionManager::GetThingAttitudeToPlayer(const CThing&) const 0x00701840, std::map<EOpinionAttitudeType,float>::find
// 0x00701FC0 (hidden iterator result), CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer const 0x00449960,
// CPlayer::GetPControlledCreature 0x00487DC0, CTCHeroStats::GetTradeSkill 0x0057C239; 1.0f 0x0122DED8, the trade scale 0x01382998;
// the defs' Release (slot 1 Delete).
// Shape: the house test is HasTC(TCI_BUYABLE_HOUSE 0xD5) + the expanded const lookup + the inline GetOwnedByPlayer (+0x11); the
// thing def comes by DefGlobalIndex (+0x8C) and is released last, the shop def (found path: no null test) first; the multiplier
// lives in the shop def's dead slot; the keeper test is HasTC(TCI_CREATURE_OPINION_OF_HERO 0x4F) on a second GetPItem; the hero
// part is m = (1 - m) * (skill * scale) + m with the skill left on the FPU stack (`fstp st(0)`).
// Statement shape that decides the callee-saved allocation (attempt 2): inside the keeper block the reaction manager and the keeper are
// two named locals, `const COpinionReactionManager& reactions = PeekWorld().PeekOpinionReactionManager();` then
// `const CThingCreatureBase* keeper = PShopKeeper.Get();`, and only then the GetThingAttitudeToPlayer call. Written as one
// expression the bytes are the same instructions with the callee-saved registers rotated (this=edi, ParentThing=ebp, zero=esi,
// &PShopKeeper=esi, thing def=ebp, 430 bytes); with the two declarations VC7.1 gives the reaction manager pointer esi ahead of
// &PShopKeeper, which then takes edi after ParentThing, `this` falls to ebp, the thing def shares esi with the map / reaction
// temporaries (reloaded from its slot after find and after GetTradeSkill) and the zero constant sits in ebx, pushed at 0x007737EA.
// Headers (overlay $W/hdr): engine/CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_TRADESKILL (declares GetTradeSkill).
// Name note: g_TradeSkillAttitudeScale (0x01382998) is a descriptive name; the original is in no PDB / string source searched.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_TRADESKILL
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroStats.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"

extern const float g_TradeSkillAttitudeScale;             // 0x01382998

float CTCShop::GetAttitudeSellPriceMultiplier() const
{
    const CTCBuyableHouse* house;
    if (GetParentThing().PeekTC(TCI_BUYABLE_HOUSE, &house)) {
        if (house->GetOwnedByPlayer())
            return 1.0f;
    }
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return 1.0f;
    float multiplier = shopDef->AttitudeSellPriceMultiplierDefault;
    if (PShopKeeper.Get() && PShopKeeper.Get()->HasTC(TCI_CREATURE_OPINION_OF_HERO)) {
        const COpinionReactionManager& reactions = PeekWorld().PeekOpinionReactionManager();
        const CThingCreatureBase* keeper = PShopKeeper.Get();
        EOpinionAttitudeType attitude = reactions.GetThingAttitudeToPlayer(*keeper);
        std::map<EOpinionAttitudeType, float>::const_iterator it = shopDef->AttitudeSellPriceMultiplier.find(attitude);
        if (it != shopDef->AttitudeSellPriceMultiplier.end())
            multiplier = it->second;
    }
    CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero) {
        CTCHeroStats* stats;
        if (hero->GetTC(TCI_HERO_STATS, &stats)) {
            float skill = stats->GetTradeSkill();
            multiplier += (1.0f - multiplier) * (skill * g_TradeSkillAttitudeScale);
        }
    }
    return multiplier;
}