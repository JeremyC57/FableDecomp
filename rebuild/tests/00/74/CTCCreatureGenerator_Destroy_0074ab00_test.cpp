#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <new>
// BEGIN exact source snapshot SHA256 32363beddb2c20c2fba634cc369d55b867cfcbaa32dbc9cea9a27c52be388495
// 0074ab00 CTCCreatureGenerator::Destroy
// Flags: /O2 /Oy
// Leaf: DestroyGenerator
#include "engine/CTCCreatureGenerator.h"
#include <stdlib.h>
// Layout: RTTI vtable1268c24 confirmed CTCCreatureGenerator. Retail uses smaller containers: ring+18, vectors1c/60/6c/78,ref8c,handle90,exclusiona4.
class CThing;class CThingCreatureBase;class CBaseObject;
struct PointerLink {CBaseObject* item;int refs;};
class CBaseIntelligentPointer {public:PointerLink* link;CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
 __forceinline CIntelligentPointer(T* p){SetPItem(p);}
 __forceinline T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}
 __forceinline void SetPItem(T* p){CBaseIntelligentPointer::SetPItem((CBaseObject*)p);}
};
class CCharString {public:char* data;~CCharString();};
struct Ref{virtual void Reserved()=0;virtual void DeleteThis()=0;int count;__forceinline void Release(){if(--count==0)DeleteThis();}};
struct Node{Node* next;Node* prev;void* value;};
struct ListView{Node* head;__forceinline int Size()const{int count=0;for(Node* p=head->next;p!=head;p=p->next)++count;return count;}};
class CEnginePrimitiveHandle {public:virtual ~CEnginePrimitiveHandle();unsigned payload;};
typedef CIntelligentPointer<CThingCreatureBase> CreaturePointer;
extern unsigned GeneratorVtable_01268c24[];extern int CreatureCount_013bad54;
struct Killable {void Kill(bool);};void __fastcall ClearList(void*);void __fastcall DestroyBase(CTCCreatureGenerator*);
void __fastcall DestroyGenerator(CTCCreatureGenerator* self){
 char* bytes=(char*)self;self->__vftable=GeneratorVtable_01268c24;CIntelligentPointer<CThing>* handle=(CIntelligentPointer<CThing>*)(bytes+0x90);
 if(handle->GetPItem() && !(*(unsigned char*)((char*)handle->GetPItem()+0x91)&1)){((Killable*)handle->GetPItem())->Kill(true);CIntelligentPointer<CThing> temp(0);handle->SetPItem(temp.GetPItem());}
 *(CTCShapeManager**)(bytes+0xa4)=0;CreatureCount_013bad54-=((ListView*)(bytes+0x18))->Size();handle->CBaseIntelligentPointer::~CBaseIntelligentPointer();
 Ref*& ref=*(Ref**)(bytes+0x8c);if(ref){ref->Release();ref=0;}
 CEnginePrimitiveHandle* end=*(CEnginePrimitiveHandle**)(bytes+0x7c);for(CEnginePrimitiveHandle* p=*(CEnginePrimitiveHandle**)(bytes+0x78);p!=end;++p)p->~CEnginePrimitiveHandle();if(*(void**)(bytes+0x78))free(*(void**)(bytes+0x78));if(*(void**)(bytes+0x6c))free(*(void**)(bytes+0x6c));if(*(void**)(bytes+0x60))free(*(void**)(bytes+0x60));((CCharString*)(bytes+0x48))->~CCharString();
 CreaturePointer* creatureEnd=*(CreaturePointer**)(bytes+0x20);for(CreaturePointer* q=*(CreaturePointer**)(bytes+0x1c);q!=creatureEnd;++q)q->~CreaturePointer();if(*(void**)(bytes+0x1c))free(*(void**)(bytes+0x1c));ClearList(bytes+0x18);if(*(void**)(bytes+0x18))free(*(void**)(bytes+0x18));if(*(void**)(bytes+0xc))free(*(void**)(bytes+0xc));DestroyBase(self);
}

// END exact source snapshot
unsigned GeneratorVtable_01268c24[1];int CreatureCount_013bad54=10;static int kills,baseCalls,primitiveDestructions,stringDestructions,linksFreed;
class CBaseObject {public:PointerLink* registration;};
CBaseIntelligentPointer::CBaseIntelligentPointer():link(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return link?link->item:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){if(GetPItem()==p)return;if(link && --link->refs==0){if(link->item)link->item->registration=0;delete link;++linksFreed;}link=0;if(p){if(!p->registration){p->registration=new PointerLink;p->registration->item=p;p->registration->refs=0;}link=p->registration;++link->refs;}}
CEnginePrimitiveHandle::~CEnginePrimitiveHandle(){++primitiveDestructions;}
CCharString::~CCharString(){free(data);++stringDestructions;}
void Killable::Kill(bool b){assert(b);++kills;}
void __fastcall DestroyBase(CTCCreatureGenerator*){++baseCalls;}
void __fastcall ClearList(void* p){Node* h=*(Node**)p;Node* n=h->next;while(n!=h){Node* next=n->next;free(n);n=next;}h->next=h->prev=h;}
static Node* MakeList(unsigned n){Node* h=(Node*)malloc(sizeof(Node));h->next=h->prev=h;while(n--){Node* p=(Node*)malloc(sizeof(Node));p->next=h;p->prev=h->prev;h->prev->next=p;h->prev=p;}return h;}
int main(){
 CTCCreatureGenerator self;memset(&self,0,sizeof(self));char* bytes=(char*)&self;
 new(bytes+0x90)CIntelligentPointer<CThing>(0);*(Node**)(bytes+0x18)=MakeList(0);DestroyGenerator(&self);assert(CreatureCount_013bad54==10 && baseCalls==1 && kills==0);
 memset(&self,0,sizeof(self));char alive[256]={0};new(bytes+0x90)CIntelligentPointer<CThing>((CThing*)alive);*(Node**)(bytes+0x18)=MakeList(2);
 CEnginePrimitiveHandle* primitives=(CEnginePrimitiveHandle*)malloc(2*sizeof(CEnginePrimitiveHandle));new(primitives)CEnginePrimitiveHandle;new(primitives+1)CEnginePrimitiveHandle;*(CEnginePrimitiveHandle**)(bytes+0x78)=primitives;*(CEnginePrimitiveHandle**)(bytes+0x7c)=primitives+2;
 CreaturePointer* creatures=(CreaturePointer*)malloc(sizeof(CreaturePointer));new(creatures)CreaturePointer((CThingCreatureBase*)alive);*(CreaturePointer**)(bytes+0x1c)=creatures;*(CreaturePointer**)(bytes+0x20)=creatures+1;
 CCharString* name=(CCharString*)(bytes+0x48);name->data=(char*)malloc(5);strcpy(name->data,"test");DestroyGenerator(&self);
 assert(kills==1 && baseCalls==2 && CreatureCount_013bad54==8 && primitiveDestructions==2 && stringDestructions==2 && linksFreed==1 && !((CBaseObject*)alive)->registration);
 puts("0074ab00_TEST PASS");return 0;
}