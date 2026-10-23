// 00820e20 NManager00820e20::Manager::Destroy
// Flags: /O2 /Oy /W3
// Leaf: Destroy
#include <stdlib.h>
#include <new>
void __cdecl operator delete[](void*);
#include "engine/CIDrawMap.h"
namespace NManager00820e20 {
// Layout: retail CMap/CIDrawMap header4, secondary base+4, cached volumes+8, list+0c, strings+10, context+20,storage+1a6c, ownedobjects+1a78/+1a7c.
// CMap.h is quarantined; CIDrawMap active header used as authoritative primary base ABI view.
extern int ManagerTable[],SubTable[],BaseTable[],SubBaseTable[];
struct Object{virtual ~Object();};struct Node{Node*next,*prev;};
__forceinline Object*Value(Node*n){return(Object*)((char*)n+8);}
#pragma pack(push,1)
struct StringData{char*text;unsigned length,capacity;bool useFastExtend;int refs;};
#pragma pack(pop)
extern int StringInstances;
struct String{StringData*data;String():data(0){++StringInstances;}__declspec(noinline)~String(){if(data&&--data->refs<=0){if(data->text){::operator delete[](data->text);data->text=0;}data->length=0;data->capacity&=0x80000000u;delete data;}data=0;--StringInstances;}};struct Strings{int id;String value;};
struct Target{void Remove(Object*);};
struct Context{virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual void v3()=0;virtual void v4()=0;virtual void v5()=0;virtual void v6()=0;virtual void v7()=0;virtual void v8()=0;virtual void v9()=0;virtual void v10()=0;virtual void v11()=0;virtual Target*Get()=0;};
struct Sub{void*table;__declspec(noinline)void Destroy(){table=SubBaseTable;}};struct Tag{Tag(){}};
struct Control{unsigned refs;void(__fastcall*destroy)(void*);void*object;};
struct Counted{Object*data;Control*info;Counted():data(0),info(0){}__forceinline~Counted(){if(info){if(!--info->refs){info->destroy(info->object);delete info;}}data=0;info=0;}};
struct CachedEntry{int key;Counted value;};
template<class T>__declspec(noinline)void __fastcall DestroyVector(CachedEntry*first,CachedEntry*last,const T&){for(;first!=last;++first)first->~CachedEntry();}
struct Vector{CachedEntry*first,*last,*end;};
struct Manager:CIDrawMap{Sub sub;Vector*heap;Node*list;Strings*first,*last,*end;int gap;Context*context;char pad[0x1a6c-36];void*storage;char pad2[8];Object*a,*b;void Destroy();__forceinline void Clear(){Node*p=list->next;while(p!=list){Node*old=p;p=p->next;Value(old)->~Object();free(old);}list->next=list;list->prev=list;}};
void Manager::Destroy(){__vftable=ManagerTable;sub.table=SubTable;Target*t=context->Get();for(Node*p=list->next;p!=list;p=p->next)t->Remove(Value(p));Clear();if(b)delete b;if(a)delete a;if(storage)free(storage);Strings*finish=last;for(Strings*s=first;s!=finish;++s)s->~Strings();if(first)free(first);Clear();if(list)free(list);Vector*p=heap;if(p){DestroyVector(p->first,p->last,Tag());if(p->first)free(p->first);delete p;}sub.Destroy();__vftable=BaseTable;}
}