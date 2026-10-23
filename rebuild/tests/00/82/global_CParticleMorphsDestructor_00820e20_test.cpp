// 00820e20 NManager00820e20::CMap::Destroy (retail ~CMap body; catalog CParticleMorphs_Destructor is wrong)
// Flags: /O2 /Oy /W3
// Leaf: Destroy
#include <stdlib.h>
#include <new>
void __cdecl operator delete[](void*);
#include "engine/CIDrawMap.h"
namespace NManager00820e20 {
// Layout: retail CMap/CIDrawMap header4, secondary base+4, cached volumes+8, list+0c, strings+10, context+20,storage+1a6c, ownedobjects+1a78/+1a7c.
// CMap.h is quarantined; CIDrawMap active header used as authoritative primary base ABI view.
extern int CMap_vftable_CIDrawMap_0127252c[],CMap_vftable_CBaseClassNonCopyable_01272524[],CIDrawMap_vftable_01272494[],CBaseClassNonCopyable_vftable[];
struct Object{virtual ~Object();};struct Node{Node*next,*prev;};
__forceinline Object*Value(Node*n){return(Object*)((char*)n+8);}
#pragma pack(push,1)
struct CCharStringData{char*text;unsigned length,capacity;bool useFastExtend;int refs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString{CCharStringData*data;CCharString():data(0){++StringInstances;}__declspec(noinline)~CCharString(){if(data&&--data->refs<=0){if(data->text){::operator delete[](data->text);data->text=0;}data->length=0;data->capacity&=0x80000000u;delete data;}data=0;--StringInstances;}};struct Strings{int id;CCharString value;};
struct Target{void Remove(Object*);};
struct Context{virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void v3()=0;virtual void v4()=0;virtual void v5()=0;virtual void v6()=0;virtual void v7()=0;virtual void v8()=0;virtual void v9()=0;virtual void v10()=0;virtual void v11()=0;virtual Target*Get()=0;};
struct CBaseClassNonCopyable{void*table;__declspec(noinline)void Destroy(){table=CBaseClassNonCopyable_vftable;}};struct __false_type{__false_type(){}};
struct Control{unsigned refs;void(__fastcall*destroy)(void*);void*object;};
struct Counted{Object*data;Control*info;Counted():data(0),info(0){}__forceinline~Counted(){if(info){if(!--info->refs){info->destroy(info->object);delete info;}}data=0;info=0;}};
struct CachedEntry{int key;Counted value;};
template<class T>__declspec(noinline)void __fastcall _Destroy(CachedEntry*first,CachedEntry*last,const T&){for(;first!=last;++first)first->~CachedEntry();}
struct vector{CachedEntry*first,*last,*end;};
struct CMap:CIDrawMap{CBaseClassNonCopyable sub;vector*heap;Node*list;Strings*first,*last,*end;int gap;Context*context;char pad[0x1a6c-36];void*storage;char pad2[8];Object*a,*b;void Destroy();__forceinline void Clear(){Node*p=list->next;while(p!=list){Node*old=p;p=p->next;Value(old)->~Object();free(old);}list->next=list;list->prev=list;}};
void CMap::Destroy(){__vftable=CMap_vftable_CIDrawMap_0127252c;sub.table=CMap_vftable_CBaseClassNonCopyable_01272524;Target*t=context->Get();for(Node*p=list->next;p!=list;p=p->next)t->Remove(Value(p));Clear();if(b)delete b;if(a)delete a;if(storage)free(storage);Strings*finish=last;for(Strings*s=first;s!=finish;++s)s->~Strings();if(first)free(first);Clear();if(list)free(list);vector*p=heap;if(p){_Destroy(p->first,p->last,__false_type());if(p->first)free(p->first);delete p;}sub.Destroy();__vftable=CIDrawMap_vftable_01272494;}
}

void* __cdecl operator new[](unsigned int);

#include <assert.h>
#include <stdio.h>
#include <string.h>
static void*watchedText;static int arrayDeletes;
void __cdecl operator delete[](void*p){if(p==watchedText)++arrayDeletes;free(p);}
namespace NManager00820e20 {
int CMap_vftable_CIDrawMap_0127252c[]={1},CMap_vftable_CBaseClassNonCopyable_01272524[]={2},CIDrawMap_vftable_01272494[]={3},CBaseClassNonCopyable_vftable[]={4};int StringInstances;static int removed,brushDeaths,ownedDeaths,cachedDeaths,baseDeaths,contextCalls;static int removalOrder[4],deathOrder[8],deathCount;
Object::~Object(){++baseDeaths;}
struct TestObject:Object{int kind,id;TestObject(int k,int n):kind(k),id(n){}~TestObject(){deathOrder[deathCount++]=id;if(kind==0)++brushDeaths;else if(kind==1)++ownedDeaths;else++cachedDeaths;}};
void Target::Remove(Object*p){TestObject*v=(TestObject*)p;assert(v->kind==0);removalOrder[removed++]=v->id;}
static Target target;
struct TestContext:Context{void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}Target*Get(){++contextCalls;return &target;}};
void __fastcall DeleteObject(void*p){delete(Object*)p;}
void Run(bool full){CMap m;memset(&m,0,sizeof m);TestContext context;m.context=&context;m.list=new(malloc(sizeof(Node)))Node;m.list->next=m.list->prev=m.list;if(full){for(int i=0;i<2;++i){void*storage=malloc(sizeof(Node)+sizeof(TestObject));Node*n=new(storage)Node;new((char*)storage+8)TestObject(0,i);n->next=m.list;n->prev=m.list->prev;m.list->prev->next=n;m.list->prev=n;}m.a=new TestObject(1,3);m.b=new TestObject(1,2);m.storage=malloc(13);m.first=(Strings*)malloc(2*sizeof(Strings));for(int j=0;j<2;++j){new(m.first+j)Strings;m.first[j].id=j;}CCharStringData*shared=new CCharStringData;shared->text=new char[6];watchedText=shared->text;memcpy(shared->text,"theme",6);shared->length=5;shared->capacity=6;shared->useFastExtend=false;shared->refs=2;m.first[0].value.data=m.first[1].value.data=shared;m.last=m.end=m.first+2;m.heap=new vector;m.heap->first=(CachedEntry*)malloc(2*sizeof(CachedEntry));for(int k=0;k<2;++k){new(m.heap->first+k)CachedEntry;CachedEntry&e=m.heap->first[k];e.key=k;e.value.data=new TestObject(2,4+k);e.value.info=new Control;e.value.info->refs=1;e.value.info->destroy=DeleteObject;e.value.info->object=e.value.data;}m.heap->last=m.heap->end=m.heap->first+2;}m.Destroy();if(full)assert(arrayDeletes==1);assert(m.__vftable==CIDrawMap_vftable_01272494&&m.sub.table==CBaseClassNonCopyable_vftable&&!StringInstances);}
}
int main(){using namespace NManager00820e20;assert(sizeof(CIDrawMap)==4&&sizeof(Node)==8&&sizeof(CachedEntry)==12&&sizeof(Control)==12&&sizeof(Strings)==8);Run(false);assert(contextCalls==1&&!removed&&!baseDeaths);Run(true);assert(contextCalls==2&&removed==2&&removalOrder[0]==0&&removalOrder[1]==1&&brushDeaths==2&&ownedDeaths==2&&cachedDeaths==2&&baseDeaths==6&&deathCount==6);for(int i=0;i<6;++i)assert(deathOrder[i]==i);{CCharString value;value.data=new CCharStringData;value.data->text=new char[2];value.data->text[0]='x';value.data->text[1]=0;value.data->length=1;value.data->capacity=0x80000002u;value.data->useFastExtend=false;value.data->refs=0;watchedText=value.data->text;}assert(arrayDeletes==2&&!StringInstances);puts("00820e20_TEST PASS");return 0;}