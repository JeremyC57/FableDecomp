// std::__copy_backward_ptrs<CHeroSoulsRoundDef*,CHeroSoulsRoundDef*>(first,last,result,falseTag) -- move-assign a range backward and return its destination start.
// Leaf: ??$__copy_backward_ptrs@PAVCHeroSoulsRoundDef@@PAV1@@std@@YIPAVCHeroSoulsRoundDef@@PAV1@00ABU__false_type@0@@Z
// Identity: proven; own _M_fill_insert callers pass result+__false_type (ret8), genuine STLport pointer dispatch around inlined random-access __copy_backward; same-template landed CFireHeartPatternDef twin00f34110. Record identity comes from its actual private wrapper/Transfer fields, not propagated catalog CThingBuildingDef labels.
// Layout: original DefClass37 full CIV/Resource/DefPointee chain, original scalar40/vector44, true typed STLport vector/base/proxy; actual compiler-generated memberwise operator= invokes original base00431f10 and typed vector assignment. Unknown unused definition slots2–17 explicit, Transfer18/Copy19. Shared flattened headers retained as evidence.
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
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CHeroSoulsRoundDef;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual void UnknownDefinitionSlot14(){}virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
#pragma pack(pop)
class CHeroSoulsWaveDef;class CHeroSoulsRoundDef:public CDefClassBase{public:long NumWaves;std::vector<CHeroSoulsWaveDef> Waves;CHeroSoulsRoundDef();CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);virtual ~CHeroSoulsRoundDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class I,class O,class D>__forceinline O __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}template<class I,class O>O __fastcall __copy_backward_ptrs(I first,I last,O result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
template CHeroSoulsRoundDef* __fastcall std::__copy_backward_ptrs(CHeroSoulsRoundDef*,CHeroSoulsRoundDef*,CHeroSoulsRoundDef*,const std::__false_type&);