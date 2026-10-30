// std::__copy_backward_ptrs<CHeroSoulsWaveDef*,CHeroSoulsWaveDef*>(first,last,result,falseTag) -- move-assign a range backward and return its destination start.
// Leaf: ??$__copy_backward_ptrs@PAVCHeroSoulsWaveDef@@PAV1@@std@@YIPAVCHeroSoulsWaveDef@@PAV1@00ABU__false_type@0@@Z
// Identity: proven; own _M_fill_insert callers pass result+__false_type (ret8), genuine STLport pointer dispatch around inlined random-access __copy_backward; same-template landed CFireHeartPatternDef twin00f34110. Record identity comes from its actual private wrapper/Transfer fields, not propagated catalog CThingBuildingDef labels.
// Layout: original DefClass37 full CIV/Resource/DefPointee chain, original scalar40/vector44, true typed STLport vector/base/proxy; actual compiler-generated memberwise operator= invokes original base00431f10 and typed vector assignment. Unknown unused definition slots2–17 explicit, Transfer18/Copy19. Shared flattened headers retained as evidence.
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
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CHeroSoulsCreatureDef;class CHeroSoulsWaveDef:public CDefClassBase{public:long NumWaveCreatures;std::vector<CHeroSoulsCreatureDef> Creatures;CHeroSoulsWaveDef();CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);virtual ~CHeroSoulsWaveDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};   // def vtable 0x012FB498 (no RTTI): slot 0 0x00F26F40 (implicit inline dtor), Transfer 0x00F27490, Copy 0x00F271D0, GetSizeofClass 0x00F26F30
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class I,class O,class D>__forceinline O __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}template<class I,class O>O __fastcall __copy_backward_ptrs(I first,I last,O result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
template CHeroSoulsWaveDef* __fastcall std::__copy_backward_ptrs(CHeroSoulsWaveDef*,CHeroSoulsWaveDef*,CHeroSoulsWaveDef*,const std::__false_type&);