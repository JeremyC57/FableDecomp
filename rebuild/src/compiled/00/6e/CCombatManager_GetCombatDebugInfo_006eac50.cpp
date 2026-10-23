// 006eac50 CCombatManager::GetCombatDebugInfo
// Flags: /O2 /Oy /W3
// Leaf: GetCombatDebugInfo
// Layout: active CThingCreatureBase header plus genuine inherited CThing UniqueID+0x18; method-only extension adds no storage. CCombatManager Combatants+0x10 is retail map12, node payload+0x18. CCombatantInfo112 has AttackerInfo+4, CurrentSequence+0x24, CombatSequences+0x28. CCombatDebugInfo44: Zone0,Attacker4,AttackerNumber8,Defender12,MovingByAction13,CurrentSequenceName16,PlacementVectors20,AttackerPos32. Packed CBasicStringData17 has text0,length4,capacity8,growth12,refs13.
#include <stdlib.h>
#include <string.h>
#include "engine/CThingCreatureBase.h"
namespace CombatDebug{
#pragma pack(push,1)
struct CBasicStringData{char*PData;unsigned StringLength,DataLength;bool UseFastExtend;int RefCount;};
#pragma pack(pop)
extern const char EmptyString[];
struct CCharString{CBasicStringData*PData;CCharString();CCharString(const char*,int=-1);CCharString(const CCharString&);~CCharString();CCharString&operator=(const CCharString&);CCharString&operator=(const char*);__forceinline const char*c_str()const{return !PData?EmptyString:PData->PData;}};
struct C3DVector{float X,Y,Z;};
namespace _STL{template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;__forceinline vector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}vector(const vector&);__forceinline ~vector(){if(_M_start)free(_M_start);}};}
// Only the genuine inherited CThing base prefix is viewed; active creature fields are never shadowed.
struct CThing{unsigned char BasePrefix[24];unsigned __int64 UniqueID;};
struct CreatureMethods:CThingCreatureBase{bool IsAttacker()const;bool IsAttacked()const;};
struct CCPPointerInfo;template<class T>struct CCountedPointer{T*Data;CCPPointerInfo*Info;__forceinline T*operator->()const{return Data;}};
struct CCombatSequenceBase{virtual~CCombatSequenceBase();virtual void Delete();virtual CCharString GetName()const;};
struct CAttackingCombatantInfo{CThingCreatureBase*Attacked;long AttackerNumber,CombatWheelRingIndex;bool PreferredAttackDollySet;long PreferredAttackDollyTimer;float PreferredAttackDollyVector[2];bool UsingCombatWheel;long CurrentSequence;_STL::vector<CCountedPointer<CCombatSequenceBase> >CombatSequences;};
struct CCombatantInfo{CThingCreatureBase*Parent;CAttackingCombatantInfo AttackerInfo;unsigned char DefenderInfo[60];};
struct CombatantMap{struct node{unsigned Colour;node*Parent,*Left,*Right;unsigned __int64 Key;CCombatantInfo Value;};node*Head;unsigned Count,Compare;node*find(const unsigned __int64&);};
__declspec(noinline) CombatantMap::node*CombatantMap::find(const unsigned __int64&key){node*y=Head,*x=Head->Parent;while(x){if(!(x->Key<key)){y=x;x=x->Left;}else x=x->Right;}if(y==Head||key<y->Key)return Head;return y;}
__forceinline CCombatantInfo&FindCombatant(CombatantMap&map,const CThingCreatureBase&creature){unsigned __int64 id=((const CThing&)creature).UniqueID;return map.find(id)->Value;}
struct CCombatDebugInfo{long Zone;bool Attacker;long AttackerNumber;bool Defender,MovingByAction;CCharString CurrentSequenceName;_STL::vector<C3DVector>PlacementVectors,AttackerPos;__forceinline CCombatDebugInfo(){memset(this,0,sizeof(*this));}__forceinline CCombatDebugInfo(const CCombatDebugInfo&x):Zone(x.Zone),Attacker(x.Attacker),AttackerNumber(x.AttackerNumber),Defender(x.Defender),MovingByAction(x.MovingByAction),CurrentSequenceName(x.CurrentSequenceName),PlacementVectors(x.PlacementVectors),AttackerPos(x.AttackerPos){}};
struct CCombatManager{unsigned Padding;unsigned char RegisteredCombatSequences[12];CombatantMap Combatants;long GetDefenderZone(const CThingCreatureBase&,const CThingCreatureBase&,long)const;CCombatDebugInfo GetCombatDebugInfo(const CThingCreatureBase&)const;};
CCombatDebugInfo CCombatManager::GetCombatDebugInfo(const CThingCreatureBase&creature)const{
 CCombatDebugInfo info;
 info.Attacker=((const CreatureMethods&)creature).IsAttacker();
 if(info.Attacker){CCombatantInfo&combatant=FindCombatant(const_cast<CombatantMap&>(Combatants),creature);
  info.Zone=GetDefenderZone(*combatant.AttackerInfo.Attacked,creature,0);
  info.CurrentSequenceName=combatant.AttackerInfo.CurrentSequence>=0?combatant.AttackerInfo.CombatSequences._M_start[combatant.AttackerInfo.CurrentSequence]->GetName():CCharString("");
  if(info.CurrentSequenceName.PData&&int(info.CurrentSequenceName.PData->StringLength)>15){CCharString shortened;shortened=info.CurrentSequenceName.c_str()+15;info.CurrentSequenceName=shortened;}
  info.AttackerNumber=combatant.AttackerInfo.AttackerNumber;
 }
 info.Defender=((const CreatureMethods&)creature).IsAttacked();return info;
}
}