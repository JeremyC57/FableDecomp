// std::vector<CHeroSoulsWaveDef>::_M_fill_insert(CHeroSoulsWaveDef* position, unsigned int n, const CHeroSoulsWaveDef& value) -- insert n copies of a hero-souls wave definition before position (STLport 4.6 _vector.c, non-trivial element)
// Leaf: ?_M_fill_insert@?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAEXPAVCHeroSoulsWaveDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F28990, 522 bytes, thiscall `ret 0Ch`.
// Identity: byte twin of the landed 0x00F34D70 vector<CArenaRoundDef>::_M_fill_insert; every differing relocation names the HeroSouls
//   types: _Construct<CHeroSoulsWaveDef> 0x00F26F00 (bound ??$_Construct@VCHeroSoulsWaveDef@@V1@@std@@...), vector<CHeroSoulsCreatureDef>
//   operator= 0x00F27200 and ~vector 0x00F26F70 (bound), CHeroSoulsWaveDef copy ctor 0x00F28BA0, __copy_backward_ptrs<CHeroSoulsWaveDef*>
//   0x00F28D30 and _M_insert_overflow 0x00F28BD0 (catalog); CDefClassBase::operator= 0x00431F10 and ~CResource 0x009FC550 are shared.
//   Ego_r layouts: CHeroSoulsWaveDef = CDefClassBase + long NumWaveCreatures + vector<CHeroSoulsCreatureDef> Creatures, the same shape as
//   CArenaRoundDef (NumWaves, vector<CArenaWaveDef> Waves); the PDB name sets hold ??0CHeroSoulsWaveDef@@QAE@ABV0@@Z and
//   ??4CHeroSoulsWaveDef@@QAEAAV0@ABV0@@Z. Catalog label `vector_insert_impl_CThingBuildingDef` is wrong.
// Shape: the twin's TU with the element type swapped (another explicit instantiation of the same STLport member).

#include "fable_compiler.h"
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
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
namespace std{struct __false_type;template<class T>class allocator{public:T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T* pointer;void _M_fill_insert(T*,unsigned int,const T&);protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CHeroSoulsCreatureDef;class CHeroSoulsWaveDef:public CDefClassBase{public:long NumWaveCreatures;std::vector<CHeroSoulsCreatureDef> Creatures;CHeroSoulsWaveDef();CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};
namespace std{struct __false_type{};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T,class V>void __fastcall _Construct(T*,const V&);template<class T>FABLE_FORCEINLINE void _Destroy(T* p){p->~T();}template<class I>FABLE_FORCEINLINE void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class I,class O>FABLE_FORCEINLINE O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}template<class I,class N,class T>FABLE_FORCEINLINE I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(&*first,value);return first;}template<class I,class T>FABLE_FORCEINLINE void fill(I first,I last,const T& value){for(;first!=last;++first)*first=value;}template<class I,class O,class D>FABLE_FORCEINLINE O __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}template<class I,class O>O __fastcall __copy_backward_ptrs(I,I,O,const __false_type&);template<class T>inline const T& max(const T&a,const T&b){return a<b?b:a;}}
template<class T,class Al>void std::vector<T,Al>::_M_fill_insert(T* position,unsigned int n,const T& value){if(n!=0){if((unsigned int)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const unsigned int elemsAfter=(unsigned int)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward_ptrs(position,oldFinish-n,oldFinish,__false_type());std::fill(position,position+n,copy);}else{__uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy,__false_type());this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;std::fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
template void std::vector<CHeroSoulsWaveDef>::_M_fill_insert(CHeroSoulsWaveDef*,unsigned int,const CHeroSoulsWaveDef&);