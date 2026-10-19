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

#include <cstdio>
#include <cstdlib>
static int failures=0,ctors=0,dtors=0,notifies=0,deletes=0,allocations=0;
static CEnginePrimitiveHandle* temporary=0;
static CPersistentPrimitiveMethods* attach=0;
static void check(bool b){if(!b)++failures;}
void* operator new(size_t size) throw(std::bad_alloc){void* p=std::malloc(size);if(!p)throw std::bad_alloc();if(size==sizeof(CHandleNode) && temporary){++allocations;if(attach)temporary->Data.Object=attach;}return p;}
void operator delete(void* p) throw(){std::free(p);}
CBase::CBase(){++ctors;temporary=reinterpret_cast<CEnginePrimitiveHandle*>(this);}
CBase::~CBase(){++dtors;}
CPersistentPrimitiveMethods::~CPersistentPrimitiveMethods(){}
void CPersistentPrimitiveMethods::Delete()const{++deletes;check(IVRefCount==0);}
void CPersistentPrimitiveMethods::ReservedSlot2(){check(false);}
void CPersistentPrimitiveMethods::NotifyReleased(const CEnginePrimitiveHandle& h){check(h.Data.Object==this && IVRefCount==1);++notifies;}
static void run(unsigned long refs,bool middle){
 CHandleNode sentinel,left;sentinel.Next=middle?&left:&sentinel;sentinel.Previous=middle?&left:&sentinel;left.Next=&sentinel;left.Previous=&sentinel;
 CPersistentPrimitiveMethods object;object.IVRefCount=refs;object.Type=91;attach=refs?&object:0;ctors=dtors=notifies=deletes=allocations=0;temporary=0;
 CEnginePrimitiveHandleList list;CHandleIterator position(&sentinel);CHandleIterator result=list.InsertDefault(position);
 CHandleNode* node=result.Node;check(node!=&sentinel && node!=&left && node->Next==&sentinel && node->Previous==(middle?&left:&sentinel));
 check(sentinel.Previous==node && (middle?left.Next:sentinel.Next)==node && position.Node==&sentinel && ctors==1 && dtors==1 && allocations==1);
 check(node->Value.Data.Object==(refs?&object:0) && object.IVRefCount==refs && notifies==0 && deletes==0);
 attach=0;temporary=0;node->Value.~CEnginePrimitiveHandle();::operator delete(node);
 check(object.IVRefCount==(refs?refs-1:0) && notifies==(refs==1?1:0) && deletes==(refs==1?1:0) && object.Type==91);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CHandleNode)==16 && sizeof(CEnginePrimitiveHandle)==8);
#endif
 run(0,false);run(0,true);run(1,false);run(3,true);
 if(failures){std::printf("007464a0_TEST FAIL %d\n",failures);return 1;}std::printf("007464a0_TEST PASS\n");return 0;}