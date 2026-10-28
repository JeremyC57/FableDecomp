// std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_fill_insert(pointer,unsigned int,const value_type&) -- Insert copied records with spare-capacity reuse or growth.
// Leaf: ?_M_fill_insert@?$vector@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@V?$allocator@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@@std@@@std@@QAEXPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@IABV34@@Z
// Flags: /Gr
// Identity: proven. Own RTTI CEngineInternalPrimitiveSpriteTrail::Update00bcbc00 invokes target for Trail104, same growth00bcbf80/copy00bca370; original nested CSpriteTrailEntry classV Position+Width. Memberwise user copy constructors and inline uninitialized_fill_n preserve the sixteen-byte local and placement-new null guard; exact original STLport typed helpers.
// Layout: element16 and genuine vector12/base/proxy. No target engine header.
#include <stdlib.h>
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
inline void* __cdecl operator new(PlacementSize,void* p){return p;}
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z){}};class CEngineInternalPrimitiveSpriteTrail{public:class CSpriteTrailEntry{public:C3DVector Position;float Width;CSpriteTrailEntry():Width(0){}CSpriteTrailEntry(const CSpriteTrailEntry& p):Position(p.Position),Width(p.Width){}};};
namespace std{
struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};
template<class T>class allocator{public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class V>__forceinline void __fastcall _Construct(T* p,const V& x){new(p)T(x);}
template<class I,class O>NOINLINE O __fastcall __uninitialized_copy(I first,I last,O result,const __false_type&){O current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}
template<class I,class N,class T>__forceinline I __fastcall uninitialized_fill_n(I first,N n,const T& value){I current=first;for(;n>0;--n,++current)_Construct(&*current,value);return current;}
template<class I,class O,class D>NOINLINE O __fastcall __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class T>NOINLINE void __fastcall fill(I first,I last,const T& x){for(;first!=last;++first)*first=x;}
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:typedef unsigned int size_type;void _M_fill_insert(T*,size_type,const T&);protected:NOINLINE void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);};
template<class T,class A>void vector<T,A>::_M_fill_insert(T* position,size_type n,const T& value){if(n!=0){if((size_type)(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T copy=value;const size_type elemsAfter=(size_type)(this->_M_finish-position);T* oldFinish=this->_M_finish;if(elemsAfter>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=n;__copy_backward(position,oldFinish-n,oldFinish,random_access_iterator_tag(),(int*)0);fill(position,position+n,copy);}else{uninitialized_fill_n(this->_M_finish,n-elemsAfter,copy);this->_M_finish+=n-elemsAfter;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=elemsAfter;fill(position,oldFinish,copy);}}else _M_insert_overflow(position,value,__false_type(),n,false);}}
}
template void std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_fill_insert(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*,unsigned int,const CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry&);