// 0085b720 CGetMatchingStrikeEndEventName::operator()
// Flags: /O2 /Oy /W3
// Leaf: ??RCGetMatchingStrikeEndEventName@StrikeEvents@@
// NF52 (round 7) attempt: CCharString from fable_string.h (StartsWith / operator== inline), replacing the StrikeEvents CCharString /
// CBasicStringData / CBasicString models and the Text / StartsWith / Equal helpers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#include "fable_compiler.h"
#include "fable_string.h"
#include <stdlib.h>
#include <new>
#include <string.h>
namespace StrikeEvents{
// Layout: retail CBasicStringData is17 bytes: text0,length4,capacity8,growth12,refs13.
using ::CCharString;
namespace std{
template<class A,class B>struct pair{A first;B second;};
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;FABLE_FORCEINLINE vector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}~vector();};
}
// Layout: CStrikeEvent flags0/1/2,HelperPointName4,Index8,StartFrame12,EndFrame16,weapon20,hit-list28; PDB and retail receiver reads.
struct CStrikeEvent{bool Legacy,PrimaryLegacy,Weapon;CCharString HelperPointName;long StrikeEventIndex,StartFrame,EndFrame;unsigned char PWeapon[8];unsigned char StruckThingList[12];};
extern long FramesPerSecond;
extern CCharString StrikeStartCarried,StrikeEndCarried,StrikeStart,StrikeEnd,CombatStrike,CombatStop,CombatStrike2,CombatStop2;
FABLE_FORCEINLINE bool StartsWith(const CCharString&s,const CCharString&p){return s.StartsWith(p);}
FABLE_FORCEINLINE bool Equal(const CCharString&a,const CCharString&b){return a==b;}
__declspec(noinline)bool __fastcall IsValidStrikeEventName(const CCharString&name,bool&start,bool&weapon,bool&legacy,bool&primaryLegacy){
 weapon=false;bool a=StartsWith(name,StrikeStartCarried),b=StartsWith(name,StrikeEndCarried);
 if(!a&&!b){a=StartsWith(name,StrikeStart);b=StartsWith(name,StrikeEnd);}else weapon=true;
 if(a||b){legacy=false;start=a;return true;}
 legacy=true;
 if(Equal(name,CombatStrike)){start=true;primaryLegacy=true;return true;}
 if(Equal(name,CombatStrike2)){start=true;primaryLegacy=false;return true;}
 if(Equal(name,CombatStop)){start=false;primaryLegacy=true;return true;}
 if(Equal(name,CombatStop2)){start=false;primaryLegacy=false;return true;}
 return false;
}
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString&,std::vector<CCharString>&,bool,bool,bool);
// Layout: PDB functor12 bytes: StrikeEvent0,BestEventEndFrame4,FoundEvent8.
struct CGetMatchingStrikeEndEventName{CStrikeEvent*StrikeEvent;long BestEventEndFrame;bool FoundEvent;void operator()(const std::pair<CCharString,float>&);};
void CGetMatchingStrikeEndEventName::operator()(const std::pair<CCharString,float>&event){
 float frame=FramesPerSecond*event.second;bool start,weapon,legacy,primaryLegacy;
 if(IsValidStrikeEventName(event.first,start,weapon,legacy,primaryLegacy)){
  if(!start){bool matches=false;
   if(StrikeEvent->Legacy&&legacy){if(!StrikeEvent->PrimaryLegacy)return;matches=primaryLegacy;}
   else{if(weapon!=StrikeEvent->Weapon)return;std::vector<CCharString>names;GetHelperPointNamesFromStrikeEventName(event.first,names,weapon,legacy,primaryLegacy);if(Equal(names._M_start[0],StrikeEvent->HelperPointName))matches=true;}
   if(matches&&StrikeEvent->StartFrame<frame&&BestEventEndFrame>frame){BestEventEndFrame=(long)frame;FoundEvent=true;}
  }
 }
}
}