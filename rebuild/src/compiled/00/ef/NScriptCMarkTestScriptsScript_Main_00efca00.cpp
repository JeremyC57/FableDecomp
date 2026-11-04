// NScript::CMarkTestScriptsScript::Main() -- register three original entity factories then finish script binding setup.
// Leaf: ?Main@CMarkTestScriptsScript@NScript@@UAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own literals/factory pointers TestMan->00efcb70,Gift->00efd680,ShopKeeper->00efd9c0 match original PDB nested owner names/static Alloc prototypes. Actual game PostAddScriptedEntities slot64. Original base data CQ_SunnyvaleMasterData comes from nested ParentClass/PMasterData donor fields, not donor addresses.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Layout: accepted main N230 genuine binding28: abstract interface slot0 GetEntityScriptName/slot1 CreateActiveEntityScript (no destructor), CCharString+4,parent+8,memberfunctionpointer+12,fastcall class-return factory+16,bool20,Flags24. CCountedPointer explicitly non-POD copy/dtor identity retained. Full typed CScriptBase64/level72; unused parent suffix explicitly omitted, nested entity20 +original parent/master fields. Correct CCharString ctor(constchar*,long). Real compiler new-expression temporary lifetime; no fake allocation/helper callback. Shared handles aliased.
// Naming wave 35: identity outer script vtable 012f6d00 slot 2 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00efca00 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CMarkTestScriptsScript; PDB ?Main@CMarkTestScriptsScript@NScript@@; refs=1; replaces NScript::CMarkTestScriptsScript::RegisterMain: current name also held by 00efc960; CMarkTestScriptsScript::RegisterMain is at 00efc960 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Main@CMarkTestScriptsScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?RegisterMain@CMarkTestScriptsScript@NScript@@UAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();bool FastCompare(const CCharString&)const;};
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
class CScriptDataBase;class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CScriptDataBase;
class CEntityScriptBindingBase{public:virtual const CCharString& GetEntityScriptName()const=0;protected:virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&,RetailUID)=0;};
template<class T>class CEntityScriptBinding:public CEntityScriptBindingBase{public:typedef CCountedPointer<CEntityScriptBase>(__fastcall *TAllocFunc)(T&,CScriptDataBase*,const CScriptThing&);CCharString EntityScriptName;T* PParentScript;void(T::*Func)(CScriptThing);TAllocFunc ClassAllocFunc;bool ClassAllocValid;unsigned long Flags;CEntityScriptBinding(const CCharString& name,T* parent,TAllocFunc alloc,unsigned long flags):EntityScriptName(name),PParentScript(parent),ClassAllocFunc(alloc),ClassAllocValid(true),Flags(flags){}virtual const CCharString& GetEntityScriptName()const{return EntityScriptName;}protected:virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&,RetailUID);};
class CGameScriptInterfaceBase:public CBaseClassNonCopyable{public:virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void UnknownSlot7(){}virtual void UnknownSlot8(){}virtual void UnknownSlot9(){}virtual void UnknownSlot10(){}virtual void UnknownSlot11(){}virtual void UnknownSlot12(){}virtual void UnknownSlot13(){}virtual void UnknownSlot14(){}virtual void UnknownSlot15(){}virtual void UnknownSlot16(){}virtual void UnknownSlot17(){}virtual void UnknownSlot18(){}virtual void UnknownSlot19(){}virtual void UnknownSlot20(){}virtual void UnknownSlot21(){}virtual void UnknownSlot22(){}virtual void UnknownSlot23(){}virtual void UnknownSlot24(){}virtual void UnknownSlot25(){}virtual void UnknownSlot26(){}virtual void UnknownSlot27(){}virtual void UnknownSlot28(){}virtual void UnknownSlot29(){}virtual void UnknownSlot30(){}virtual void UnknownSlot31(){}virtual void UnknownSlot32(){}virtual void UnknownSlot33(){}virtual void UnknownSlot34(){}virtual void UnknownSlot35(){}virtual void UnknownSlot36(){}virtual void UnknownSlot37(){}virtual void UnknownSlot38(){}virtual void UnknownSlot39(){}virtual void UnknownSlot40(){}virtual void UnknownSlot41(){}virtual void UnknownSlot42(){}virtual void UnknownSlot43(){}virtual void UnknownSlot44(){}virtual void UnknownSlot45(){}virtual void UnknownSlot46(){}virtual void UnknownSlot47(){}virtual void UnknownSlot48(){}virtual void UnknownSlot49(){}virtual void UnknownSlot50(){}virtual void UnknownSlot51(){}virtual void UnknownSlot52(){}virtual void UnknownSlot53(){}virtual void UnknownSlot54(){}virtual void UnknownSlot55(){}virtual void UnknownSlot56(){}virtual void UnknownSlot57(){}virtual void UnknownSlot58(){}virtual void UnknownSlot59(){}virtual void UnknownSlot60(){}virtual void UnknownSlot61(){}virtual void UnknownSlot62(){}virtual void UnknownSlot63(){}virtual void PostAddScriptedEntities()const=0;};
namespace NScript{class CQ_SunnyvaleMasterData;class CMarkTestScriptsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {public:CMarkTestScriptsScript(CScriptDataBase*,CGameScriptInterfaceBase&);class CTestMan:public CEntityScriptBase{public:CMarkTestScriptsScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CMarkTestScriptsScript&,CScriptDataBase*,const CScriptThing&);};class CGift:public CEntityScriptBase{public:CMarkTestScriptsScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CMarkTestScriptsScript&,CScriptDataBase*,const CScriptThing&);};class CShopKeeper:public CEntityScriptBase{public:CMarkTestScriptsScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CMarkTestScriptsScript&,CScriptDataBase*,const CScriptThing&);};
public:
    virtual void Main();                          // PDB ?Main@CMarkTestScriptsScript@NScript@@UAEXXZ
};void NScript::CMarkTestScriptsScript::Main(){AddEntityScriptBinding(new CEntityScriptBinding<CMarkTestScriptsScript>("TestMan",this,CTestMan::Alloc,0));AddEntityScriptBinding(new CEntityScriptBinding<CMarkTestScriptsScript>("Gift",this,CGift::Alloc,0));AddEntityScriptBinding(new CEntityScriptBinding<CMarkTestScriptsScript>("ShopKeeper",this,CShopKeeper::Alloc,0));PostAddScriptedEntities();Game->PostAddScriptedEntities();}}