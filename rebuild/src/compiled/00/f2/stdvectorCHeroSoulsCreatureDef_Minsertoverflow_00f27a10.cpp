// std::vector<CHeroSoulsCreatureDef>::_M_insert_overflow(position,value,tag,count,atEnd) -- copy nontrivial original script-definition elements using genuine STLport storage.
// Leaf: ?_M_insert_overflow@?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@IAEXPAVCHeroSoulsCreatureDef@@ABV3@ABU__false_type@2@I_N@Z
// Identity: proven; Original CHeroSoulsCreatureDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase retail ancestry from the shared FABLE_RETAIL_DEF_CLASS_BASE headers (CDefClassBase 0x25 packed, vtable 0x01230BCC). Actual original element52 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
// Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.
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
namespace std{struct __false_type;template<class T>class allocator{public:T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:vector();vector(const vector&);~vector();vector& operator=(const vector&);unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}unsigned int capacity()const{return (unsigned int)(this->_M_end_of_storage._M_data-this->_M_start);}protected:T* _M_allocate_and_copy(unsigned int,const T*,const T*);void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
enum EFeatAttackType{EFeatAttackType_DescriptiveZero=0};
class CHeroSoulsCreatureDef:public CDefClassBase{public:CCharString CreatureType;CCharString Location;long NumCreatures;CHeroSoulsCreatureDef();CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&);CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef&);virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};   // def vtable 0x012FB500 (no RTTI): slot 0 0x00F270B0 (implicit inline dtor), Transfer 0x00F27100, Copy 0x00F27180, GetSizeofClass 0x00F270A0
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T,class U>__forceinline void _Construct(T* p,const U& x){new((void*)p)T(x);}template<class T>__forceinline void _Destroy(T*p){p->~T();}template<class I>__forceinline void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(first);}template<class I,class O>__forceinline O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O cur=result;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}template<class I,class N,class T>__forceinline I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(first,value);return first;}template<class I,class O,class D>__forceinline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class I,class O>__forceinline O __fastcall __copy_ptrs(I first,I last,O result,const __false_type&){return __copy(first,last,result,random_access_iterator_tag(),(int*)0);}template<class T>__forceinline const T& max(const T&a,const T&b){return a<b?b:a;}}
template<class T,class Al>void std::vector<T,Al>::_M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int n,bool atEnd){const unsigned int oldSize=size();const unsigned int length=oldSize+max(oldSize,n);T* start=this->_M_end_of_storage.allocate(length);T* finish=__uninitialized_copy(this->_M_start,position,start,__false_type());if(n==1){new((void*)finish)T(value);++finish;}else finish=__uninitialized_fill_n(finish,n,value,__false_type());if(!atEnd)finish=__uninitialized_copy(position,this->_M_finish,finish,__false_type());__destroy_aux(this->_M_start,this->_M_finish,__false_type());this->_M_end_of_storage.deallocate(this->_M_start,capacity());this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+length;}
template void std::vector<CHeroSoulsCreatureDef>::_M_insert_overflow(CHeroSoulsCreatureDef*,const CHeroSoulsCreatureDef&,const std::__false_type&,unsigned int,bool);
#ifdef _MSC_VER
typedef char ElementSize[sizeof(CHeroSoulsCreatureDef)==52?1:-1];
#endif