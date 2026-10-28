// CTCSimpleAppearanceMorph::CTCSimpleAppearanceMorph(CThing& thing) -- initialize the component and its empty state.
// Leaf: ??0CTCSimpleAppearanceMorph@@QAE@AAVCThing@@@Z
// Identity: proven; own vtable RTTI CTCSimpleAppearanceMorph; PDB PMeshMorphSet+12 is8-byte counted pointer; own CThing& ctor factory0x4D2A14; base CTCBase(CThing&)0x686800 and own vtable0x012664a4.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Layout: no target-class engine header; generated CTCBase header uses a data-member vptr, so the real polymorphic base chain is required to retain the own vtable and member+12.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
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
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object){if(Object)Object->AddRef();}__forceinline ~CIVCountedPointer(){if(Object){Object->Release();Object=0;}}CIVCountedPointer& operator=(const CIVCountedPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
class CThing;class CGameDefinitionManager;class CTCBase:public CBaseClassNonCopyable{public:CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();virtual void UnknownCTCSlot1(){}virtual void UnknownCTCSlot2(){}virtual void UnknownCTCSlot3(){}virtual void OnCreate();protected:const CGameDefinitionManager& GetDefinitionManager()const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}~CCountedPointer();};
class CTCAppearanceMorphBase:public CTCBase{public:class CMeshMorphSet;CTCAppearanceMorphBase(CThing& thing):CTCBase(thing){}};class CTCSimpleAppearanceMorph:public CTCAppearanceMorphBase{public:CCountedPointer<CMeshMorphSet> PMeshMorphSet;CTCSimpleAppearanceMorph(CThing&);virtual void OnCreate();};CTCSimpleAppearanceMorph::CTCSimpleAppearanceMorph(CThing& thing):CTCAppearanceMorphBase(thing){}

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
typedef char AuditSize_1[(sizeof(CTCBase)==12)?1:-1];
typedef char AuditSize_2[(sizeof(CTCAppearanceMorphBase)==12)?1:-1];
typedef char AuditSize_3[(sizeof(CTCSimpleAppearanceMorph)==20)?1:-1];
#endif