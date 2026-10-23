// 005e31f0 Row005e31f0::vector<Element>::_M_insert_overflow (STLport; catalog: CTargetPointVector_ReserveAndCopy, but element is 20 bytes, CTargetPoint is 28)
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
namespace Row005e31f0 {
// Layout: oracle uses a 12-byte STLport vector; element stride 20 with counted pointer at final dword.
struct Ref {unsigned vtable;int refs;}; struct Element {unsigned a,b,c,d;Ref*ref;Element():a(0),b(0),c(0),d(0),ref(0){}__forceinline Element(const Element&v):a(v.a),b(v.b),c(v.c),d(v.d),ref(v.ref){if(ref)++ref->refs;}};
struct __false_type {__false_type(){}};
Element* __fastcall __uninitialized_copy(Element*,Element*,Element*,const bool&);
Element* __fastcall __uninitialized_fill_n(Element*,unsigned,const Element&,const bool&);
void __fastcall _Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);};
template<class T> void vector<T>::_M_insert_overflow(T*pos,const T&value,const __false_type&,unsigned n,bool atEnd) {
 unsigned old=_M_finish-_M_start; unsigned capacity=old+max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=__uninitialized_copy(_M_start,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=__uninitialized_fill_n(finish,n,value,atEnd);
 if(!atEnd)finish=__uninitialized_copy(pos,_M_finish,finish,atEnd);
 _Destroy(_M_start,_M_finish,atEnd);if(_M_start)free(_M_start);
 _M_start=buffer;_M_finish=finish;_M_end_of_storage=buffer+capacity;
}
template struct vector<Element>;
}

#include <stdio.h>
#include <assert.h>
namespace Row005e31f0 {
static int liveOwners,liveResources,releases,destructions,clears;
static Element* releasingElement;
struct Owner {
 virtual void Unused(){assert(false);}
 virtual void Release(){assert(refs==0);assert(releasingElement&&releasingElement->ref==(Ref*)this);++releases;delete this;}
 int refs;int* resource;
 Owner():refs(1),resource(new int(73)){++liveOwners;++liveResources;assert((char*)&refs-(char*)this==4);}
 ~Owner(){assert(refs==0&&*resource==73);delete resource;--liveResources;--liveOwners;++destructions;}
};
Element* __fastcall __uninitialized_copy(Element*a,Element*b,Element*c,const bool&){while(a!=b){if(c)new(c)Element(*a);++c;++a;}return c;}
Element* __fastcall __uninitialized_fill_n(Element*a,unsigned n,const Element&v,const bool&){while(n--){if(a)new(a)Element(v);++a;}return a;}
void __fastcall _Destroy(Element*a,Element*b,const bool&){while(a!=b){if(a->ref){Ref*r=a->ref;assert(r->refs>0);if(!--r->refs){releasingElement=a;((Owner*)r)->Release();releasingElement=0;}a->ref=0;++clears;}++a;}}
static void Empty(vector<Element>&v){bool flag=false;Element*end=v._M_finish;_Destroy(v._M_start,end,flag);for(Element*p=v._M_start;p!=end;++p)assert(!p->ref);int before=releases;_Destroy(v._M_start,end,flag);assert(releases==before);free(v._M_start);v._M_start=v._M_finish=v._M_end_of_storage=0;}
}
int main(){using namespace Row005e31f0;assert(sizeof(Ref)==8&&sizeof(Element)==20);bool flag=false;__false_type tag;
 {vector<Element>v;v._M_start=v._M_finish=v._M_end_of_storage=0;Element value;Owner*owner=new Owner;value.ref=(Ref*)owner;value.a=11;value.b=22;value.c=33;value.d=44;
 v._M_insert_overflow(v._M_start,value,tag,1,false);assert(owner->refs==2&&!releases);assert(v._M_start[0].a==11&&v._M_start[0].b==22&&v._M_start[0].c==33&&v._M_start[0].d==44);value.a=99;assert(v._M_start[0].a==11);
 v._M_insert_overflow(v._M_start,value,tag,2,false);assert(v._M_finish-v._M_start==3&&owner->refs==4&&!releases);
 v._M_insert_overflow(v._M_start+1,v._M_start[2],tag,1,false);assert(v._M_finish-v._M_start==4&&owner->refs==5&&!releases);
 v._M_insert_overflow(v._M_finish,value,tag,0,true);assert(v._M_finish-v._M_start==4&&owner->refs==5&&!releases);
 Empty(v);assert(owner->refs==1&&!releases&&liveOwners==1);_Destroy(&value,&value+1,flag);assert(!value.ref&&releases==1&&destructions==1&&!liveOwners&&!liveResources);_Destroy(&value,&value+1,flag);assert(releases==1);}
 {Element elements[4];Owner*unique=new Owner;Owner*shared=new Owner;elements[0].ref=(Ref*)unique;elements[1].ref=(Ref*)shared;elements[2].ref=(Ref*)shared;++shared->refs;
 _Destroy(elements,elements+2,flag);assert(!elements[0].ref&&!elements[1].ref&&shared->refs==1);assert(releases==2&&liveOwners==1&&liveResources==1);
 _Destroy(elements+2,elements+4,flag);assert(!elements[2].ref&&!elements[3].ref&&releases==3&&destructions==3&&!liveOwners&&!liveResources);
 _Destroy(elements,elements+4,flag);assert(releases==3);}
 {vector<Element>v;v._M_start=v._M_finish=v._M_end_of_storage=0;Element value;value.a=17;v._M_insert_overflow(v._M_start,value,tag,2,true);assert(v._M_finish-v._M_start==2&&v._M_start[0].a==17&&!v._M_start[0].ref&&!v._M_start[1].ref);Empty(v);assert(releases==3);}
 assert(!releasingElement&&clears>3&&!liveOwners&&!liveResources);puts("005e31f0_TEST PASS");return 0;}