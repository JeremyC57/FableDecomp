// CResourceWorld::AllocateFor(void* owner) -- creates the owner's 0x8C-byte resource allocation (`new CAmbientEmitter(this, owner)`); the allocation registers itself in the world's +0x14 registry, so the pointer is not kept
// Naming (naming wave 22): the allocation is the CAmbientEmitter (destructor 0x00C44F80 runs the landed CAmbientEmitter::ReleaseSounds 0x00C44B80 / ClearEvents 0x00C44D30 on it); was the descriptive CResourceAllocation.
// Leaf: AllocateFor
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36180, 85 bytes, __fastcall `ret 4`, EH-framed (handler 0x0121441B: the
//   operator delete unwind of the new-expression). Descriptive name (no PDB covers this library): the catalog's
//   `Alloc` is a placeholder. Callers: tail jumps at 0x00C2A593 / 0x00C36263. Counterpart of FreeAllocation 0x00C36290.
// Callees: scalar operator new @ 0x00BFEA1A, the allocation's constructor (landed 0x00C44EB0, `(world, owner)`,
//   `ret 8`; it registers the allocation in the world's +0x14 registry).
// Shape: the new-expression's result is homed in the `push ecx` frame word, the EH state 0 store sits between the
//   null test and its branch; the result is discarded (void member: both paths share one epilogue, a returned
//   pointer adds a second `xor eax,eax` epilogue).
#include <stddef.h>
void* __cdecl operator new(size_t size);                    // 0x00BFEA1A
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct CResourceWorld;
class CAmbientEmitter { public:
    unsigned char _body[0x8c];
    CAmbientEmitter(CResourceWorld* world, void* owner);    // 0x00C44EB0
};

struct CResourceWorld {
    void AllocateFor(void* owner);
};

void CResourceWorld::AllocateFor(void* owner)
{
    new CAmbientEmitter(this, owner);
}