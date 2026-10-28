// NScript::CQ_HobbeCaveScript::CHobbeTorrent::Init() -- Parse the entity data string into its initial count.
// Leaf: ?Init@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Identity: Own named entity registration00d8dee2 ->factory00d8f3f0 ->vtable012d5960 slot2; original PDB private virtual Init signature and named fields. CScriptThing GetDataString const slot3; GFCharStringToInt fastcall0099e7f0; CCharString destructor0099eae0.
// Layout: real CEntityScriptBase20 with Game4 and CScriptThing Me8; ParentClass20, PMasterData24; original field order. CCharString is a four-byte pointer to CCharStringData, not a CBasicString base. Inactive virtual slots are descriptive projections. Shared handle header aliased to restore real polymorphic inheritance.
#include <stddef.h>
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#ifndef _MSC_VER
#define __fastcall
#endif
class CBaseClass{public:virtual ~CBaseClass(){}};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing> PImp;virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual CCharString GetDataString()const;};
class CGameScriptInterfaceBase;class CPersistContext;
class CEntityScriptBase{public:CGameScriptInterfaceBase* Game;CScriptThing Me;virtual ~CEntityScriptBase(){}virtual void UnknownEntitySlot1(){}virtual void Init(){}virtual void UnknownEntitySlot3(){}virtual void OnPersist(CPersistContext&){}virtual void OnPredicateFail(){}};
namespace NScript{class CQ_SunnyvaleMasterData;}
long __fastcall GFCharStringToInt(const CCharString&);
namespace NScript{class CQ_HobbeCaveScript{public:class CHobbeTorrent:public CEntityScriptBase{public:CQ_HobbeCaveScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;long NumberToGenerate;private:virtual void Init();};};void CQ_HobbeCaveScript::CHobbeTorrent::Init(){NumberToGenerate=GFCharStringToInt(Me.GetDataString());}}
#ifdef _MSC_VER
typedef char EntityBaseSize[sizeof(CEntityScriptBase)==20?1:-1];typedef char TargetOffset[offsetof(NScript::CQ_HobbeCaveScript::CHobbeTorrent,NumberToGenerate)==28?1:-1];
#endif