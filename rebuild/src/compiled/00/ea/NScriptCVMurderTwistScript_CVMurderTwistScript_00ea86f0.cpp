// NScript::CV_MurderTwistScript::CV_MurderTwistScript(CScriptDataBase* data,CGameScriptInterfaceBase& game) -- Initialize script ownership and its empty vector.
// Leaf: ??0CV_MurderTwistScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own script-info registration00cd8ffb literalV_MurderTwist -> factory -> constructor00ea86f0/vtable012ee634; exact original PDB signature.
// Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation96. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.
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
class CGameScriptInterfaceBase;class CScriptDataBase;
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
namespace NScript{class CV_MurderTwistScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long MurderTwistItemsFound,MurderTwistState,MurderTwistEvent;std::vector<CScriptThing> MagicRuneRumbleList;CV_MurderTwistScript(CScriptDataBase* data,CGameScriptInterfaceBase& game);virtual void RegisterMain();virtual void Main();virtual void Init();virtual void OnPersist(CPersistContext&);static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);};
CV_MurderTwistScript::CV_MurderTwistScript(CScriptDataBase* data,CGameScriptInterfaceBase& game):CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game){}
}
#ifdef _MSC_VER
typedef char ExactBaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ExactOwnerSize[sizeof(NScript::CV_MurderTwistScript)==96?1:-1];
#endif