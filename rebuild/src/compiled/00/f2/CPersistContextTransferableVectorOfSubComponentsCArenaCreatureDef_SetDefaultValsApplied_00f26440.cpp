// CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::SetDefaultValsApplied(...) -- set only the sub-definition default flag.
// Leaf: ?SetDefaultValsApplied@?$CPersistContext_TransferableVectorOfSubComponents@VCArenaCreatureDef@@@@EAEXJ_N@Z
// Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
// Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, packed Def37 and flags+36; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18. Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBase_HeaderEvidence
#include "engine/CDefClassBase.h"
#undef CDefClassBase
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
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CArenaCreatureDef;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual void UnknownDefinitionSlot14(){}virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);};
#pragma pack(pop)
class CArenaCreatureDef:public CDefClassBase{public:CCharString CreatureType;long NumCreatures;CCharString HUDType;long DeathScore;CArenaCreatureDef();virtual void Transfer(CPersistContext&);};
class CPersistContext_TransferableVectorOfSubComponentsGenericType{public:virtual long Size()=0;virtual void Resize(long)=0;virtual bool AreDefaultValsApplied(long)=0;virtual void SetDefaultValsApplied(long,bool)=0;virtual void Transfer(CPersistContext&,long)=0;};template<class T>class CPersistContext_TransferableVectorOfSubComponents:public CPersistContext_TransferableVectorOfSubComponentsGenericType{std::vector<T>* TransVector;public:CPersistContext_TransferableVectorOfSubComponents(std::vector<T>& v):TransVector(&v){}private:virtual long Size();virtual void Resize(long);virtual bool AreDefaultValsApplied(long);virtual void SetDefaultValsApplied(long,bool);virtual void Transfer(CPersistContext&,long);};
template<class T>void CPersistContext_TransferableVectorOfSubComponents<T>::SetDefaultValsApplied(long index,bool value){(*TransVector)[index].SetDefaultValsApplied(value);}
template void CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::SetDefaultValsApplied(long index,bool value);

#ifdef _MSC_VER
typedef char AuditDef[(sizeof(CDefClassBase)==37)?1:-1];typedef char AuditRecord[(sizeof(CArenaCreatureDef)==56)?1:-1];
#endif