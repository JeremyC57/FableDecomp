// Grade: HELD(bytes match retail) -- wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E | Codex source 00/74/CEnginePrimitiveHandleList_InsertDefault_007464a0.cpp, flags , pass pattern 007464a0_TEST PASS
// CEnginePrimitiveHandleList::InsertDefault -- insert default handle before position, return iterator in hidden result. Retail007464a0 complete177bytes+15int3. Nodes16bytes, prev/next links0/4,value8; allocationBFEA0E. Shared persistent headers and genuine handle destructor. No list/handle header exists.
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _MSC_VER
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#endif
struct CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {
 virtual ~CPersistentPrimitiveMethods();
 virtual void Delete()const;
 virtual void ReservedSlot2();
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 unsigned long IVRefCount;
 long Type;
 __forceinline unsigned long RemoveReference(){return --IVRefCount;}
};
struct CBase {CBase();virtual ~CBase();}; //0099a2d0/0099a2e0
struct CHandlePointer {
 CPersistentPrimitiveMethods* Object;
 CHandlePointer():Object(0){}
 CHandlePointer(const CHandlePointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CHandlePointer(){if(Object){CPersistentPrimitiveMethods* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
struct CEnginePrimitiveHandle:CBase {
 CHandlePointer Data;
 CEnginePrimitiveHandle():CBase(),Data(){}
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
 CHandleNode* node=static_cast<CHandleNode*>(::operator new(sizeof(CHandleNode)));
 if(&node->Value)new(&node->Value)CEnginePrimitiveHandle(value);
 CHandleNode* previous=position.Node->Previous;
 node->Next=position.Node;
 node->Previous=previous;
 previous->Next=node;
 position.Node->Previous=node;
 return CHandleIterator(node);
}

CHandleIterator CEnginePrimitiveHandleList::InsertDefault(CHandleIterator position){return Insert(position,CEnginePrimitiveHandle());}