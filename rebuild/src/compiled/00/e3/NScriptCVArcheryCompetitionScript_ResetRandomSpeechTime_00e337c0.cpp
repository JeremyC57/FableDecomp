// NScript::CV_ArcheryCompetitionScript::ResetRandomSpeechTime() -- Set the speech timer to maximum minus a random variation.
// Leaf: ?ResetRandomSpeechTime@CV_ArcheryCompetitionScript@NScript@@QAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: original PDB member names, own callers00e3372f/00e34332/00e34a77/00e34b70/00e34d3e and distinct script-def field pairs; CTimer GetIndex is original long const accessor. Actual global game interface SetTimer slot89 and MSVCR71 rand thunk00bfeb16.
// Layout: original typed script prefix through timer+112, unused suffix omitted. True CScriptDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase; own constructor00f28f90 base initialization confirms packed parent57, first own long60, original typed vectors12. Def Max/Var+912/916. Only read prefix modeled, no complete owner/definition size claimed. Shared headers aliased.
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a one-line local stand-in)
class CCPPointerInfo;
#include "engine/CCountedPointer.h"

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};

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
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" int __cdecl rand(void);
// agentNF60: the def chain (CIVCountedPointeeBase, CResource, CDefPointeeBase, CDefString, CDefClassBase through a renamed
//   engine/CDefClassBase.h include, CParentDefClassBase, CScriptDef) was modelled locally with the stand-in `CScriptDef* g_ScriptDef`;
//   it is the shared headers now, and the global is pGlobalScriptDef (CDefPointer<const CScriptDef>, 0x0143E90C).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"
class CGameScriptInterfaceBase:public CBaseClassNonCopyable{public:virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void UnknownSlot7(){}virtual void UnknownSlot8(){}virtual void UnknownSlot9(){}virtual void UnknownSlot10(){}virtual void UnknownSlot11(){}virtual void UnknownSlot12(){}virtual void UnknownSlot13(){}virtual void UnknownSlot14(){}virtual void UnknownSlot15(){}virtual void UnknownSlot16(){}virtual void UnknownSlot17(){}virtual void UnknownSlot18(){}virtual void UnknownSlot19(){}virtual void UnknownSlot20(){}virtual void UnknownSlot21(){}virtual void UnknownSlot22(){}virtual void UnknownSlot23(){}virtual void UnknownSlot24(){}virtual void UnknownSlot25(){}virtual void UnknownSlot26(){}virtual void UnknownSlot27(){}virtual void UnknownSlot28(){}virtual void UnknownSlot29(){}virtual void UnknownSlot30(){}virtual void UnknownSlot31(){}virtual void UnknownSlot32(){}virtual void UnknownSlot33(){}virtual void UnknownSlot34(){}virtual void UnknownSlot35(){}virtual void UnknownSlot36(){}virtual void UnknownSlot37(){}virtual void UnknownSlot38(){}virtual void UnknownSlot39(){}virtual void UnknownSlot40(){}virtual void UnknownSlot41(){}virtual void UnknownSlot42(){}virtual void UnknownSlot43(){}virtual void UnknownSlot44(){}virtual void UnknownSlot45(){}virtual void UnknownSlot46(){}virtual void UnknownSlot47(){}virtual void UnknownSlot48(){}virtual void UnknownSlot49(){}virtual void UnknownSlot50(){}virtual void UnknownSlot51(){}virtual void UnknownSlot52(){}virtual void UnknownSlot53(){}virtual void UnknownSlot54(){}virtual void UnknownSlot55(){}virtual void UnknownSlot56(){}virtual void UnknownSlot57(){}virtual void UnknownSlot58(){}virtual void UnknownSlot59(){}virtual void UnknownSlot60(){}virtual void UnknownSlot61(){}virtual void UnknownSlot62(){}virtual void UnknownSlot63(){}virtual void UnknownSlot64(){}virtual void UnknownSlot65(){}virtual void UnknownSlot66(){}virtual void UnknownSlot67(){}virtual void UnknownSlot68(){}virtual void UnknownSlot69(){}virtual void UnknownSlot70(){}virtual void UnknownSlot71(){}virtual void UnknownSlot72(){}virtual void UnknownSlot73(){}virtual void UnknownSlot74(){}virtual void UnknownSlot75(){}virtual void UnknownSlot76(){}virtual void UnknownSlot77(){}virtual void UnknownSlot78(){}virtual void UnknownSlot79(){}virtual void UnknownSlot80(){}virtual void UnknownSlot81(){}virtual void UnknownSlot82(){}virtual void UnknownSlot83(){}virtual void UnknownSlot84(){}virtual void UnknownSlot85(){}virtual void UnknownSlot86(){}virtual void UnknownSlot87(){}virtual void UnknownSlot88(){}virtual void SetTimer(long,long)=0;};class CGameScriptInterface:public CGameScriptInterfaceBase{public:virtual void SetTimer(long,long);};extern CGameScriptInterfaceBase* pDSTGame;class CTimer{public:long TimerIndex;long GetIndex()const{return TimerIndex;}FABLE_FORCEINLINE CTimer& operator=(long time){pDSTGame->SetTimer(TimerIndex,time);return *this;}};
namespace NScript{class CV_ArcheryCompetitionScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long Score;bool IsOwnerDead;bool PrizeAlreadyGot;bool QuestActivated;bool AttackedByHero;bool GiveAttackWarning;bool QuestFinished;bool WelcomeComplete;bool Pickpocketed;long DummyCounter;long ArcheryHighScoreInfoID;long ArcheryGameScoreInfoID;long ArcheryGameTimerID;bool SteppedOut;bool UsedMagic;bool ShotOwner;bool GivenReallyCoolPrize;bool GivenFirstHighScorePrize;bool AddedReallyCoolPrizeToOwner;bool AddedFirstHighScorePrizeToOwner;CTimer GameTimeLeft;CTimer RandomSpeechTimer;void ResetRandomSpeechTime();};void CV_ArcheryCompetitionScript::ResetRandomSpeechTime(){const CScriptDef* definition=pGlobalScriptDef.Get();RandomSpeechTimer=pGlobalScriptDef->ArcheryRandomSpeechTimerMax-rand()%definition->ArcheryRandomSpeechTimerVar;}}
#ifdef _MSC_VER
typedef char ExactParentDefSize[sizeof(CParentDefClassBase)==57?1:-1];typedef char ExactTimerOffset[offsetof(NScript::CV_ArcheryCompetitionScript,RandomSpeechTimer)==112?1:-1];typedef char ExactDefOffset[offsetof(CScriptDef,ArcheryRandomSpeechTimerMax)==912?1:-1];
#endif