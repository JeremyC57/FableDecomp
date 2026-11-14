// CTCCarriedActionUseActivateQuest::OnActionUse(CThing& user) -- use a quest-activating item: the hero-guild teleport item opens the teleport map (and posts the teleport game event) when nothing prevents it, any other item makes the user activate its quest
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B57C0, 776 bytes, thiscall `ret 4`, /GS frame 0x100.
// Leaf: ?OnActionUse@CTCCarriedActionUseActivateQuest@@UAEXAAVCThing@@@Z
// Flags: /GS
// Flags evidence: the security cookie 0x0139C8A8 stored at entry and __security_check_cookie 0x00BFE9F9 before `ret 4`.
// Name: Ego_r.pdb ?OnActionUse@CTCCarriedActionUseActivateQuest@@UAEXAAVCThing@@@Z (0x007B59A0, after GetSubDef<CActivateQuestDef> 0x007B5890 /
//   CActivateQuestDef::Transfer 0x007B5950; retail GetSubDef<CActivateQuestDef> 0x007B5680 precedes this row), vtable slot 28 like the matched
//   sibling CTCCarriedActionUseExpression::OnActionUse 0x007EF200 (same CTCActionUseBase::OnActionUse 0x007EAB90 first call, `ret 4`).
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90, GetSubDef<CActivateQuestDef> 0x007B5680 (body visible: result unused, retail tests the
//   def pointer at 0x007B57FD), CDefString::operator!= 0x005FA740 / operator== 0x005B3440, CVectorMap LowerBound 0x004365B0 / 0x0040F020,
//   CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 + CModeBase::GetType (slot 1), CTCBase::PeekWorld 0x00686A80 / GetWorld 0x00686A70 /
//   PeekPlayerManager 0x00686D50, CTCInventoryMap::ConstructMapList 0x0060C900, IS_THING_ALIVE<CThingCreatureBase*> 0x005C05C0,
//   CGameEvent::AddData 0x009F1760, CPlayerManager::GetMainPlayerNumber 0x00449700, CMainGameComponent::AddGameEvent 0x0041649C,
//   CMessageEventManager::AddMessage 0x00687540, CDefStringTable::GetString 0x009D49B0, CCreatureAction_ActivateQuest ctor 0x00843F50 /
//   dtor 0x00629060, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCharString 0x0099EAE0, CTCInventoryItem::ReturnToInventory 0x005D8E50.
// Shape: the teleport path sets the map flags, then tests subtitles / region load, builds the map list into a local bool and tests
//   alive / health; a `done` flag guards the flag reset (VC7.1 threads it away: the failures share one reset block 0x007B5A64).
//   GetString is expanded in place (FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING, explicit .GetString()).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYMAP
#define FABLE_RETAIL_CTCINVENTORYMAP_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCINVENTORYMAP_CONSTRUCTMAPLIST
#define FABLE_RETAIL_CTCINVENTORYMAP_SETAS
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST
#define FABLE_RETAIL_CACTIVATEQUESTDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISPERFORMINGREGIONLOADREALLY
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
namespace NUISystem { class CComponent; }
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCarriedActionUseActivateQuest.h"
#include "engine/CActivateQuestDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_ActivateQuest.h"
#include "engine/ThingAlive.h"

extern const float DELTA_NOTIONAL_ZERO;

void CTCCarriedActionUseActivateQuest::OnActionUse(CThing& user)
{
    CTCCarriedActionUseBase::OnActionUse(user);
    CDefPointer<const CActivateQuestDef> def;
    GetParentThing().PeekBaseDef().GetSubDef(&def);   // result unused: retail tests the def pointer (0x007B57FD)
    if (!def.IsNull() && def->ScriptName != "") {
        if (def->ScriptName == "Global_TeleportToHeroGuild") {
            const CTCCreatureModeManager* modes;
            if (!(user.PeekTC(&modes) && modes->GetCurrentMode().GetType() == 34)) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(&user);
                if (!user.IsDead()) {
                    CTCInventoryMap* map;
                    user.GetTC(&map);
                    map->SetAsInventoryScreen(false);
                    map->SetAsOpenedViaGuildSeal(true);
                    bool done = false;
                    if (!CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() && !PeekWorld().IsPerformingRegionLoadReally()) {
                        bool opened = false;
                        map->ConstructMapList(0, true, &opened);
                        if (opened && IS_THING_ALIVE(creature) && creature->GetHealth() > DELTA_NOTIONAL_ZERO) {
                            CGameEvent event(26);
                            event.AddSLONG(8);
                            event.SetPlayer(PeekPlayerManager().GetMainPlayerNumber());
                            CMainGameComponent::Get()->AddGameEvent(event);
                            GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)0x3d, CMainGameComponent::GetConstantFPS(),
                                C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
                            done = true;
                        }
                    }
                    if (!done) {
                        map->SetAsInventoryScreen(true);
                        map->SetAsOpenedViaGuildSeal(false);
                    }
                }
            }
        } else if (&user != 0 && !user.IsDead()) {
            static_cast<CThingCreatureBase&>(user).SetCurrentAction(CCreatureAction_ActivateQuest(static_cast<CThingCreatureBase&>(user), 0,
                def->ScriptName.GetString(), def->LoadResources));
        }
    }
    CTCInventoryItem* item;
    if (ParentThing->GetTC(&item))
        item->ReturnToInventory();
}
