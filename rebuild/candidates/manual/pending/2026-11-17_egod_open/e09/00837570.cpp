// CThingTrackNode::Save(CCharString& script) -- append the track node's level-script text after CThingPhysical's: "LinkedToUID1 <previous node uid>;", "LinkedToUID2 <next node uid>;" (remapped for serialisation, 0 when unlinked), "Start TRUE|FALSE;", "End TRUE|FALSE;" and "ScriptName <track name|NULL>;", each line ending "\r\n"
// Grade: DIFFER(697v697) -- frame/schedule only: retail zeroes the NULL-temporary flag first at entry (0x00837576, before `mov ebp,[esp+20h]`), ours after `mov esi,ecx`; the ScriptName line's temps: retail NULL at F+8 and the "ScriptName "+x result sharing the flag word F+0xC (0x008377BE / 0x008377F5 / 0x00837819), ours F+4 / F+8.
// Leaf: ?Save@CThingTrackNode@@UAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00837570, 697 bytes (public virtual, `ret 4`).
// Ego_d.pdb (thing_track_node.obj): only `this` and `script` -- no named locals, so every intermediate is an unnamed temporary.
// Callees: CThingPhysical::Save 0x006A4BA0 (by name), CThing::IsLevelLess 0x004C7930, CThingManager::RemapUIDForSerialisation 0x0051E210
//   on CThing::PCurrentThingManager 0x013B8A1C, GFUHUGEToCharString 0x0099F930, operator+(const char*, const CCharString&) 0x0099F690,
//   operator+(const CCharString&, const char*) 0x0099F600, CCharString::operator+= 0x0099F0A0, CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0.
// Shape: the landed CThingAICreature::Save 0x008304A0 / CThingSwitch::Save 0x0083BF90 lines (`script += "Key " + value + ";\r\n";`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_SAVE
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_SAVE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_SAVE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingTrackNode.h"
#include "engine/CThingManager.h"
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);            // 0x0099F930

void CThingTrackNode::Save(CCharString& script)
{
    CThingGameObject::Save(script);
    script += "LinkedToUID1 " + GFUHUGEToCharString(PreviousNode ? GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), PreviousNode->GetUniqueID()) : 0) + ";\r\n";
    script += "LinkedToUID2 " + GFUHUGEToCharString(NextNode ? GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), NextNode->GetUniqueID()) : 0) + ";\r\n";
    script += CCharString("") + "Start " + (IsStart ? "TRUE" : "FALSE") + ";\r\n";
    script += CCharString("") + "End " + (IsEnd ? "TRUE" : "FALSE") + ";\r\n";
    script += "ScriptName " + (TrackName == "" ? CCharString("NULL") : TrackName) + ";\r\n";
}
