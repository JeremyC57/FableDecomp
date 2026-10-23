// 005e31f0 Row005e31f0::Vector::InsertOverflow
// Flags: /O2 /Oy /W3 
// Leaf: InsertOverflow
#include <stdlib.h>
#include <new>
namespace Row005e31f0 {
// Layout: oracle uses a 12-byte STLport vector; element stride 20 with counted pointer at final dword.
struct Ref {unsigned vtable;int refs;}; struct Element {unsigned a,b,c,d;Ref*ref;Element():a(0),b(0),c(0),d(0),ref(0){}__forceinline Element(const Element&v):a(v.a),b(v.b),c(v.c),d(v.d),ref(v.ref){if(ref)++ref->refs;}};
struct Tag {Tag(){}};
Element* __fastcall Copy(Element*,Element*,Element*,const bool&);
Element* __fastcall Fill(Element*,unsigned,const Element&,const bool&);
void __fastcall Destroy(Element*,Element*,const bool&);
__forceinline const unsigned& Max(const unsigned&a,const unsigned&b){return a<b?b:a;}
template<class T> struct Vector {T*first,*last,*limit;void InsertOverflow(T*,const T&,const Tag&,unsigned,bool);};
template<class T> void Vector<T>::InsertOverflow(T*pos,const T&value,const Tag&,unsigned n,bool atEnd) {
 unsigned old=last-first; unsigned capacity=old+Max(old,n);
 T* buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T* finish=Copy(first,pos,buffer,atEnd);
 if(n==1){if(finish)new(finish)T(value);++finish;}
 else finish=Fill(finish,n,value,atEnd);
 if(!atEnd)finish=Copy(pos,last,finish,atEnd);
 Destroy(first,last,atEnd);if(first)free(first);
 first=buffer;last=finish;limit=buffer+capacity;
}
template struct Vector<Element>;
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
Element* __fastcall Copy(Element*a,Element*b,Element*c,const bool&){while(a!=b){if(c)new(c)Element(*a);++c;++a;}return c;}
Element* __fastcall Fill(Element*a,unsigned n,const Element&v,const bool&){while(n--){if(a)new(a)Element(v);++a;}return a;}
void __fastcall Destroy(Element*a,Element*b,const bool&){while(a!=b){if(a->ref){Ref*r=a->ref;assert(r->refs>0);if(!--r->refs){releasingElement=a;((Owner*)r)->Release();releasingElement=0;}a->ref=0;++clears;}++a;}}
static void Empty(Vector<Element>&v){bool flag=false;Element*end=v.last;Destroy(v.first,end,flag);for(Element*p=v.first;p!=end;++p)assert(!p->ref);int before=releases;Destroy(v.first,end,flag);assert(releases==before);free(v.first);v.first=v.last=v.limit=0;}
}
int main(){using namespace Row005e31f0;assert(sizeof(Ref)==8&&sizeof(Element)==20);bool flag=false;Tag tag;
 {Vector<Element>v;v.first=v.last=v.limit=0;Element value;Owner*owner=new Owner;value.ref=(Ref*)owner;value.a=11;value.b=22;value.c=33;value.d=44;
 v.InsertOverflow(v.first,value,tag,1,false);assert(owner->refs==2&&!releases);assert(v.first[0].a==11&&v.first[0].b==22&&v.first[0].c==33&&v.first[0].d==44);value.a=99;assert(v.first[0].a==11);
 v.InsertOverflow(v.first,value,tag,2,false);assert(v.last-v.first==3&&owner->refs==4&&!releases);
 v.InsertOverflow(v.first+1,v.first[2],tag,1,false);assert(v.last-v.first==4&&owner->refs==5&&!releases);
 v.InsertOverflow(v.last,value,tag,0,true);assert(v.last-v.first==4&&owner->refs==5&&!releases);
 Empty(v);assert(owner->refs==1&&!releases&&liveOwners==1);Destroy(&value,&value+1,flag);assert(!value.ref&&releases==1&&destructions==1&&!liveOwners&&!liveResources);Destroy(&value,&value+1,flag);assert(releases==1);}
 {Element elements[4];Owner*unique=new Owner;Owner*shared=new Owner;elements[0].ref=(Ref*)unique;elements[1].ref=(Ref*)shared;elements[2].ref=(Ref*)shared;++shared->refs;
 Destroy(elements,elements+2,flag);assert(!elements[0].ref&&!elements[1].ref&&shared->refs==1);assert(releases==2&&liveOwners==1&&liveResources==1);
 Destroy(elements+2,elements+4,flag);assert(!elements[2].ref&&!elements[3].ref&&releases==3&&destructions==3&&!liveOwners&&!liveResources);
 Destroy(elements,elements+4,flag);assert(releases==3);}
 {Vector<Element>v;v.first=v.last=v.limit=0;Element value;value.a=17;v.InsertOverflow(v.first,value,tag,2,true);assert(v.last-v.first==2&&v.first[0].a==17&&!v.first[0].ref&&!v.first[1].ref);Empty(v);assert(releases==3);}
 assert(!releasingElement&&clears>3&&!liveOwners&&!liveResources);puts("005e31f0_TEST PASS");return 0;}