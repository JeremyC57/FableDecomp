// NUISystem::CObservable::CObservable() -- an observable UI node: two empty observer lists around
// a cleared exclusive-observer pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BE7B, length 46 bytes.
// Callee: the STLport _List_base constructor @ 0x0042AC0A (takes the allocator by const reference,
// nulls the head and allocates the sentinel node: `[this]=0 ; malloc(0Ch) ; next=prev=self`); the
// manifest's "DoublyLinkedList_InitializeEmpty" label describes it. Vtable 0x01230044 =
// NUISystem::CObservable (rtti-vtables.tsv); the manifest label
// "CConsoleCommandParameters::CConsoleCommandParameters" is propagated and rejected.
// Layout (PDB CObservable 32 bytes with 12-byte debug lists; retail lists are the 4-byte STLport
// head pointer): list<CObserver*> Observers +0x04, CObserver* m_pExclusiveObserver +0x08,
// list<CObserver*> ConcurrentExclusiveObservers +0x0c (size 0x10).
// Shape: a frame-pointer, size-optimised translation unit (`push ebp ; mov ebp,esp ; push ecx`,
// `and [..],0`, `leave`): the default allocator argument of each list is a 1-byte empty temporary
// homed at [ebp-1] (both lists reuse the slot), passed by address to the out-of-line base.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {

class CObserver;

struct SRetailAllocator {                      // std::allocator<T>: empty, user constructor keeps it uninitialised
    SRetailAllocator() { }
};

struct CRetailListBase {                       // STLport _List_base: the sentinel node pointer
    void* m_pNode;
    CRetailListBase(const SRetailAllocator& allocator);   // 0x0042AC0A
};

struct CRetailList : CRetailListBase {         // STLport list<CObserver*>, 4 bytes
    CRetailList(const SRetailAllocator& allocator = SRetailAllocator()) : CRetailListBase(allocator) { }
};

struct CObservable {                           // vtable 0x01230044
    virtual void v00();
    CRetailList Observers;                     // +0x04
    CObserver*  m_pExclusiveObserver;          // +0x08
    CRetailList ConcurrentExclusiveObservers;  // +0x0c
    CObservable();
};

CObservable::CObservable()
    : Observers(), m_pExclusiveObserver(0), ConcurrentExclusiveObservers()
{
}

}