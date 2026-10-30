// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E)
// CEnginePrimitiveHandleList::InsertDefault(iterator) -- insert a default CEnginePrimitiveHandle before `position` and return
//   the new node's iterator (hidden result)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007464A0, 177 bytes (+15 int3). STLport list nodes are 16 bytes: next +0, prev +4,
//   value +8. The node comes from the CRT malloc import thunk 0x00BFEA0E (not ::operator new).
// Callees: CBaseClass::CBaseClass() 0x0099A2D0 and CBaseClass::~CBaseClass() 0x0099A2E0 (both store vtable 0x01231710 =
//   CBaseClass; the manifest calls them "CBase::CBase"), vtable 0x0122F598 = CEnginePrimitiveHandle (rtti-vtables.tsv).
// Layout: PDB CEnginePrimitiveHandle (8 bytes: vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04);
//   the pointee keeps IVRefCount at +0x04 and Type at +0x08 (engine/CIVCountedPointeeBase.h,
//   engine/CEnginePersistentPrimitiveBase.h). Local model: those generated headers are flat and carry no virtual
//   slots, which the handle's destructor needs (Delete slot 1, NotifyReleased slot 3).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t);                 // 0x00BFEA0E
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
 virtual ~CEnginePersistentPrimitiveBase();
 virtual void Delete()const;
 virtual void ReservedSlot2();
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 unsigned long IVRefCount;
 long Type;
 __forceinline unsigned long RemoveReference(){return --IVRefCount;}
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CHandlePointer {
 CEnginePersistentPrimitiveBase* Object;
 CHandlePointer():Object(0){}
 CHandlePointer(const CHandlePointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CHandlePointer(){if(Object){CEnginePersistentPrimitiveBase* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
class CEnginePrimitiveHandle:public CBaseClass { public:
 CHandlePointer Data;
 CEnginePrimitiveHandle():CBaseClass(),Data(){}
 __forceinline virtual ~CEnginePrimitiveHandle(){if(Data.Object && Data.Object->IVRefCount==1)Data.Object->NotifyReleased(*this);}
};
#include <new>
struct CHandleNode { CHandleNode* Next;CHandleNode* Previous;CEnginePrimitiveHandle Value; };
struct CHandleIterator {CHandleNode* Node;CHandleIterator(CHandleNode* n):Node(n){} CHandleIterator(const CHandleIterator& o):Node(o.Node){}};
struct CEnginePrimitiveHandleList {
 CHandleIterator InsertDefault(CHandleIterator);
 __forceinline CHandleIterator Insert(CHandleIterator position,const CEnginePrimitiveHandle& value);
};
__forceinline CHandleIterator CEnginePrimitiveHandleList::Insert(CHandleIterator position,const CEnginePrimitiveHandle& value)
{
 CHandleNode* node=static_cast<CHandleNode*>(malloc(sizeof(CHandleNode)));
 if(&node->Value)new(&node->Value)CEnginePrimitiveHandle(value);
 CHandleNode* previous=position.Node->Previous;
 node->Next=position.Node;
 node->Previous=previous;
 previous->Next=node;
 position.Node->Previous=node;
 return CHandleIterator(node);
}

CHandleIterator CEnginePrimitiveHandleList::InsertDefault(CHandleIterator position){return Insert(position,CEnginePrimitiveHandle());}