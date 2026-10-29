#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
#include <new>
int main();
extern "C" void*__cdecl TestMalloc(unsigned int);extern "C" void __cdecl TestFree(void*);
#define malloc TestMalloc
#define free TestFree
// 00af7b60 NParticleEngine::CParticle::~CParticle
// Leaf: ??1CParticle@NParticleEngine@@UAE@XZ
// Flags:
#include <stdlib.h>
#include "rebuild_abi.h"
class CBaseClass{public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{CBaseClassNonCopyable(const CBaseClassNonCopyable&);protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
// Layout: handle private counted-pointer storage is unobserved; destruction is external.
class CEnginePrimitiveHandle:public CBaseClass{unsigned char unreadData[4];public:virtual ~CEnginePrimitiveHandle();};
namespace std{
template<class T>class allocator{public:allocator(){}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
template<class T>inline void _Destroy(T*p){p->~T();}template<class Forward>inline void _Destroy(Forward first,Forward last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,(unsigned int)(_M_end_of_storage._M_data-_M_start));}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){_Destroy(this->_M_start,this->_M_finish);}};
}
// Layout: native CArray storage/lifetime is its twelve-byte STLport vector base.
template<class T>class CArray:public std::vector<T>{};
namespace boost{template<class T>class scoped_ptr{T*ptr;scoped_ptr(const scoped_ptr&);public:explicit scoped_ptr(T*p=0):ptr(p){}~scoped_ptr(){delete ptr;}};}
namespace NParticleEngine{
// Layout: random-acceleration state is unread; raw original base and virtual slots retained.
class CRandomAcceleration:public CBaseClassNonCopyable{unsigned char unread[20];public:virtual ~CRandomAcceleration();virtual void Delete();};
class CParticle:public CBaseClassNonCopyable{unsigned char unreadPrefix[124];boost::scoped_ptr<CRandomAcceleration>RandomAcceleration;CArray<CArray<long> >PrimIDList;CArray<CEnginePrimitiveHandle>ParentHandleList;
#ifdef PASS9_TEST
friend int ::main();
#endif
public:virtual ~CParticle();virtual void Delete();};
CParticle::~CParticle(){}
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticle)==156);FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);

#undef malloc
#undef free

static unsigned errors,active,allocationCount,eventCount,events[64];static void*ownerAddress,*randomAddress;static CEnginePrimitiveHandle*handles;
struct Allocation{unsigned char*raw,*data;unsigned size;bool released;};static Allocation records[32];
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount==32){++errors;return 0;}Allocation&r=records[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;memset(r.raw,0xa5,size+32);++active;return r.data;}
static unsigned allocationId(void*p){for(unsigned i=0;i<allocationCount;++i)if(records[i].data==p)return i;return 999;}
extern "C" void __cdecl TestFree(void*p){unsigned id=allocationId(p);if(id==999||records[id].released){++errors;return;}Allocation&r=records[id];for(unsigned i=0;i<16;++i)if(r.raw[i]!=0xa5||r.data[r.size+i]!=0xa5)++errors;events[eventCount++]=100+id;r.released=true;--active;free(r.raw);}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(this==ownerAddress)events[eventCount++]=300;else if(this==randomAddress)events[eventCount++]=201;else ++errors;}
CEnginePrimitiveHandle::~CEnginePrimitiveHandle(){events[eventCount++]=(unsigned)(this-handles);}
namespace NParticleEngine{CRandomAcceleration::~CRandomAcceleration(){events[eventCount++]=200;}void CRandomAcceleration::Delete(){++errors;}void CParticle::Delete(){++errors;}}
using namespace NParticleEngine;
struct FixtureAcceleration:public CRandomAcceleration{static void*operator new(size_t n){return TestMalloc((unsigned)n);}static void operator delete(void*p){TestFree(p);}};
int main(){unsigned cases=0;for(unsigned n=0;n<=6;++n)for(unsigned outer=0;outer<=5;++outer)for(unsigned seed=0;seed<8;++seed)for(unsigned random=0;random<2;++random)for(unsigned capacity=0;capacity<2;++capacity){errors=active=allocationCount=eventCount=0;unsigned expected[64],wanted=0;
union Guard{double align;unsigned char bytes[164];}guard;memset(guard.bytes,0x6b,sizeof(guard.bytes));CParticle*owner=new(guard.bytes+4)CParticle;ownerAddress=owner;memset(owner->unreadPrefix,0x37,sizeof(owner->unreadPrefix));
unsigned hc=n+capacity;handles=hc?static_cast<CEnginePrimitiveHandle*>(TestMalloc(hc*sizeof(CEnginePrimitiveHandle))):0;for(unsigned i=0;i<n;++i){new(handles+i)CEnginePrimitiveHandle;expected[wanted++]=i;}CEnginePrimitiveHandle*hs[3]={handles,n?handles+n:handles,hc?handles+hc:handles};memcpy(&owner->ParentHandleList,hs,12);if(handles)expected[wanted++]=100+allocationId(handles);
unsigned oc=outer+capacity;CArray<long>*arrays=oc?static_cast<CArray<long>*>(TestMalloc(oc*sizeof(CArray<long>))):0;for(unsigned i=0;i<outer;++i){new(arrays+i)CArray<long>;unsigned length=(seed+i)%5,cap=length+capacity;long*data=cap?static_cast<long*>(TestMalloc(cap*sizeof(long))):0;for(unsigned j=0;j<length;++j)data[j]=(long)(seed*100+i*10+j);long*state[3]={data,length?data+length:data,cap?data+cap:data};memcpy(arrays+i,state,12);if(data)expected[wanted++]=100+allocationId(data);}CArray<long>*as[3]={arrays,outer?arrays+outer:arrays,oc?arrays+oc:arrays};memcpy(&owner->PrimIDList,as,12);if(arrays)expected[wanted++]=100+allocationId(arrays);
CRandomAcceleration*acc=random?new FixtureAcceleration:0;randomAddress=acc;memcpy(&owner->RandomAcceleration,&acc,4);if(acc){expected[wanted++]=200;expected[wanted++]=201;expected[wanted++]=100+allocationId(acc);}expected[wanted++]=300;unsigned char before[156];memcpy(before,owner,156);owner->~CParticle();if(errors||active||eventCount!=wanted)return 1;for(unsigned i=0;i<wanted;++i)if(events[i]!=expected[i])return 2;for(unsigned i=0;i<4;++i)if(guard.bytes[i]!=0x6b||guard.bytes[160+i]!=0x6b)return 3;for(unsigned i=4;i<156;++i)if(reinterpret_cast<unsigned char*>(owner)[i]!=before[i])return 4;for(unsigned i=0;i<allocationCount;++i)if(!records[i].released)return 5;++cases;}if(cases!=1344)return 6;puts("00af7b60_TEST PASS");return 0;}