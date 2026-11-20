// CTCWife::Died() -- a married wife's death unmarries her and sets the living hero's marriage state to 6 (widowed)
// Leaf: ?Died@CTCWife@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0020, 120 bytes (+8 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState) 0x0057BC6E. Global CMainGameComponent::Get() 0x013B86A0.
// Shape: same guard as OnGreet; the player manager is read from the main game component's +0x1C (not through CTCBase);
//        interface 4 is CTCHeroStats (bitset byte +0x20 bit 4), called through the out-parameter lookup form.
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; CMainGameComponent PlayerManager +0x1C.
// NF113 (PRIORITY q4716 / q4950, binder of 0x0057BC6E): ported onto the shared headers with the landed sibling CTCWife::Murdered
//   0x006D00A0 recipe (same body minus the deed post): CTCHeroStats::SetMarriageState is the private Ego_r.pdb member
//   ?SetMarriageState@CTCHeroStats@@AAEXPBVCThingAICreature@@W4EMarriageState@@@Z (CTCWife is its friend; engine/CTCHeroStats.h
//   FABLE_RETAIL_CTCHEROSTATS_MARRIAGE); was a row-local CTCHeroStats::SetMarriageState(CThing*, long) and local CTCWife / CMainGameComponent
//   / CWorld / CThingPlayerCreature models. State 6 = the wife died (no enumerator name in any name source; passed as its value).
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
enum EMarriageState { };

void CTCWife::Died()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    Unmarry(true);
    CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CTCHeroStats* stats;
    if (hero->GetTC(&stats))
        stats->SetMarriageState(static_cast<const CThingAICreature*>(&GetParentThing()), (EMarriageState)6);
}
