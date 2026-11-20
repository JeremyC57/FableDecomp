// CombatDialogueManager::IsSpeechGroupAllowed(unsigned long group) const -- whether the world frame has reached the group's NextAllowedFrame in the speech-group map
// Leaf: ?IsSpeechGroupAllowed@CombatDialogueManager@@QBE_NK@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x008AF1E0, 51 bytes, `__fastcall`, `ret 4` (Ego_r.pdb ?IsSpeechGroupAllowed@CombatDialogueManager@@QBE_NK@Z).
// Callees: CMainGameComponent::GetCurrentWorldFrame 0x004167C8 on CMainGameComponent::Get() 0x013B86A0; map<unsigned long,TSpeechGroupInfo>::find(const
// unsigned long&) const 0x008AEF20 (iterator returned through a hidden pointer; manifest label "BinaryTree_Search").
// Layout: SpeechGroupInfoMap +0x00 (retail STLport map, 12 bytes); tree node value at +0x10, TSpeechGroupInfo::NextAllowedFrame
// at node +0x14. The group must be present (the caller found it); find() is dereferenced unchecked.
// Shape: the iterator is a named local read back from its frame slot; the result is a named `bool` local over a `const` frame
// local -- the bare `return frame >= x;` adds `xor eax,eax` and loads x into ecx.
// NF175 (2026-11-20, q4989): the map find const 0x008AEF20 respelled from the local stand-in ?find@CSpeechGroupInfoMap@@QBE?AUCSpeechGroupMapIterator@@ABK@Z
//   to the STLport 4.6 non-template _Rb_tree::find const (the catalog / reloc_bindings spelling the landed 0x008AF6E0 CanSaySpeech binds): the row moves
//   onto the shared engine/CombatDialogueManager.h (retail branch, std::map<unsigned long,TSpeechGroupInfo> SpeechGroupInfoMap) and
//   engine/CMainGameComponent.h, with 0x008AF6E0's define block; the local CSpeechGroupInfoMap / CSpeechGroupMapIterator / SSpeechGroupNode /
//   TSpeechGroupInfo / CombatDialogueManager / CMainGameComponent models are dropped. The named iterator and bool locals are kept.
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

bool CombatDialogueManager::IsSpeechGroupAllowed(unsigned long group) const
{
    const long frame = CMainGameComponent::Get()->GetCurrentWorldFrame();
    std::map<unsigned long, TSpeechGroupInfo>::const_iterator it = SpeechGroupInfoMap.find(group);
    bool allowed = frame >= it->second.NextAllowedFrame;
    return allowed;
}
