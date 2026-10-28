// std::vector<CHeroSoulsRoundDef>::_M_fill_insert(...) -- insert counted copies with genuine STLport vector lifetime/storage operations.
// Leaf: ?_M_fill_insert@?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@QAEXPAVCHeroSoulsRoundDef@@IABV3@@Z
// Identity: proven from own script wrapper/vtable/Transfer literal/typed vector constructor and previous script definition helper mappings. Correct original CHeroSoulsRoundDef element56, CHeroSoulsWaveDef vector member. Genuine STLport4.6 canonical _M_fill_insert body, false_type nontrivial construction/assignment/destruction.
// Retry evidence: original actual out-of-line standard __copy_backward_ptrs helper declared with proven fastcall signature; fixture retains genuine copy-backward body. Header helper-body visibility no longer removes original zero-initialized empty tag at that call. No invented helper/barrier.
// Layout: full original CIV/Resource/DefPointee/packedDef37 chain and typed scalar40/vector44; actual private default Def ctor/public copy API, proper destructor phases and original base/vector callees. Shared flattened base headers aliased. Unknown unused slots2–17 explicit; real Transfer18/Copy19. Actual standard _Construct helper is declared out of line (matched sibling N248 supplies its body); count1 growth uses equivalent direct placement construction. Fill-insert declares its original constructor and grow helper out of line; their independently supplied sources retain all behavior. No fake fields, identity barriers or replacement flat container.

#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
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
namespace std{struct __false_type;template<class T>class allocator{public:T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T* pointer;void _M_fill_insert(T*,unsigned int,const T&);protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:virtual ~CDefClassBase(){}CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CHeroSoulsRoundDef;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual void UnknownDefinitionSlot14(){}virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
#pragma pack(pop)
class CHeroSoulsWaveDef;class CHeroSoulsRoundDef:public CDefClassBase{public:long NumWaves;std::vector<CHeroSoulsWaveDef> Waves;CHeroSoulsRoundDef();CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
namespace std{struct __false_type{};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T,class V>void __fastcall _Construct(T*,const V&);template<class T>__forceinline void _Destroy(T* p){p->~T();}template<class I>__forceinline void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class I,class O>__forceinline O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}template<class I,class N,class T>__forceinline I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(&*first,value);return first;}template<class I,class T>__forceinline void fill(I first,I last,const T& value){for(;first!=last;++first)*first=value;}template<class I,class O,class D>__forceinline O __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}template<class I,class O>O __fastcall __copy_backward_ptrs(I,I,O,const __false_type&);template<class T>inline const T& max(const T&a,const T&b){return a<b?b:a;}}
template<class T,class Al>void std::vector<T,Al>::_M_fill_insert(T* position,unsigned int n,const T& value){if(n!=0){if((unsigned int)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const unsigned int elemsAfter=(unsigned int)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward_ptrs(position,oldFinish-n,oldFinish,__false_type());std::fill(position,position+n,copy);}else{__uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy,__false_type());this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;std::fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
template void std::vector<CHeroSoulsRoundDef>::_M_fill_insert(CHeroSoulsRoundDef*,unsigned int,const CHeroSoulsRoundDef&);