#include <stdio.h>
// std::vector::operator=(const vector& x) -- Copy or reconstruct the element range, growing when needed.
// Leaf: ??4?$vector@UAttackParticleAndEndPoints@@V?$allocator@UAttackParticleAndEndPoints@@@std@@@std@@QAEAAV01@ABV01@@Z
// Identity: CTCObjectAugmentations AttackParticleEffects vector assignment; own copy007698c0, assign00769c00,destroy0076a2a0.
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
typedef AttackParticleAndEndPoints Element;
template std::vector<Element>& std::vector<Element>::operator=(const std::vector<Element>&);

static int copies,assigns,destroys;
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){++copies;}
const CCharString& CCharString::operator=(const CCharString& x){++assigns;PStringData=x.PStringData;return *this;}
CCharString::~CCharString(){++destroys;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++destroys;}
template<class T>CIntelligentPointer<T>::CIntelligentPointer(){PData=0;}
template<class T>CIntelligentPointer<T>::CIntelligentPointer(const CIntelligentPointer& x){PData=x.PData;++copies;}
template<class T>CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& x){PData=x.PData;++assigns;return *this;}
template<class T>CCountedPointer<T>::CCountedPointer():Data(0),Info(0){}
template<class T>CCountedPointer<T>::CCountedPointer(const CCountedPointer& x):Data(x.Data),Info(x.Info){++copies;}
template<class T>CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer& x){Data=x.Data;Info=x.Info;++assigns;return *this;}
template<class T>CCountedPointer<T>::~CCountedPointer(){++destroys;}
CVertexStreamFormat::CVertexStreamFormat(){DataElements.Start=DataElements.Finish=DataElements.End=0;}
CVertexStreamFormat::CVertexStreamFormat(const CVertexStreamFormat& x):DataElements(x.DataElements){++copies;}
CVertexStreamFormat& CVertexStreamFormat::operator=(const CVertexStreamFormat& x){DataElements=x.DataElements;++assigns;return *this;}
CVertexStreamFormat::~CVertexStreamFormat(){++destroys;}
class CBaseObjectPointer {};class CCPPointerInfo {};class CContainmentVolume {};class CVertexStreamFormat::CDataElement {};
static char texts[12];static CBaseObjectPointer handles[6];static CCPPointerInfo infos[6];static CContainmentVolume volumes[6];static CVertexStreamFormat::CDataElement elements[12];
void set(Element& e,int k){e.StartDummy.PStringData=&texts[k];e.EndDummy.PStringData=&texts[k+6];e.ParticleType=k+20;e.PeriodicFrames=k+30;e.ParticleEffect.PData=&handles[k];}
static bool same(const Element& e,int k){return e.StartDummy.PStringData==&texts[k] && e.EndDummy.PStringData==&texts[k+6] && e.ParticleType==k+20 && e.PeriodicFrames==k+30 && e.ParticleEffect.PData==&handles[k];}

typedef std::vector<Element> Vec;
static void init(Vec& v,unsigned int capacity,unsigned int length){v._M_start=(Element*)malloc(capacity*sizeof(Element));if(!v._M_start)exit(2);v._M_finish=v._M_start+length;v._M_end_of_storage._M_data=v._M_start+capacity;for(unsigned int i=0;i<length;++i){new(v._M_start+i)Element;set(v._M_start[i],i);}}
static void clear(Vec& v){for(Element* p=v._M_start;p!=v._M_finish;++p)p->~Element();free(v._M_start);}
int main(){Vec source,target;int ok=1;
 init(source,4,4);init(target,2,2);Element* old=target._M_start;copies=assigns=destroys=0;
 Vec* result=&(target=source);ok=ok && result==&target && target._M_start!=old && target.size()==4 && target.capacity()==4 && copies==4*3 && assigns==0 && destroys==2*3;
 for(int i=0;i<4;++i)ok=ok && same(target._M_start[i],i);clear(target);
 init(target,6,2);old=target._M_start;copies=assigns=destroys=0;target=source;
 ok=ok && target._M_start==old && target.size()==4 && target.capacity()==6 && copies==2*3 && assigns==2*3 && destroys==0;
 for(int j=0;j<4;++j)ok=ok && same(target._M_start[j],j);
 copies=assigns=destroys=0;target=target;ok=ok && target.size()==4 && !copies && !assigns && !destroys;
 source._M_finish[-1].~Element();--source._M_finish;copies=assigns=destroys=0;target=source;
 ok=ok && target._M_start==old && target.size()==3 && target.capacity()==6 && copies==0 && assigns==3*3 && destroys==3;
 for(int k=0;k<3;++k)ok=ok && same(target._M_start[k],k);
 for(Element* p=source._M_start;p!=source._M_finish;++p)p->~Element();source._M_finish=source._M_start;copies=assigns=destroys=0;target=source;
 ok=ok && target._M_start==old && target.size()==0 && target.capacity()==6 && !copies && !assigns && destroys==3*3;
 clear(source);clear(target);
 puts(ok?"0076b320_TEST PASS":"0076b320_TEST FAIL");return ok?0:1;
}