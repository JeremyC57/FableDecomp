// std::vector<CArenaCreatureDef>::operator=(const vector&) -- copy nontrivial original script-definition elements using genuine STLport storage.
// Leaf: ??4?$vector@VCArenaCreatureDef@@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Identity: proven; Original CArenaCreatureDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. Retail CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase ancestry (shared headers); CArenaCreatureDef copy ctor 0x00F25DD0. Actual original element56 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes, vtable 0x01230BCC); CArenaCreatureDef
//   (vtable 0x012FB358, no RTTI) keeps its real overrides.
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
namespace std{struct __false_type;template<class T>class allocator{public:T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:vector();vector(const vector&);~vector();vector& operator=(const vector&);unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}unsigned int capacity()const{return (unsigned int)(this->_M_end_of_storage._M_data-this->_M_start);}protected:T* _M_allocate_and_copy(unsigned int,const T*,const T*);void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
enum EFeatAttackType{EFeatAttackType_DescriptiveZero=0};
class CArenaCreatureDef : public CDefClassBase {     // 0x38 bytes; def vtable 0x012FB358 (no RTTI)
public:
    CCharString CreatureType;                           // +0x28
    long        NumCreatures;                           // +0x2c
    CCharString HUDType;                                // +0x30
    long        DeathScore;                             // +0x34
    CArenaCreatureDef(const CArenaCreatureDef&);        // 0x00F25DD0
    CArenaCreatureDef& operator=(const CArenaCreatureDef&);   // 0x00F25F10
                                                        // slot 0  0x00F25E20 (scalar deleting; implicit inline dtor)
    virtual void Transfer(CPersistContext&);            // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00F25F00
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F25E10
};
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T,class U>__forceinline void _Construct(T* p,const U& x){new((void*)p)T(x);}template<class T>__forceinline void _Destroy(T*p){p->~T();}template<class I>__forceinline void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)_Destroy(first);}template<class I,class O>__forceinline O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O cur=result;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}template<class I,class N,class T>__forceinline I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(first,value);return first;}template<class I,class O,class D>__forceinline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class I,class O>__forceinline O __fastcall __copy_ptrs(I first,I last,O result,const __false_type&){return __copy(first,last,result,random_access_iterator_tag(),(int*)0);}template<class T>__forceinline const T& max(const T&a,const T&b){return a<b?b:a;}}
template<class T,class Al>std::vector<T,Al>& std::vector<T,Al>::operator=(const vector& x){if(&x!=this){const unsigned int xlen=x.size();if(xlen>capacity()){T* tmp=_M_allocate_and_copy(xlen,(const T*)x._M_start,(const T*)x._M_finish);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}else if(size()>=xlen){T* i=__copy_ptrs((const T*)x._M_start,(const T*)x._M_finish,this->_M_start,__false_type());__destroy_aux(i,this->_M_finish,__false_type());}else{__copy_ptrs((const T*)x._M_start,(const T*)x._M_start+size(),this->_M_start,__false_type());__uninitialized_copy((const T*)x._M_start+size(),(const T*)x._M_finish,this->_M_finish,__false_type());}this->_M_finish=this->_M_start+xlen;}return *this;}
template std::vector<CArenaCreatureDef>& std::vector<CArenaCreatureDef>::operator=(const std::vector<CArenaCreatureDef>&);
#ifdef _MSC_VER
typedef char ElementSize[sizeof(CArenaCreatureDef)==56?1:-1];
#endif