// NScript::CQ_RansomVictimScript::DisappearFollowers() -- thread: whenever WitchwoodCavern is loaded before the victim is rescued, take every entity following the hero under scripted control, make them invisible, unpushable, unhittable, undamageable, untargetable, icon-less and hidden on the mini-map; once the region has unloaded restore all of them
// Grade: DIFFER(1913v1428) -- per-exit ~vector<CScriptThing> expansion split (documented blocker family): retail expands the vector destructor at the four depth<=1 terminate exits and routes the six deeper-loop exits through one shared pad that calls 0x00CD319B (~vector<STB>) / 0x008AC970 (~vector<CScriptThing>); ours expands every exit (12 frees). Chunk 844 e1 (new): declared-only `template<> std::vector<CScriptThing>::~vector(); template<> std::vector<STB>::~vector();` turns every exit into a call, DIFFER(1024v1428) md 172 -- fewer diff lines but the four expanded pads are gone; kept here as the parked a2 (473) because its shape is retail's at the shallow exits.
// Next idea: VC7.1's inliner takes loop-nested sites first (chunk 844, 00e482f0: a do{}while(0) around one explicit call site made a plain-inline callee expand there only); implicit scope-exit dtors have no such lever, so this needs a header opt-in that splits the vector dtor by site (FABLE_RETAIL_STLP_VECTOR_DTOR_INLINE-style) or an owner decision on the inline budget.
// Leaf: ?DisappearFollowers@CQ_RansomVictimScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE4710, 1428 bytes, thiscall, plain `ret`, no EH frame.
// Identity (two sources): Ego_r.pdb name set ?DisappearFollowers@CQ_RansomVictimScript@NScript@@QAEXXZ; the landed
//   CQ_RansomVictimScript::Main 0x00DE0850 spawns it as the CSpawnedFunc<CQ_RansomVictimScript> thread named "DisappearFollowers"
//   (Func = 0x00DE4710); engine/CQ_RansomVictimScript.h declares it under FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   CScriptGameResourceObjectScriptedThingBase ctor 0x007E72A0 / dtor 0x007E74D0 (the resize fill temporary, COMDAT calls),
//   IsValid 0x00CD23B9, Release 0x00CD2770; std::vector<CScriptGameResourceObjectScriptedThingBase>::resize 0x00CD3CCB and
//   ~vector 0x00CD319B (catalog placeholder StdList_Dtor_00cd319b: it is the STLport vector dtor, _Destroy range + free),
//   std::vector<CScriptThing>::~vector 0x008AC970, free 0x00BFEA14 (the expanded vector dtors at four exits); Game (+0x40)
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 12 IsRegionLoaded, 70 GetHero, 231 GetFollowingEntityList, 356
//   EntitySetAsHiddenOnMiniMap, 469 EntitySetTargetable, 510 EntitySetAlpha, 515 EntitySetAsRespondToHit, 516 EntitySetAsDamageable,
//   530 EntitySetAsDisplayingEmoteIcon, 745 SetIsThingForcePushable.
// Layout (engine/CQ_RansomVictimScript.h, Ego_r.pdb - 0x14): Game +0x40, RescuedVictim +0x4b. Literal "WitchwoodCavern" 0x012DCEE8.
// Shape: a translated script thread: the `&&` region test carries the conditional-temporary flag word for its literal; the
//   follower count is a signed long over size(); each per-follower loop opens with the terminate check; the two vectors are
//   destroyed inline at the loop's own exits and through the out-of-line dtors at the shared exit of the inner waits.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_RansomVictimScript.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };

void NScript::CQ_RansomVictimScript::DisappearFollowers()
{
    std::vector<CScriptThing> followers;
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_followers;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (Game->IsRegionLoaded("WitchwoodCavern") && !RescuedVictim) {
            if (IsActiveThreadTerminating()) return;
            Game->GetFollowingEntityList(Game->GetHero(), followers);
            long num = followers.size();
            seh_followers.resize(num);
            for (long i = 0; i < num; i++) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase& seh = seh_followers[i];
                if (seh.IsValid()) seh.Release();
                while (!Game->StartScriptingEntity(followers[i], seh_followers[i], SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                Game->EntitySetAlpha(followers[i], 0.0f, true);
                Game->SetIsThingForcePushable(followers[i], false);
                Game->EntitySetAsRespondToHit(followers[i], false);
                Game->EntitySetAsDamageable(followers[i], false);
                Game->EntitySetTargetable(followers[i], false);
                Game->EntitySetAsDisplayingEmoteIcon(followers[i], false);
                Game->EntitySetAsHiddenOnMiniMap(followers[i], true);
            }
            if (IsActiveThreadTerminating()) return;
            while (Game->IsRegionLoaded("WitchwoodCavern") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            for (long i = 0; i < num; i++) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase& seh = seh_followers[i];
                if (seh.IsValid()) seh.Release();
                Game->EntitySetAlpha(followers[i], 1.0f, true);
                Game->SetIsThingForcePushable(followers[i], true);
                Game->EntitySetAsRespondToHit(followers[i], true);
                Game->EntitySetAsDamageable(followers[i], true);
                Game->EntitySetTargetable(followers[i], true);
                Game->EntitySetAsDisplayingEmoteIcon(followers[i], true);
                Game->EntitySetAsHiddenOnMiniMap(followers[i], false);
            }
            if (IsActiveThreadTerminating()) return;
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
