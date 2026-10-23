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

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
namespace CombatDebug{
const char EmptyString[]="";static int strings;static bool attacker,defender;static CombatantMap::node*found;
CCharString::CCharString():PData(0){}
static CBasicStringData*make(const char*t){CBasicStringData*p=new CBasicStringData;p->StringLength=(unsigned)strlen(t);p->DataLength=p->StringLength+1;p->PData=new char[p->DataLength];strcpy(p->PData,t);p->RefCount=1;p->UseFastExtend=false;++strings;return p;}
CCharString::CCharString(const char*t,int):PData(make(t)){}
CCharString::CCharString(const CCharString&x):PData(x.PData){if(PData)++PData->RefCount;}
CCharString::~CCharString(){if(PData&&!--PData->RefCount){delete[]PData->PData;delete PData;--strings;}}
CCharString&CCharString::operator=(const CCharString&x){if(this!=&x){CCharString tmp(x);CBasicStringData*p=PData;PData=tmp.PData;tmp.PData=p;}return*this;}
CCharString&CCharString::operator=(const char*t){CCharString tmp(t);return*this=tmp;}
bool CreatureMethods::IsAttacker()const{return attacker;}bool CreatureMethods::IsAttacked()const{return defender;}

long CCombatManager::GetDefenderZone(const CThingCreatureBase&,const CThingCreatureBase&,long n)const{assert(n==0);return 3;}
CCombatSequenceBase::~CCombatSequenceBase(){}void CCombatSequenceBase::Delete(){}CCharString CCombatSequenceBase::GetName()const{return CCharString("CombatSequence_Bandit");}
namespace _STL{template<class T>vector<T>::vector(const vector&x){unsigned n=x._M_finish-x._M_start;_M_start=n?(T*)malloc(n*sizeof(T)):0;_M_finish=_M_start+n;_M_end_of_storage=_M_finish;if(n)memcpy(_M_start,x._M_start,n*sizeof(T));}}
}
int main(){using namespace CombatDebug;assert(sizeof(CCombatDebugInfo)==44);assert(offsetof(CAttackingCombatantInfo,CurrentSequence)==32);CreatureMethods c;memset(&c,0,sizeof c);((CThing&)c).UniqueID=123;CCombatManager m;CombatantMap::node n;found=&n;CombatantMap::node head;head.Parent=&n;head.Left=head.Right=&n;n.Parent=&head;n.Left=n.Right=0;m.Combatants.Head=&head;m.Combatants.Count=1;m.Combatants.Compare=0;n.Key=123;n.Value.Parent=&c;n.Value.AttackerInfo.Attacked=&c;n.Value.AttackerInfo.AttackerNumber=4;n.Value.AttackerInfo.CombatWheelRingIndex=9;n.Value.AttackerInfo.CurrentSequence=-1;attacker=false;defender=true;{CCombatDebugInfo a=m.GetCombatDebugInfo(c);assert(!a.Attacker&&a.Defender&&a.Zone==0&&!a.PlacementVectors._M_start);}attacker=true;{CCombatDebugInfo b=m.GetCombatDebugInfo(c);assert(b.Attacker&&b.Defender&&b.Zone==3&&b.AttackerNumber==4&&strcmp(b.CurrentSequenceName.c_str(),"")==0);}CCombatSequenceBase sequence;CCountedPointer<CCombatSequenceBase>entries[1];entries[0].Data=&sequence;entries[0].Info=0;n.Value.AttackerInfo.CombatSequences._M_start=entries;n.Value.AttackerInfo.CombatSequences._M_finish=entries+1;n.Value.AttackerInfo.CombatSequences._M_end_of_storage=entries+1;n.Value.AttackerInfo.CurrentSequence=0;{CCombatDebugInfo d=m.GetCombatDebugInfo(c);assert(strcmp(d.CurrentSequenceName.c_str(),"Bandit")==0);}n.Value.AttackerInfo.CombatSequences._M_start=0;n.Value.AttackerInfo.CombatSequences._M_finish=0;n.Value.AttackerInfo.CombatSequences._M_end_of_storage=0;assert(!strings);puts("006eac50_TEST PASS");}