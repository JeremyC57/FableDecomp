// CombatDialogueManager::CanSaySpeech(CThingCreatureBase& creature, unsigned long textGroup) const -- whether the creature may say the text group now: speech allowed for it, it has a talk component, the group's next allowed frame has come and the talk component has that speech
// Leaf: ?CanSaySpeech@CombatDialogueManager@@QBE_NAAVCThingCreatureBase@@K@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AF6E0, 150 bytes, `ret 8`; Ego_r.pdb ?CanSaySpeech@CombatDialogueManager@@QBE_NAAVCThingCreatureBase@@K@Z.
// Callees: CombatDialogueManager::IsSpeechAllowed(const CThingCreatureBase&) const 0x008AF5F0, LowerBound 0x0040F020 (CThing::GetTC<CTCTalk>),
//   CMainGameComponent::GetCurrentWorldFrame() const 0x004167C8 (on CMainGameComponent::Get()), the STLport _Rb_tree::find const 0x008AEF20,
//   CTCTalk::DoesSpeechExist(unsigned long) 0x006E2910. IsSpeechGroupAllowed (Ego_r.pdb, same TU) is expanded inline.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTalk.h"
#include "engine/CMainGameComponent.h"
#include "engine/CombatDialogueManager.h"

bool CombatDialogueManager::IsSpeechGroupAllowed(unsigned long textGroup) const
{
    long frame = CMainGameComponent::Get()->GetCurrentWorldFrame();
    return frame >= SpeechGroupInfoMap.find(textGroup)->second.NextAllowedFrame;
}

bool CombatDialogueManager::CanSaySpeech(CThingCreatureBase& creature, unsigned long textGroup) const
{
    if (!IsSpeechAllowed(creature))
        return false;
    CTCTalk* talk;
    if (!creature.GetTC(&talk))
        return false;
    if (!IsSpeechGroupAllowed(textGroup))
        return false;
    if (talk->DoesSpeechExist(textGroup))
        return true;
    return false;
}