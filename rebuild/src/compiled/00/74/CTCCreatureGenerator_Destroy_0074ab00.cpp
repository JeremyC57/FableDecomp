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