// NScript::CQ_GuildTrainingWoodsMeleeScript::CScorpionHome::Init() -- Initialise the scorpion count and flourish hint.
// Leaf: ?Init@CScorpionHome@CQ_GuildTrainingWoodsMeleeScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own named ScorpionHome registration00d6670c ->factory00d66ba0 ->vtable012d1da0 slot2; original PDB nested private virtual Init, named ScorpionsLeft/FlourishHint and CScriptDef GUI_MeleeBeetles; global0143e90c and typed retail field+3856.
// Layout: original CEntityScriptBase20, parent/master20/24, count28, hint32, full child36. Typed definition prefix through GUI_MeleeBeetles; original def base chain with packed parent57 and STLport12-byte vectors reconciles donor offsets. No opaque base prefix; inactive virtual slots are descriptive.
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
#include "engine/CCountedPointer.h"

#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a one-line local stand-in)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CGameScriptInterfaceBase;class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript{class CQ_SunnyvaleMasterData;}
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};

// agentNF60: the def chain (CIVCountedPointeeBase, CResource, CDefPointeeBase, CDefString, CDefClassBase through a renamed
//   engine/CDefClassBase.h include, CParentDefClassBase, CScriptDef) was modelled locally with the stand-in `CScriptDef* g_ScriptDef`;
//   it is the shared headers now, and the global is pGlobalScriptDef (CDefPointer<const CScriptDef>, 0x0143E90C).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"
namespace NScript{class CQ_GuildTrainingWoodsMeleeScript{public:class CScorpionHome:public CEntityScriptBase{public:CQ_GuildTrainingWoodsMeleeScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;long ScorpionsLeft;bool FlourishHint;private:virtual void Init();};};void CQ_GuildTrainingWoodsMeleeScript::CScorpionHome::Init(){ScorpionsLeft=(long)pGlobalScriptDef->GUI_MeleeBeetles;FlourishHint=false;}}
#ifdef _MSC_VER
typedef char DefOffset[offsetof(CScriptDef,GUI_MeleeBeetles)==3856?1:-1];typedef char ChildSize[sizeof(NScript::CQ_GuildTrainingWoodsMeleeScript::CScorpionHome)==36?1:-1];
#endif