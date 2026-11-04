// CTCWife::Murdered() -- a married wife's murder unmarries her, posts the murder-spouse opinion deed at her position to the living hero's deed log and sets his marriage state to wife-murdered
// Leaf: ?Murdered@CTCWife@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D00A0, 217 bytes (+7 int3), plain `ret`. Ego_r.pdb ?Murdered@CTCWife@@QAEXXZ (0x00704C90).
// Callees: CWorld::GetFrame 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; CPlayerManager::GetMainPlayer 0x00449970 on
//   CMainGameComponent::Get()->GetPlayerManager() (inline; PMainGameComponent 0x013B86A0, scoped_ptr +0x1C);
//   CPlayer::GetPControlledCreature const 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside
//   CThing::GetTC<T>); CThing::GetPos const 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed 0x006C6990;
//   CTCHeroStats::SetMarriageState(const CThingAICreature*, EMarriageState) 0x0057BC6E (private in Ego_r.pdb: CTCWife is a friend).
// Shape: the guard of Died / OnGreet; both component lookups are CThing::GetTC<T>(&out) on the retail CThing; the wife's thing is
//   CTCBase::GetParentThing() (inline, read in place), cast to the creature SetMarriageState takes. The parent accessor (not the
//   raw ParentThing member) is what keeps retail's order in the last block: component into ecx, then the parent into eax.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_MARRIAGEEND
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MARRIAGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CTCWife.h"
#include "engine/CTCHeroStats.h"
#include "engine/EOpinionDeedType.h"
enum EMarriageState { MARRIAGE_STATE_WIFE_MURDERED = 7 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_ALL = 7 };
class CFaction;
class CThingAICreature;
class CTCHeroOpinionDeedLog : public CTCBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

void CTCWife::Murdered()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    Unmarry(true);
    CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->GetTC(&deedLog))
        deedLog->PostDeed(OPINION_DEED_TYPE_MURDER_SPOUSE, GetParentThing().GetPos(), 0, 0, OPINION_POST_DEED_TYPE_ALL,
                          false, false, 0, false, 0);
    CTCHeroStats* stats;
    if (hero->GetTC(&stats))
        stats->SetMarriageState(static_cast<const CThingAICreature*>(&GetParentThing()), MARRIAGE_STATE_WIFE_MURDERED);
}