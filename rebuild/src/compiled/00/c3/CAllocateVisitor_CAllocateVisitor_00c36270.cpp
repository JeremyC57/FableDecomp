// CAllocateVisitor::CAllocateVisitor(CResourceWorld* world) -- constructs the stack visitor CResourceWorld::AllocateForAll walks the owner set with: vtable 0x012B50CC, the world at +4
// Leaf: ??0CAllocateVisitor@@QAE@PAUCResourceWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36270, 18 bytes, __thiscall `ret 4`, returns `this`:
//   mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],12B50CCh ; mov [eax+4],ecx ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CVtable_012b50cc::CVtable_012b50cc(void*) (catalog; the manifest's "CSBAgents"
//   is propagated); FINDINGS chunk 437 ("0xC36270 = visitor ctor"). Its only caller is the landed
//   CResourceWorld::AllocateForAll 0x00C362F0 (call 0x00C3630F), whose source models this exact class: CAllocateVisitor
//   : IItemVisitor (vtable 0x012B465C: pure Visit, inline virtual destructor), vtable 0x012B50CC slot 0 Visit 0x00C36260
//   (`m_pWorld->AllocateFor(item)`), slot 1 the deleting destructor. The interface's own vptr store is dead and dropped.
//   Descriptive names (no PDB covers libpk).
struct CResourceWorld;

struct IItemVisitor {                       // vtable 0x012B465C
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~IItemVisitor() { }             // slot 1
};

struct CAllocateVisitor : IItemVisitor {    // vtable 0x012B50CC
    CResourceWorld* m_pWorld;               // +0x04
    CAllocateVisitor(CResourceWorld* world);
    virtual void Visit(void* item);         // 0x00C36260
};

CAllocateVisitor::CAllocateVisitor(CResourceWorld* world)
    : m_pWorld(world)
{
}
