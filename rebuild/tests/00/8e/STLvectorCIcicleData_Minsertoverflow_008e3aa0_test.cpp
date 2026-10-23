// 008e3aa0 _STL::vector<CIcicleData>::_M_insert_overflow
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
namespace Icicles {
// Layout: retail vector has start/finish/storage at +0,+4,+8; 36-byte objects use the landed copy constructor 008e2fc0.
struct CBaseObjectPointer{void*object;unsigned refcount;};
struct CBaseIntelligentPointer{virtual ~CBaseIntelligentPointer();CBaseObjectPointer*PData;CBaseIntelligentPointer();CBaseIntelligentPointer(const CBaseIntelligentPointer&);};
template<class T>struct CIntelligentPointer:CBaseIntelligentPointer{CIntelligentPointer(){}CIntelligentPointer(const CIntelligentPointer&x):CBaseIntelligentPointer(x){}};
struct CThing;struct C3DVector{float X,Y,Z;};
struct CIcicleData{CIntelligentPointer<CThing>PEffect,PTrap;int EffectTimer,TrapTimer;C3DVector Position;CIcicleData();CIcicleData(const CIcicleData&);};
struct __false_type {__false_type(){}};
CIcicleData* __fastcall uninitialized_copy(CIcicleData*,CIcicleData*,CIcicleData*,const __false_type&);
namespace _STL {
template<class T>__forceinline const T&max(const T&a,const T&b){return a<b?b:a;}
template<class T>__forceinline T*uninitialized_fill_n(T*out,unsigned n,const T&v){for(;n>0;--n,++out)if(out)new(out)T(v);return out;}
template<class T>struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;
 void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);
};
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&v,const __false_type&,unsigned n,bool atEnd){
 unsigned old=_M_finish-_M_start;unsigned capacity=old+max(old,n);
 T*buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T*out=buffer;
 for(T*p=_M_start;p!=pos;++p,++out)if(out)new(out)T(*p);
 if(n==1){if(out)new(out)T(v);++out;}
 else out=uninitialized_fill_n(out,n,v);
 if(!atEnd)out=uninitialized_copy(pos,_M_finish,out,__false_type());
 _M_clear();_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
}
template struct vector<CIcicleData>;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace Icicles{
static int liveLinks;
static void release(CBaseObjectPointer*p){if(p&&!--p->refcount&&!p->object){delete p;--liveLinks;}}
struct CThing{CBaseObjectPointer*IntelligentPointer;CThing():IntelligentPointer(0){}void Unlink(){if(IntelligentPointer){CBaseObjectPointer*p=IntelligentPointer;IntelligentPointer=0;p->object=0;if(!p->refcount){delete p;--liveLinks;}}}~CThing(){Unlink();}};
static CThing*get(CBaseIntelligentPointer&x){if(!x.PData)return 0;if(x.PData->object)return(CThing*)x.PData->object;CBaseObjectPointer*p=x.PData;x.PData=0;release(p);return 0;}
static void set(CBaseIntelligentPointer&x,CThing*p){release(x.PData);x.PData=0;if(p){if(!p->IntelligentPointer){p->IntelligentPointer=new CBaseObjectPointer;p->IntelligentPointer->object=p;p->IntelligentPointer->refcount=0;++liveLinks;}x.PData=p->IntelligentPointer;++x.PData->refcount;}}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::CBaseIntelligentPointer(const CBaseIntelligentPointer&x):PData(0){set(*this,get(const_cast<CBaseIntelligentPointer&>(x)));}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){release(PData);PData=0;}
CIcicleData::CIcicleData():EffectTimer(0),TrapTimer(0){Position.X=Position.Y=Position.Z=0;}
CIcicleData::CIcicleData(const CIcicleData&v):PEffect(v.PEffect),PTrap(v.PTrap),EffectTimer(v.EffectTimer),TrapTimer(v.TrapTimer),Position(v.Position){}
CIcicleData*__fastcall uninitialized_copy(CIcicleData*a,CIcicleData*b,CIcicleData*c,const __false_type&){while(a!=b){if(c)new(c)CIcicleData(*a);++a;++c;}return c;}
namespace _STL{template<>void vector<CIcicleData>::_M_clear(){for(CIcicleData*p=_M_start;p!=_M_finish;++p)p->~CIcicleData();free(_M_start);}}
}
int main(){using namespace Icicles;assert(sizeof(CIcicleData)==36);{CThing owner;{_STL::vector<CIcicleData>v;memset(&v,0,sizeof v);CIcicleData seed;seed.EffectTimer=17;seed.TrapTimer=23;seed.Position.X=2;set(seed.PEffect,&owner);__false_type tag;v._M_insert_overflow(v._M_start,seed,tag,1,false);assert(v._M_finish-v._M_start==1&&v._M_start[0].EffectTimer==17&&owner.IntelligentPointer->refcount==2);{CIcicleData liveCopy(seed);assert(liveCopy.PEffect.PData==owner.IntelligentPointer&&owner.IntelligentPointer->refcount==3);}assert(owner.IntelligentPointer->refcount==2);v._M_insert_overflow(v._M_start,seed,tag,2,false);assert(v._M_finish-v._M_start==3&&v._M_start[2].TrapTimer==23&&owner.IntelligentPointer->refcount==4);v._M_insert_overflow(v._M_finish,seed,tag,0,true);assert(v._M_finish-v._M_start==3&&owner.IntelligentPointer->refcount==4);v._M_clear();assert(owner.IntelligentPointer->refcount==1);}assert(owner.IntelligentPointer&&owner.IntelligentPointer->refcount==0&&liveLinks==1);owner.Unlink();assert(!liveLinks);}{CThing owner;CIcicleData expired;set(expired.PEffect,&owner);owner.Unlink();assert(liveLinks==1&&expired.PEffect.PData&&!expired.PEffect.PData->object);{CIcicleData copied(expired);assert(!copied.PEffect.PData&&!expired.PEffect.PData&&!liveLinks);}}{CThing owner;CIcicleData seed;set(seed.PEffect,&owner);_STL::vector<CIcicleData>v;memset(&v,0,sizeof v);__false_type tag;v._M_insert_overflow(v._M_start,seed,tag,1,false);assert(owner.IntelligentPointer->refcount==2);owner.Unlink();v._M_insert_overflow(v._M_finish,seed,tag,1,true);assert(!seed.PEffect.PData&&!v._M_start[0].PEffect.PData&&!v._M_start[1].PEffect.PData&&!liveLinks);v._M_clear();}assert(!liveLinks);puts("008e3aa0_TEST PASS");}