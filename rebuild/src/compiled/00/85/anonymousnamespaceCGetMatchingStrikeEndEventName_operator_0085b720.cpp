// `anonymous namespace'::CGetMatchingStrikeEndEventName::operator()(std::pair<CCharString, float>& event) -- for_each functor: an end event matching the strike event (legacy flags, or weapon flag + helper point name) whose frame lies after the start and before the best so far becomes the strike's end
// Flags: /O2 /Oy /W3
// Leaf: ??RCGetMatchingStrikeEndEventName@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path, as the landed 0x00870C90.)
// Identity (NF56, round 7, queue q10447): FableWin.pdb 0x0282F660
//   ??RCGetMatchingStrikeEndEventName@?A0xad05d24c@@QAEXAAU?$pair@VCCharString@@M@std@@@Z (anonymous-namespace functor of the strike-events
//   TU; non-const pair reference); Ego_r.pdb 0x0083B660 ??$_For_each@PAU?$pair@VCCharString@@M@std@@VCGetMatchingStrikeEndEventName@?A0xad05d24c@@...
//   applies it over the animation's vector<pair<CCharString, float> > events. Layout (PDB functor, 12 bytes): StrikeEvent +0,
//   BestEventEndFrame +4, FoundEvent +8. Was StrikeEvents::CGetMatchingStrikeEndEventName (a stand-in namespace) over local std::pair /
//   std::vector models. Now in namespace { } on the shared stlport/fable_containers.h (vector<CCharString> destructor out of line:
//   0x00414EA0, FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE), fable_string.h CCharString, the PDB string globals of the TU
//   (STRIKE_EVENT_*_IDENTIFIER / *_LEGACY, as row 0x00856780) and CMainGameComponent::GetConstantFPS (0x01375550).
// Callees: IsValidStrikeEventName 0x00856780 (q10446: the same anonymous namespace; the same-TU noinline body below is the landed
//   copy of that row, which gives this TU's register shape), GetHelperPointNamesFromStrikeEventName 0x00858E00 (Ego_r.pdb 0x008386C0
//   ?GetHelperPointNamesFromStrikeEventName@?A0xad05d24c@@YIXABVCCharString@@AAV?$vector@VCCharString@@...@std@@_N22@Z), ~vector 0x00414EA0.
// CStrikeEvent has no shared header: the row's layout model (flags 0/1/2, HelperPointName 4, index 8, StartFrame 12, EndFrame 16, ...).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include <stdlib.h>
#include <string.h>
#include "stlport/fable_containers.h"
#include "engine/CMainGameComponent.h"
extern const CCharString STRIKE_EVENT_WEAPON_START_IDENTIFIER,STRIKE_EVENT_WEAPON_END_IDENTIFIER,STRIKE_EVENT_START_IDENTIFIER,STRIKE_EVENT_END_IDENTIFIER;   // 0x013BAC64 / 68 / 6C / 70
extern const CCharString STRIKE_EVENT_BEGIN_LEGACY,STRIKE_EVENT_END_LEGACY,SECONDARY_STRIKE_EVENT_BEGIN_LEGACY,SECONDARY_STRIKE_EVENT_END_LEGACY;   // 0x013BAC78 / 7C / 80 / 84
namespace {
struct CStrikeEvent{bool Legacy,PrimaryLegacy,Weapon;CCharString HelperPointName;long StrikeEventIndex,StartFrame,EndFrame;unsigned char PWeapon[8];unsigned char StruckThingList[12];};
FABLE_FORCEINLINE bool StartsWith(const CCharString&s,const CCharString&p){return s.StartsWith(p);}
FABLE_FORCEINLINE bool Equal(const CCharString&a,const CCharString&b){return a==b;}
__declspec(noinline)bool __fastcall IsValidStrikeEventName(const CCharString&name,bool&start,bool&weapon,bool&legacy,bool&primaryLegacy){
 weapon=false;bool a=StartsWith(name,STRIKE_EVENT_WEAPON_START_IDENTIFIER),b=StartsWith(name,STRIKE_EVENT_WEAPON_END_IDENTIFIER);
 if(!a&&!b){a=StartsWith(name,STRIKE_EVENT_START_IDENTIFIER);b=StartsWith(name,STRIKE_EVENT_END_IDENTIFIER);}else weapon=true;
 if(a||b){legacy=false;start=a;return true;}
 legacy=true;
 if(Equal(name,STRIKE_EVENT_BEGIN_LEGACY)){start=true;primaryLegacy=true;return true;}
 if(Equal(name,SECONDARY_STRIKE_EVENT_BEGIN_LEGACY)){start=true;primaryLegacy=false;return true;}
 if(Equal(name,STRIKE_EVENT_END_LEGACY)){start=false;primaryLegacy=true;return true;}
 if(Equal(name,SECONDARY_STRIKE_EVENT_END_LEGACY)){start=false;primaryLegacy=false;return true;}
 return false;
}
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString&,std::vector<CCharString>&,bool,bool,bool);   // 0x00858E00
class CGetMatchingStrikeEndEventName{public:   // FableWin.pdb: a class, constructed from const CCombatActionBase::CStrikeEvent& (not emitted here)
 CGetMatchingStrikeEndEventName(const CStrikeEvent&strikeEvent);void operator()(std::pair<CCharString,float>&);CStrikeEvent*StrikeEvent;long BestEventEndFrame;bool FoundEvent;};
void CGetMatchingStrikeEndEventName::operator()(std::pair<CCharString,float>&event){
 float frame=CMainGameComponent::GetConstantFPS()*event.second;bool start,weapon,legacy,primaryLegacy;
 if(IsValidStrikeEventName(event.first,start,weapon,legacy,primaryLegacy)){
  if(!start){bool matches=false;
   if(StrikeEvent->Legacy&&legacy){if(!StrikeEvent->PrimaryLegacy)return;matches=primaryLegacy;}
   else{if(weapon!=StrikeEvent->Weapon)return;std::vector<CCharString>names;GetHelperPointNamesFromStrikeEventName(event.first,names,weapon,legacy,primaryLegacy);if(Equal(names[0],StrikeEvent->HelperPointName))matches=true;}
   if(matches&&StrikeEvent->StartFrame<frame&&BestEventEndFrame>frame){BestEventEndFrame=(long)frame;FoundEvent=true;}
  }
 }
}
}
