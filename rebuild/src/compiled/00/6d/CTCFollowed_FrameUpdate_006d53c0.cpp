// CTCFollowed::FrameUpdate() -- every 10th frame (staggered by UID) culls dead followers, drops the ones no longer willing to follow (posting deed 0x31 for each), resets the idle timer when the hero moves more than 3 units, otherwise after 5 s idle posts deed 0x34 for a random follower at most every 10 s
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D53C0, 800 bytes, `ret` (aligned ebp frame from the __int64 UID math).
// Callees: CTCBase::GetWorld @ 0x00686A70 (result unused: CWorld::GetFrame @ 0x0049D870 is static), __aullrem @ 0x00BFEBB0,
//          CullNullFollowers @ 0x006D5260, LowerBound @ 0x0040F020 / 0x004365B0 (two CVectorMap instances),
//          CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero @ 0x006818F0, std::list<CIntelligentPointer<CThing> >::remove @ 0x0065DE8A,
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10, notify @ 0x004378C0 on the global
//          at 0x013B8790, TC 0x7A AddDependentThing-style call @ 0x006AEBA0, CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990,
//          OnFollowerLeft @ 0x006D5050, CThing::GetPos @ 0x004C73D0, GetRandom @ 0x00686DA0, list size @ 0x0065F17B,
//          GFRandom @ 0x004CDAE0 (__fastcall range, rng), free @ 0x00BFEA14. Frames-per-second global 0x01375550.
// Layout: CTCFollowed Followers (STLport list) +0x0C, AIFollowers (STLport list, node pointer only) +0x10, FrameLastIdleReaction +0x14,
//         FramesSincePlayerMoved +0x18, LastPlayerPosition +0x1C; CThing UID +0x18, TC present bits +0x20, TC map +0x44.
// Shape: IsTimeToUpdate loads GetWorld() into a named local and the UID before the static GetFrame call; a reference local to
//        AIFollowers._M_node gives retail's cached &head (edi) in the culling loop, while the random-pick loop uses begin()/end()
// Leaf: ?FrameUpdate@CTCFollowed@@UAEXXZ
// NF76 port (2026-11-17): every local model of a class with a shared header deleted; the row now compiles on engine/CTCFollowed.h,
//   CTCBase.h (ParentThing, GetWorld, WORLDSEED GetPWorldSeed), CThing.h (GETTC_EXPANDED GetTC / PeekTC), CThingAICreature.h,
//   CIntelligentPointer.h, CTCHeroOpinionDeedLog.h, CTCCreatureOpinionOfHero.h, CTCRegionFollower.h, CPlayerGui.h (singleton Get),
//   CMainGameComponent.h, CWorld.h, GFRandom.h and stlport/fable_containers.h. Bindings now under their PDB spellings:
//   AIFollowers is the PDB list<CIntelligentPointer<CThingAICreature> > (size 0x0065F17B out of line: overlay STLP_LIST_SIZE_DECL,
//   queue entry nf41:e53); the TC 0x7A component is CTCRegionFollower (Ego_d.pdb local ptc_region_follower) and 0x006AEBA0 is its
//   RemoveFollower(CThing&) (Ego_r.pdb); the made-up OnFollowerLeft is ?RemoveEmoteIcon@CTCFollowed@@IAEXAAVCThing@@@Z (0x006D5050);
//   GetRandom is ?GetPWorldSeed@CTCBase@@IBEPAKXZ (0x00686DA0); GFRandom is ?GFRandom@@YIKKPAK@Z (Ego_r.pdb; overlay
//   GFRANDOM_OUTOFLINE keeps the call). The self is virtual: Ego_r.pdb 0x0070A220 ?FrameUpdate@CTCFollowed@@UAEXXZ.
//   Locals as Ego_d.pdb names them (ptc_deedlog, it, pcreature, ptc_opinion, ptc_region_follower, follower, follower_chosen).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCREGIONFOLLOWER_REGIONLOAD
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DECL
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#include "stlport/fable_containers.h"
#include "engine/GFRandom.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCFollowed.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCRegionFollower.h"

void CTCFollowed::FrameUpdate()
{
    ++FramesSincePlayerMoved;
    if ((GetWorld().GetFrame() + ParentThing->GetUniqueID()) % 10 == 0) {
        CullNullFollowers();
        if (!AIFollowers.empty()) {
            CTCHeroOpinionDeedLog* ptc_deedlog;
            ParentThing->GetTC(&ptc_deedlog);
            {
                std::list<CIntelligentPointer<CThingAICreature> >::iterator it = AIFollowers.begin();
                while (it != AIFollowers.end()) {
                    CThingAICreature* pcreature = (*it).Get();
                    const CTCCreatureOpinionOfHero* ptc_opinion;
                    if (pcreature->PeekTC(&ptc_opinion) && ptc_opinion->IsWillingToContinueFollowingHero()) {
                        ++it;
                    } else {
                        it = AIFollowers.erase(it);
                        Followers.remove(CIntelligentPointer<CThing>(pcreature));
                        CPlayerGui::Get().UpdateMiniMapMarkers(false);
                        {
                            CTCRegionFollower* ptc_region_follower;
                            ParentThing->GetTC(&ptc_region_follower);
                            ptc_region_follower->RemoveFollower(*pcreature);
                        }
                        ptc_deedlog->PostDeed(OPINION_DEED_TYPE_FOLLOWER_QUIT, ParentThing->GetPos(), pcreature, pcreature,
                                              (EOpinionPostDeedType)1, false, false, 0, false, 0);
                        RemoveEmoteIcon(*pcreature);
                    }
                }
            }
            const C3DVector& pos = ParentThing->GetPos();
            float dx = pos.X - LastPlayerPosition.X;
            float dy = pos.Y - LastPlayerPosition.Y;
            if (dx * dx + dy * dy > 9.0f) {
                LastPlayerPosition = ParentThing->GetPos();
                FramesSincePlayerMoved = 0;
            } else if (FramesSincePlayerMoved > CMainGameComponent::GetConstantFPS() * 5.0f) {
                if (GetWorld().GetFrame() - FrameLastIdleReaction > CMainGameComponent::GetConstantFPS() * 10.0f) {
                    long follower = GFRandom(AIFollowers.size(), GetPWorldSeed());
                    long follower_chosen = 0;
                    for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = AIFollowers.begin(); it != AIFollowers.end(); ++it) {
                        if (follower_chosen == follower) {
                            ptc_deedlog->PostDeed(OPINION_DEED_TYPE_FOLLOWER_IDLEBORED, GetParentThing().GetPos(), (*it).Get(), (*it).Get(),
                                                  (EOpinionPostDeedType)1, false, false, 0, false, 0);
                            FrameLastIdleReaction = GetWorld().GetFrame();
                            break;
                        }
                        ++follower_chosen;
                    }
                }
            }
        }
    }
    if ((GetWorld().GetFrame() + ParentThing->GetUniqueID()) % 10 == 0)
        CullNullFollowers();
}
