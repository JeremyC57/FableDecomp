// CTCSoundAtmosphere::CTCSoundAtmosphere(CThing& thing) -- initialize the component and its empty state.
// Leaf: ??0CTCSoundAtmosphere@@QAE@AAVCThing@@@Z
// Identity: proven; own vtable RTTI CTCSoundAtmosphere; PDB TimeUntilNextUpdate+12 and PDef+16; own factory0x4DCE57; base CTCBase(CThing&)0x686800 and own vtable0x01271dfc.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Layout: no target-class engine header; generated CTCBase header uses a data-member vptr, so the real polymorphic base chain is required to retain the own vtable and member+12.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CTCBase CTCBase_HeaderEvidence
#include "engine/CTCBase.h"
#undef CTCBase
#define CBaseObject CBaseObject_HeaderEvidence
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass CClass_HeaderEvidence
#include "engine/CClass.h"
#undef CClass
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer(ENullDef=ENullDef_DescriptiveNull):Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}CDefPointer(T* object):Object((CDefPointeeBase*)object){if(Object)Object->AddRef();}__forceinline ~CDefPointer(){if(Object){Object->Release();Object=0;}}CDefPointer& operator=(const CDefPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
class CThing;class CGameDefinitionManager;class CTCBase:public CBaseClassNonCopyable{public:CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();virtual void UnknownCTCSlot1(){}virtual void UnknownCTCSlot2(){}virtual void UnknownCTCSlot3(){}virtual void OnCreate();protected:const CGameDefinitionManager& GetDefinitionManager()const;};
class CSoundAtmospheresDef;class CTCSoundAtmosphere:public CTCBase{public:float TimeUntilNextUpdate;CDefPointer<const CSoundAtmospheresDef> PDef;CTCSoundAtmosphere(CThing&);};CTCSoundAtmosphere::CTCSoundAtmosphere(CThing& thing):CTCBase(thing){TimeUntilNextUpdate=0.0f;}

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
typedef char AuditSize_1[(sizeof(CTCBase)==12)?1:-1];
typedef char AuditSize_2[(sizeof(CTCSoundAtmosphere)==20)?1:-1];
#endif