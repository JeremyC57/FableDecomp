// NScript::CQ_EndGameBossBattleScript::Init() -- Reset the script progress flags.
// Leaf: ?Init@CQ_EndGameBossBattleScript@NScript@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own script-info registration 00cd5f68->00d34410 stores factory for literal Q_EndGameBossBattle, whose own allocation/constructor stores vtable012cc60c; target is slot3 Init. Original field names from named PDB class layout, independently checked against own offsets and setter uses.
// Layout: shared handle headers aliased to restore real CBaseObject/CBaseClass hierarchy and counted pointers. Original CScriptBase64 typed list/maps, CLevelScriptBase72 Game/MasterScriptData; own registry uses common master-data allocator00cdbd20. Flags at72; PC owner size108. No opaque base prefix.
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
#include "engine/CVectorMap.h"
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};

class CPersistContext;class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript{class CQ_EndGameBossBattleScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool IntroFinished,FinalDecision,FinalDecisionMade,SisterNeededForCutscene,GuildmasterChat,FinalCutscenePlayed;CScriptGameResourceObjectScriptedThingBase seh_Jack;CScriptThing Jack;virtual void Init();};
void CQ_EndGameBossBattleScript::Init(){IntroFinished=false;FinalDecision=false;FinalDecisionMade=false;FinalCutscenePlayed=false;SisterNeededForCutscene=false;GuildmasterChat=false;}}
#ifdef _MSC_VER
typedef char ExactBaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ExactOwnerSize[sizeof(NScript::CQ_EndGameBossBattleScript)==108?1:-1];
#endif