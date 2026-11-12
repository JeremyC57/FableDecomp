// CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long object_def_index) -- the wife is handed a nice gift; says whether her opinion reaction happens. A wedding ring before the marriage is accepted when she is in love with the hero (ReceivedWeddingRing set, deed RECEIVE_WEDDING_RING) and refused otherwise (deed HIGH_PRIORITY_INCOMPREHENSION, false); any other gift is recorded in GiftsReceived with its frame, and the same gift again inside the marriage def's interval gives deed WIFE_GIFT_RECEIVE_ALREADY_GOT and false. The deed is posted to the living hero's opinion deed log at the hero's position with the wife as target and victim.
// Leaf: ?ReceivedNiceGiftIsOpinionReactionHappening@CTCWife@@QAE_NJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0630, 459 bytes (+5 int3), __fastcall, `ret 4`, no frame pointer, no EH frame.
// Identity: Ego_r.pdb 0x00704610 / FableWin.pdb 0x0248D890 ?ReceivedNiceGiftIsOpinionReactionHappening@CTCWife@@QAE_NJ@Z; called by
//   CCreatureAction_GiveItemToThing::PerformAction 0x00844BC0 (0x00844D68) and CCreatureAction_GiveMoneyToThing::PerformAction
//   0x00845040 (0x0084521A).
// Callees: CTCBase::GetWorld 0x00686A70 (then CWorld::PeekDefinitionManager inline, +0x10); CDefinitionManager::GetDef<CThingBaseDef>
//   0x0042AF3C; CDefClassBase::GetSubDef<CGiftDef> 0x006D3EF0; CWorld::GetFrame 0x0049D870 (static, six sites);
//   CTCWife::IsCourtingHero 0x006CF890 (inside the inline IsInLoveWithHero 0x006CF900); CTCBase::PeekWorld 0x00686A80;
//   CWorld::PeekMyPlayer 0x0049D9D0; CPlayer::GetPControlledCreature 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (inside CThing::GetTC<T>); CThing::GetPos 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed 0x006C6990;
//   _Rb_tree<long,pair<const long,long>,..>::find<long> 0x005937E1 (out of line, hidden iterator result) and insert_unique
//   0x004E539F; the defs' virtual slot 1 when a handle's release drops the count to zero. Global: CMainGameComponent::ConstantFPS
//   0x01375550 through the inline GetConstantFPS().
// Shape: two CDefPointer locals (object def, then gift def; released in reverse order in the one epilogue); the wedding-ring test is
//   a named bool (retail materialises it: `mov al,1 / xor al,al ; test al,al`), `result = true` follows it. The deed type is a local
//   that starts as OPINION_DEED_TYPE_NONE and the post is one block behind `if (deed != NONE)`; VC7.1 threads every path past
//   that test. The frame of the repeat check is a local (`call GetFrame` before the def's interval is read). The key fact for the
//   block layout: FrameLastReceivedNiceGift is stored by ONE statement after the insert / update if-else, guarded by the result
//   (the compiler duplicates it into the two success paths, 0x006D07B0 and 0x006D07E1). With that statement between the
//   already-got arm and the post, the post block has no fall-through predecessor, so VC7.1 lays it out straight after the
//   refused-ring arm (0x006D06B3) and puts the accepted-ring arm (0x006D0765) and the whole gift-map arm (0x006D0776) behind the
//   epilogue, jumping back; a store written in each success path keeps the post at the end instead (447 v 459 bytes).
// Callee bodies the TU sees: IsInLoveWithHero (CTCWife 0x006CF900, earlier in the same retail source file, expanded here) is
//   defined inline below with its real landed body; GetDef<T> (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) and
//   GetSubDef<T> (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) are visible but called, which keeps the handles' releases in the
//   `dec dword ptr [ecx+4]` form; the tree's member template find<long> is visible but called
//   (no map-header opt-in is needed for that: the row grades the same with the repo header; checked at landing).
// Enumerators: OPINION_ATTITUDE_LOVE = 12 as the landed CTCWife rows spell it; the post type 1 is passed by number.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
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
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CTCWIFE_NICEGIFT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGiftDef.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CGameDefinitionManager.h"
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
#include "engine/EOpinionDeedType.h"
#include "engine/CTCHeroOpinionDeedLog.h"
enum EOpinionAttitudeType { OPINION_ATTITUDE_LOVE = 12 };
enum EOpinionPostDeedType { };                   // Ego_r.pdb enum (the value used here by number)

// CTCWife::IsInLoveWithHero 0x006CF900 (landed): the real body, inline in this TU as in retail.
inline bool CTCWife::IsInLoveWithHero() const
{
    return IsCourtingHero() && OpinionAttitude == OPINION_ATTITUDE_LOVE;
}

bool CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long object_def_index)
{
    CDefPointer<const CThingBaseDef> object_def;
    CDefPointer<const CGiftDef> gift_def;
    bool is_wedding_ring = GetWorld().PeekDefinitionManager().GetDef(object_def_index, &object_def)
                           && object_def->GetSubDef(&gift_def) && gift_def->IsWeddingRing;
    bool result = true;
    EOpinionDeedType deed = OPINION_DEED_TYPE_NONE;
    if (is_wedding_ring && FrameGotMarriedToThePlayer > CWorld::GetFrame()) {
        if (!IsInLoveWithHero()) {
            deed = OPINION_DEED_TYPE_HIGH_PRIORITY_INCOMPREHENSION;
            result = false;
        } else {
            ReceivedWeddingRing = true;
            deed = OPINION_DEED_TYPE_RECEIVE_WEDDING_RING;
        }
    } else {
        std::map<long,long>::iterator it = GiftsReceived.find(object_def_index);
        if (it == GiftsReceived.end()) {
            GiftsReceived.insert(std::make_pair(object_def_index, CWorld::GetFrame()));
        } else {
            long frame = CWorld::GetFrame();
            if (frame - it->second
                > PHeroMarriageDef->SecondsBetweenReceivingGiftOpinionReactionCanHappen * CMainGameComponent::GetConstantFPS()) {
                it->second = CWorld::GetFrame();
            } else {
                deed = OPINION_DEED_TYPE_WIFE_GIFT_RECEIVE_ALREADY_GOT;
                result = false;
            }
        }
        if (result)
            FrameLastReceivedNiceGift = CWorld::GetFrame();
    }
    if (deed != OPINION_DEED_TYPE_NONE) {
        CThing* hero = PeekWorld().PeekMyPlayer()->GetPControlledCreature();
        if (hero && !hero->IsDead()) {
            CTCHeroOpinionDeedLog* deedLog;
            if (hero->GetTC(&deedLog))
                deedLog->PostDeed(deed, hero->GetPos(), &GetParentThing(), (CThingAICreature*)&GetParentThing(), (EOpinionPostDeedType)1,
                                  false, false, 0, false, 0);
        }
    }
    return result;
}