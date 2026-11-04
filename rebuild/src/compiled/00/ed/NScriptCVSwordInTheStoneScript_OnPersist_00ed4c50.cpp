// NScript::CV_SwordInTheStoneScript::OnPersist(CPersistContext& context) -- Transfer persistent state with zero defaults.
// Leaf: ?OnPersist@CV_SwordInTheStoneScript@NScript@@UAEXAAVCPersistContext@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: original PDB member signatures and unique persist field names; own vtable012f301c slot4, constructed at00ed4abf. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
// Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size128. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

class CPersistContext;class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;
class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
namespace NScript{class CV_SwordInTheStoneScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long SwordRemovalAttemptCount;bool SwordCutscene,CrowdAttemptingToPullSword,CrowdSwordPullActive,HeroHasBeenToldSwordLegend,HeroCongratulated,CrowdDisperse,HeroHasTakenSword,SwordInTheStoneIntroComplete;long HeroStrengthPhysique,HeroStrengthToughness,HeroStrengthHealth;bool StatsInitialised,AttackedByHero;CScriptThing CrowdMemberPullingCurrent,CrowdMemberPullingNull;CTimer CrowdSwordTimer;virtual void OnPersist(CPersistContext&);};void CV_SwordInTheStoneScript::OnPersist(CPersistContext& context){context.Transfer("HeroHasBeenToldSwordLegend",HeroHasBeenToldSwordLegend,false);context.Transfer("SwordRemovalAttemptCount",SwordRemovalAttemptCount,0L);context.Transfer("HeroStrengthPhysique",HeroStrengthPhysique,0L);context.Transfer("HeroStrengthToughness",HeroStrengthToughness,0L);context.Transfer("HeroStrengthHealth",HeroStrengthHealth,0L);context.Transfer("SwordInTheStoneIntroComplete",SwordInTheStoneIntroComplete,false);context.Transfer("StatsInitialised",StatsInitialised,false);}}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(NScript::CV_SwordInTheStoneScript)==128?1:-1];
#endif