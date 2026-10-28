// std::vector<CHeroSoulsRoundDef>::operator=(const vector&) -- copy nontrivial original script-definition elements using genuine STLport storage.
// Leaf: ??4?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Identity: proven; Original CHeroSoulsRoundDef type proven by own virtual Transfer literals/typed wrapper callers, original PDB namesets and original member record layout. Catalog Conversation/SymbolInfo names false. True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor. Actual original element56 and genuine STLport allocator/vector protectedbase/endproxy. Standard algorithm helper bodies/declarations reflect actual separate codegen at own targets, no invented barrier.
// Layout: original full typed element and base chain, incomplete unused nested vector element declaration only.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBase_HeaderEvidence
#include "engine/CDefClassBase.h"
#undef CDefClassBase
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
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
namespace std{struct __false_type;template<class T>class allocator{public:T* allocate(unsigned int n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:vector();vector(const vector&);~vector();vector& operator=(const vector&);unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}unsigned int capacity()const{return (unsigned int)(this->_M_end_of_storage._M_data-this->_M_start);}protected:T* _M_allocate_and_copy(unsigned int,const T*,const T*);void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool=false);};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CHeroSoulsRoundDef;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual void UnknownDefinitionSlot14(){}virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
#pragma pack(pop)
class CHeroSoulsWaveDef;enum EFeatAttackType{EFeatAttackType_DescriptiveZero=0};
class CHeroSoulsRoundDef:public CDefClassBase{public:long NumWaves;std::vector<CHeroSoulsWaveDef> Waves;CHeroSoulsRoundDef();CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);CHeroSoulsRoundDef& operator=(const CHeroSoulsRoundDef&);virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
namespace std{struct __false_type{};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T,class U>void __fastcall _Construct(T*,const U&);template<class T>__forceinline void _Destroy(T*p){p->~T();}template<class I>void __fastcall __destroy_aux(I,I,const __false_type&);template<class I,class O>__forceinline O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O cur=result;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}template<class I,class N,class T>__forceinline I __fastcall __uninitialized_fill_n(I first,N n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(first,value);return first;}template<class I,class O,class D>__forceinline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class I,class O>O __fastcall __copy_ptrs(I,I,O,const __false_type&);template<class T>__forceinline const T& max(const T&a,const T&b){return a<b?b:a;}}
namespace std{template<class I>void __fastcall _Destroy(I,I);}template<class T,class Al>std::vector<T,Al>& std::vector<T,Al>::operator=(const vector& x){if(&x!=this){const unsigned int xlen=x.size();if(xlen>capacity()){T* tmp=_M_allocate_and_copy(xlen,(const T*)x._M_start,(const T*)x._M_finish);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}else if(size()>=xlen){T* i=__copy_ptrs((const T*)x._M_start,(const T*)x._M_finish,this->_M_start,__false_type());_Destroy(i,this->_M_finish);}else{__copy_ptrs((const T*)x._M_start,(const T*)x._M_start+size(),this->_M_start,__false_type());__uninitialized_copy((const T*)x._M_start+size(),(const T*)x._M_finish,this->_M_finish,__false_type());}this->_M_finish=this->_M_start+xlen;}return *this;}
template std::vector<CHeroSoulsRoundDef>& std::vector<CHeroSoulsRoundDef>::operator=(const std::vector<CHeroSoulsRoundDef>&);
#ifdef _MSC_VER
typedef char ElementSize[sizeof(CHeroSoulsRoundDef)==56?1:-1];
#endif