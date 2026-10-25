// CResourceWorld::AllocateForAll() -- walks the world's owner collection with a stack visitor whose Visit creates each owner's resource allocation (CResourceWorld::AllocateFor)
// Leaf: AllocateForAll
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C362F0, 75 bytes, __fastcall, no arguments. Descriptive name (no PDB covers this
//   library): the catalog's `CSBAgents_BuildAndDispatch_A` is propagated. The collection is the interface at +8 of the
//   world's +4 object; its slot 10 calls the visitor's slot 0 for every item.
// Visitor: vtable 0x012B50CC (slot 0 Visit 0x00C36260 = `m_pWorld->AllocateFor(item)` as a tail jump, slot 1 the deleting
//   destructor 0x00C366A0), constructor 0x00C36270 (landed as CVtable_012b50cc::CVtable_012b50cc); abstract base vtable
//   0x012B465C. EH state 0 covers the visitor; its destructor 0x00C36690 (the base vtable store) is the unwind action.
// Shape: `this` stays in edx across the constructor call because the constructor's body is in this translation unit
//   (it touches only eax/ecx); a same-TU noinline stand-in gives VC7.1 the same knowledge. It must not sit under
//   `auto_inline(off)`: that keeps the abstract base's implicit constructor out of line, the stand-in then makes a call
//   and VC7.1 saves `this` in esi instead (78 bytes).
struct CResourceWorld;

struct IItemVisitor {                       // vtable 0x012B465C
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~IItemVisitor() { }             // slot 1
};

struct CAllocateVisitor : IItemVisitor {    // vtable 0x012B50CC
    CResourceWorld* m_pWorld;               // +0x04
    CAllocateVisitor(CResourceWorld* world);     // 0x00C36270
    virtual void Visit(void* item);              // 0x00C36260
};

struct IItemCollection {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void ForEach(IItemVisitor* visitor);   // slot 10
};

struct COwnerSet {
    unsigned char _pad00[8];
    IItemCollection m_items;                // +0x08
};

struct CResourceWorld {
    void* m_vtable;                         // +0x00
    COwnerSet* m_pOwners;                   // +0x04
    void AllocateFor(void* owner);          // 0x00C36180
    void AllocateForAll();
};

__declspec(noinline) CAllocateVisitor::CAllocateVisitor(CResourceWorld* world)
    : m_pWorld(world)
{
}

void CResourceWorld::AllocateForAll()
{
    CAllocateVisitor visitor(this);
    m_pOwners->m_items.ForEach(&visitor);
}