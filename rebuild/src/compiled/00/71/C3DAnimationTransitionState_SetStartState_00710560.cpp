// C3DAnimationTransitionState::SetStartState(const Pointer& value) -- replace the owned counted pointer.
// Leaf: ?SetStartState@C3DAnimationTransitionState@@QAEXABV?$CIVCountedPointer@VC3DAnimationStateBase@@@@@Z
// Identity: proven; PDB SetStartState(CIVCountedPointer<C3DAnimationStateBase>const&); own +8 is StartState not propagated EndState+12; CalcInterpolatedMeshAnim0x70D940 callers.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Layout: shared transition header reused, including its named flattened StartState_Object; counts accessed through real CIVCountedPointeeBase.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define C3DAnimationTransitionState C3DAnimationTransitionState_HeaderEvidence
#include "engine/C3DAnimationTransitionState.h"
#undef C3DAnimationTransitionState
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
class C3DAnimationStateBase:public CIVCountedPointeeBase{};class C3DAnimationTransitionState:public C3DAnimationStateBase{public:CIVCountedPointer<C3DAnimationStateBase> StartState,EndState;float Progress;C3DAnimationTransitionState(const CIVCountedPointer<C3DAnimationStateBase>&,const CIVCountedPointer<C3DAnimationStateBase>&,float);void SetStartState(const CIVCountedPointer<C3DAnimationStateBase>&);};void C3DAnimationTransitionState::SetStartState(const CIVCountedPointer<C3DAnimationStateBase>& value){StartState=value;}

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
typedef char AuditSize_1[(sizeof(C3DAnimationStateBase)==8)?1:-1];
typedef char AuditSize_2[(sizeof(C3DAnimationTransitionState)==20)?1:-1];
#endif