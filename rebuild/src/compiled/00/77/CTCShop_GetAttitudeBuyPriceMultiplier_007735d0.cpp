// CTCShop::GetAttitudeBuyPriceMultiplier() const -- the factor the shop keeper's opinion of the hero puts on the shop's buy prices:
// 1 for a house the player owns or a thing without a shop def; else the shop def's default attitude multiplier, replaced by the
// def's entry for the keeper's attitude to the player when the keeper has an opinion of the hero, then eased toward 1 by the
// hero's trade skill.
// Leaf: ?GetAttitudeBuyPriceMultiplier@CTCShop@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007735D0, 448 bytes (plain `ret`, float result in st0, no frame pointer).
// Identity: Ego_r.pdb 0x007807A0 + FableWin.pdb ?GetAttitudeBuyPriceMultiplier@CTCShop@@QBEMXZ; the landed caller 0x007742C0
// CTCShop::GetCurrentBuyPrice binds this spelling (engine/CTCShop.h FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE). Sell twin: 0x00773790.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 (the house lookup, inline CThing::PeekTC) and
// non-const 0x0040F020 (the hero-stats lookup, inline CThing::GetTC); CTCBase::GetDefinitionManager 0x006869D0;
// CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (result ignored); CDefClassBase::GetSubDef<CShopDef> 0x005130A0;
// CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times: inline CTCShop::GetShopKeeper); CTCBase::PeekWorld 0x00686A80;
// COpinionReactionManager::GetThingAttitudeToPlayer 0x00701840; std::map<EOpinionAttitudeType,float>::find const 0x00701FC0;
// CTCBase::PeekPlayerManager 0x00686D50; CPlayerManager::GetMainPlayer const 0x00449960; CPlayer::GetPControlledCreature
// 0x00487DC0; CTCHeroStats::GetTradeSkill 0x0057C239; the two defs' releases (`dec [def+4] ; jne ; call [vtbl+4]`).
// Globals: the float at 0x01382998 (.data, 0.84f in the image; only this function and its Sell twin read it) scales the trade
// skill -- the name g_TradeSkillAttitudeScale is DESCRIPTIVE (found in no PDB / string / sibling source); 1.0f pool 0x0122DED8.
// Layout: CThing::UsingComponents +0x20 (bit 213 = `test [thing+38h],200000h`, bit 79 = `test ch,ch` on +0x28, bit 4 = `test byte
// [hero+20h],10h`), Components +0x44, DefGlobalIndex +0x8C; CTCBuyableHouse::OwnedByPlayer +0x11; CShopDef::
// AttitudeBuyPriceMultiplierDefault +0x60, AttitudeBuyPriceMultiplier +0x64 (node value +0x14); CTCShop::PShopKeeper +0x10;
// CWorld::POpinionReactionManager_Data +0x74.
// Shape: the house test returns before the def handles exist (so its epilogue pops no ebx). Both defs are dtor-carrying
// CDefPointer locals and GetDef / GetSubDef are the headers' noinline template bodies (as in GetCurrentBuyPrice): the thing def is
// cached in esi and re-read from its slot after each short-lived esi temp, the shop def is read once into ebx on the found path
// and released there without a null test. What decides the register roles (this = ebp, ParentThing = edi, zero = ebx, thing def =
// esi) is the keeper block: the opinion manager and the keeper are NAMED REFERENCES declared in that order (PeekWorld, then the
// third GetPItem) before the attitude call. With the call written as one expression VC7.1 gives this = edi, ParentThing / thing def
// = ebp, zero = esi (430 bytes, the parked draft's residue). The multiplier is a float local in memory (it shares the shop def's
// dead out slot); `skill * scale` keeps the skill on the x87 stack (`fld scale ; fmul st(1) ... fstp st(0)`).
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPKEEPER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_TRADESKILL
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroStats.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

extern float g_TradeSkillAttitudeScale;             // 0x01382998

float CTCShop::GetAttitudeBuyPriceMultiplier() const
{
    const CTCBuyableHouse* house;
    if (ParentThing->PeekTC(TCI_BUYABLE_HOUSE, &house)) {
        if (house->GetOwnedByPlayer())
            return 1.0f;
    }
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return 1.0f;
    float multiplier = shopDef->AttitudeBuyPriceMultiplierDefault;
    if (GetShopKeeper() && GetShopKeeper()->HasTC(TCI_CREATURE_OPINION_OF_HERO)) {
        const COpinionReactionManager& manager = PeekWorld().PeekOpinionReactionManager();
        const CThing& keeper = *GetShopKeeper();
        EOpinionAttitudeType attitude = manager.GetThingAttitudeToPlayer(keeper);
        std::map<EOpinionAttitudeType, float>::const_iterator it = shopDef->AttitudeBuyPriceMultiplier.find(attitude);
        if (it != shopDef->AttitudeBuyPriceMultiplier.end())
            multiplier = it->second;
    }
    CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats;
    if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
        float skill = stats->GetTradeSkill();
        multiplier += (1.0f - multiplier) * (skill * g_TradeSkillAttitudeScale);
    }
    return multiplier;
}