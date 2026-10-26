// Behaviour test for retail 0x007464a0: the candidate source below is embedded verbatim (from src/007464a0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
class CBaseClass { public:CBaseClass();virtual ~CBaseClass();}; //0099a2d0/0099a2e0
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
// ===== harness =====

#include <cstdio>
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) void* __stdcall HeapAlloc(void*, unsigned long, size_t);
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*, unsigned long, void*);
static int failures=0,ctors=0,dtors=0,notifies=0,deletes=0,allocations=0;
static CEnginePrimitiveHandle* temporary=0;
static CEnginePersistentPrimitiveBase* attach=0;
static void check(bool b){if(!b)++failures;}
/* malloc spy (the candidate allocates its node through the CRT malloc import): every allocation goes to the process
   heap, and free matches it; an allocation made while the temporary handle is live is the node allocation. */
extern "C" void* __cdecl malloc(size_t size){void* p=HeapAlloc(GetProcessHeap(),0,size?size:1);if(size==sizeof(CHandleNode) && temporary){++allocations;if(attach)temporary->Data.Object=attach;}return p;}
extern "C" void __cdecl free(void* p){if(p)HeapFree(GetProcessHeap(),0,p);}
CBaseClass::CBaseClass(){++ctors;temporary=reinterpret_cast<CEnginePrimitiveHandle*>(this);}
CBaseClass::~CBaseClass(){++dtors;}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CEnginePersistentPrimitiveBase::Delete()const{++deletes;check(IVRefCount==0);}
void CEnginePersistentPrimitiveBase::ReservedSlot2(){check(false);}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle& h){check(h.Data.Object==this && IVRefCount==1);++notifies;}
static void run(unsigned long refs,bool middle){
 CHandleNode sentinel,left;sentinel.Next=middle?&left:&sentinel;sentinel.Previous=middle?&left:&sentinel;left.Next=&sentinel;left.Previous=&sentinel;
 CEnginePersistentPrimitiveBase object;object.IVRefCount=refs;object.Type=91;attach=refs?&object:0;ctors=dtors=notifies=deletes=allocations=0;temporary=0;
 CEnginePrimitiveHandleList list;CHandleIterator position(&sentinel);CHandleIterator result=list.InsertDefault(position);
 CHandleNode* node=result.Node;check(node!=&sentinel && node!=&left && node->Next==&sentinel && node->Previous==(middle?&left:&sentinel));
 check(sentinel.Previous==node && (middle?left.Next:sentinel.Next)==node && position.Node==&sentinel && ctors==1 && dtors==1 && allocations==1);
 check(node->Value.Data.Object==(refs?&object:0) && object.IVRefCount==refs && notifies==0 && deletes==0);
 attach=0;temporary=0;node->Value.~CEnginePrimitiveHandle();free(node);
 check(object.IVRefCount==(refs?refs-1:0) && notifies==(refs==1?1:0) && deletes==(refs==1?1:0) && object.Type==91);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CHandleNode)==16 && sizeof(CEnginePrimitiveHandle)==8);
#endif
 run(0,false);run(0,true);run(1,false);run(3,true);
 if(failures){std::printf("007464a0_TEST FAIL %d\n",failures);return 1;}std::printf("007464a0_TEST PASS\n");return 0;}