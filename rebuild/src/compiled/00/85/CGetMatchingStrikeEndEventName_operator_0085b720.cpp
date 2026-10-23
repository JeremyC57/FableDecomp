// 0085b720 CGetMatchingStrikeEndEventName::operator()
// Flags: /O2 /Oy /W3
// Leaf: ??RCGetMatchingStrikeEndEventName@StrikeEvents@@
#include <stdlib.h>
#include <new>
#include <string.h>
namespace StrikeEvents{
// Layout: retail CBasicStringData is17 bytes: text0,length4,capacity8,growth12,refs13.
#pragma pack(push,1)
struct CBasicStringData{char*PData;unsigned StringLength,DataLength;bool UseFastExtend;int RefCount;};
#pragma pack(pop)
struct CCharString{CBasicStringData*PData;CCharString();CCharString(const CCharString&);~CCharString();};
template<class T>struct CBasicString{static int __fastcall Compare(const T*,const T*);};
namespace _STL{
template<class A,class B>struct pair{A first;B second;};
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;__forceinline vector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}~vector();};
}
// Layout: CStrikeEvent flags0/1/2,HelperPointName4,Index8,StartFrame12,EndFrame16,weapon20,hit-list28; PDB and retail receiver reads.
struct CStrikeEvent{bool Legacy,PrimaryLegacy,Weapon;CCharString HelperPointName;long StrikeEventIndex,StartFrame,EndFrame;unsigned char PWeapon[8];unsigned char StruckThingList[12];};
extern long FramesPerSecond;
extern CCharString StrikeStartCarried,StrikeEndCarried,StrikeStart,StrikeEnd,CombatStrike,CombatStop,CombatStrike2,CombatStop2;
__forceinline const char*Text(const CCharString&s){return s.PData?s.PData->PData:"";}
__forceinline bool StartsWith(const CCharString&s,const CCharString&p){return strncmp(Text(s),Text(p),p.PData?p.PData->StringLength:0)==0;}
__forceinline bool Equal(const CCharString&,const CCharString&);
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
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString&,_STL::vector<CCharString>&,bool,bool,bool);
__forceinline bool Equal(const CCharString&a,const CCharString&b){if(a.PData==b.PData)return true;if(!a.PData||!b.PData)return false;if(a.PData->StringLength!=b.PData->StringLength)return false;return CBasicString<char>::Compare(a.PData->PData,b.PData->PData)==0;}
// Layout: PDB functor12 bytes: StrikeEvent0,BestEventEndFrame4,FoundEvent8.
struct CGetMatchingStrikeEndEventName{CStrikeEvent*StrikeEvent;long BestEventEndFrame;bool FoundEvent;void operator()(const _STL::pair<CCharString,float>&);};
void CGetMatchingStrikeEndEventName::operator()(const _STL::pair<CCharString,float>&event){
 float frame=FramesPerSecond*event.second;bool start,weapon,legacy,primaryLegacy;
 if(IsValidStrikeEventName(event.first,start,weapon,legacy,primaryLegacy)){
  if(!start){bool matches=false;
   if(StrikeEvent->Legacy&&legacy){if(!StrikeEvent->PrimaryLegacy)return;matches=primaryLegacy;}
   else{if(weapon!=StrikeEvent->Weapon)return;_STL::vector<CCharString>names;GetHelperPointNamesFromStrikeEventName(event.first,names,weapon,legacy,primaryLegacy);if(Equal(names._M_start[0],StrikeEvent->HelperPointName))matches=true;}
   if(matches&&StrikeEvent->StartFrame<frame&&BestEventEndFrame>frame){BestEventEndFrame=(long)frame;FoundEvent=true;}
  }
 }
}
}