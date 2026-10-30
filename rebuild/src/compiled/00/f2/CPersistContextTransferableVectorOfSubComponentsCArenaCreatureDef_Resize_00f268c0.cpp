// CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::Resize(long) -- resize with an original default-constructed sub-definition.
// Leaf: ?Resize@?$CPersistContext_TransferableVectorOfSubComponents@VCArenaCreatureDef@@@@EAEXJ@Z
// Identity: proven original privatevirtual Resize(long), five-entry generic wrapper vtable. Own temporary constructor00f263d0, destructor00f25e40, typed vector resize00f26490; constructor fields/literals reconcile original donor records and retail size56.
// Layout: original DefClass37/CDefPointee/Resource/CIV base chain and named typed fields; genuine STLport vector base/proxy, original constructors/destructors and real vector resize API. Unknown unused definition slots2–17 are explicit projections, Transfer18. Available shared flattened base headers are referenced in accompanying model evidence.
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
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:void resize(unsigned int,const T&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CArenaCreatureDef:public CDefClassBase{public:CCharString CreatureType;long NumCreatures;CCharString HUDType;long DeathScore;CArenaCreatureDef();virtual ~CArenaCreatureDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};   // def vtable 0x012FB358 (no RTTI): slot 0 0x00F25E20 (implicit inline dtor), Transfer 0x00F25E70, Copy 0x00F25F00, GetSizeofClass 0x00F25E10
class CPersistContext_TransferableVectorOfSubComponentsGenericType{public:virtual long Size()=0;virtual void Resize(long)=0;virtual bool AreDefaultValsApplied(long)=0;virtual void SetDefaultValsApplied(long,bool)=0;virtual void Transfer(CPersistContext&,long)=0;};template<class T>class CPersistContext_TransferableVectorOfSubComponents:public CPersistContext_TransferableVectorOfSubComponentsGenericType{std::vector<T>* TransVector;public:CPersistContext_TransferableVectorOfSubComponents(std::vector<T>& v):TransVector(&v){}private:virtual long Size();virtual void Resize(long);virtual bool AreDefaultValsApplied(long);virtual void SetDefaultValsApplied(long,bool);virtual void Transfer(CPersistContext&,long);};
template<class T>void CPersistContext_TransferableVectorOfSubComponents<T>::Resize(long n){std::vector<T>* vector=TransVector;vector->resize(n,T());}
template void CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::Resize(long);
#ifdef _MSC_VER
typedef char RecordSize[(sizeof(CArenaCreatureDef)==56)?1:-1];
#endif