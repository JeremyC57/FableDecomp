// std::vector::operator=(const vector& x) -- Copy or reconstruct the element range, growing when needed.
// Leaf: ??4?$vector@VCStreamInfo@CVertexShaderInput@@V?$allocator@VCStreamInfo@CVertexShaderInput@@@std@@@std@@QAEAAV01@ABV01@@Z
// Identity: Own uninitialized_copy00a5e7f0, copy00a5e830 and CVertexShaderInput DataStreams assignment; CVertexStreamFormat RTTI0129d578; PDB nested class keyV.
// Layout: original STLport vector12bytes and24/12/20-byte element layouts. Same-TU real helper loops expose input reads and destruction without modifying empty tags.
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
inline void* operator new(size_t,void* p){return p;}
class CThing;class CBaseObject;class CBaseObjectPointer;class CCPPointerInfo;class CContainmentVolume;
class CCharString {public:void* PStringData;CCharString();CCharString(const CCharString&);const CCharString& operator=(const CCharString&);~CCharString();};
class CBaseIntelligentPointer {public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:CIntelligentPointer();CIntelligentPointer(const CIntelligentPointer&);CIntelligentPointer& operator=(const CIntelligentPointer&);};
struct AttackParticleAndEndPoints {CCharString StartDummy,EndDummy;long ParticleType,PeriodicFrames;CIntelligentPointer<CThing> ParticleEffect;};
template<class T>class CCountedPointer {public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);CCountedPointer& operator=(const CCountedPointer&);~CCountedPointer();};
template<class T>class CArray {public:T* Start;T* Finish;T* End;};
// Layout: genuine polymorphic CVertexStreamFormat is16bytes: vptr+0 and12-byte DataElements+4. No active shared header.
class CVertexStreamFormat {public:class CDataElement;virtual ~CVertexStreamFormat();CArray<CDataElement> DataElements;CVertexStreamFormat();CVertexStreamFormat(const CVertexStreamFormat&);CVertexStreamFormat& operator=(const CVertexStreamFormat&);};
// Layout: owner supplies nested-name scope only. CStreamInfo is StreamIndex+0, StreamFormat+4,20bytes.
class CVertexShaderInput {public:class CStreamInfo {public:unsigned long StreamIndex;CVertexStreamFormat StreamFormat;};};

namespace std {
template<class F,class S>struct pair {F first;S second;};
struct __false_type {__false_type(){}};
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class T,class U>inline void _Construct(T* p,const U& x){new(p)T(x);}
template<class T>inline void _Destroy(T* p){p->~T();}
#pragma auto_inline(off)
template<class Input,class Output>__declspec(noinline) Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output cur=result;for(;first!=last;++first,++cur)_Construct(cur,*first);return cur;}
template<class Iter>__declspec(noinline) void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)_Destroy(first);}
template<class Input,class Output,class Distance>__declspec(noinline) Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}
#pragma auto_inline(on)
template<class Input,class Output>inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&){return __copy(first,last,result,random_access_iterator_tag(),(int*)0);}
template<class T>class allocator {public:T* allocate(unsigned int n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T* p,unsigned int){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A=allocator<T> >class vector {public:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
 unsigned int size()const{return (unsigned int)(_M_finish-_M_start);}unsigned int capacity()const{return (unsigned int)(_M_end_of_storage._M_data-_M_start);}
 T* _M_allocate_and_copy(unsigned int n,const T* first,const T* last){T* result=_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,__false_type());return result;}
 void _M_clear(){__destroy_aux(_M_start,_M_finish,__false_type());_M_end_of_storage.deallocate(_M_start,capacity());}
 vector& operator=(const vector& x){if(&x!=this){const unsigned int xlen=x.size();
 if(xlen>capacity()){T* tmp=_M_allocate_and_copy(xlen,(const T*)x._M_start+0,(const T*)x._M_finish+0);_M_clear();_M_start=tmp;_M_end_of_storage._M_data=_M_start+xlen;}
 else if(size()>=xlen){T* i=__copy_ptrs((const T*)x._M_start+0,(const T*)x._M_finish+0,_M_start,__false_type());__destroy_aux(i,_M_finish,__false_type());}
 else{__copy_ptrs((const T*)x._M_start,(const T*)x._M_start+size(),_M_start,__false_type());__uninitialized_copy((const T*)x._M_start+size(),(const T*)x._M_finish+0,_M_finish,__false_type());}
 _M_finish=_M_start+xlen;}return *this;}
};
}
typedef CVertexShaderInput::CStreamInfo Element;
template std::vector<Element>& std::vector<Element>::operator=(const std::vector<Element>&);