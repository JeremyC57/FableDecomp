// CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::Transfer(...) -- transfer the indexed sub-definition through its original virtual API.
// Leaf: ?Transfer@?$CPersistContext_TransferableVectorOfSubComponents@VCHeroSoulsCreatureDef@@@@EAEXAAVCPersistContext@@J@Z
// Identity: proven; original PDB private virtual EAE signature, actual wrapper vtables with Size/Resize/AreDefaultValsApplied/SetDefaultValsApplied/Transfer and TransVector+4. Own typed Transfer literals CreatureType/HUDType vs CreatureType/Location vs FeatName establish element identities alongside donor named fields. Retail strides56/52/100 reconcile donor vector differences.
// Layout: true CIV/CResource/CDefPointee/CDefClassBase hierarchy, CDefClassBase 0x25 packed (FABLE_RETAIL_DEF_CLASS_BASE shared header) and flags +0x24; original typed derived fields, real STLport vector/_Vector_base/proxy. No target generated header exists. Definition slots 0-24 come from the shared header (Transfer slot 18). Wrapper generic base has no virtual destructor (five-entry own tables). No foreign original type is replaced by a byte buffer.
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
enum ENullDef{ENullDef_DescriptiveNull=0};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CHeroSoulsCreatureDef:public CDefClassBase{public:CCharString CreatureType;CCharString Location;long NumCreatures;CHeroSoulsCreatureDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};   // def vtable 0x012FB500 (no RTTI): slot 0 0x00F270B0 (implicit inline dtor), Transfer 0x00F27100, Copy 0x00F27180, GetSizeofClass 0x00F270A0
class CPersistContext_TransferableVectorOfSubComponentsGenericType{public:virtual long Size()=0;virtual void Resize(long)=0;virtual bool AreDefaultValsApplied(long)=0;virtual void SetDefaultValsApplied(long,bool)=0;virtual void Transfer(CPersistContext&,long)=0;};template<class T>class CPersistContext_TransferableVectorOfSubComponents:public CPersistContext_TransferableVectorOfSubComponentsGenericType{std::vector<T>* TransVector;public:CPersistContext_TransferableVectorOfSubComponents(std::vector<T>& v):TransVector(&v){}private:virtual long Size();virtual void Resize(long);virtual bool AreDefaultValsApplied(long);virtual void SetDefaultValsApplied(long,bool);virtual void Transfer(CPersistContext&,long);};
template<class T>void CPersistContext_TransferableVectorOfSubComponents<T>::Transfer(CPersistContext& context,long index){(*TransVector)[index].Transfer(context);}
template void CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::Transfer(CPersistContext& context,long index);

#ifdef _MSC_VER
typedef char AuditDef[(sizeof(CDefClassBase)==37)?1:-1];typedef char AuditRecord[(sizeof(CHeroSoulsCreatureDef)==52)?1:-1];
#endif