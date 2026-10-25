// CMapNodeAllocator::Allocate() -- vtable 0x012B30EC slot 1: one 0x18-byte PKCTRRedblackMap node from the heap (operator new @ 0x00BFEA1A)
// Leaf: ?Allocate@CMapNodeAllocator@@UAEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B330, 11 bytes (`push 18h ; call operator new ; add esp,4 ; ret`), thiscall (`this` unused).
// Identity (sweep1, 2026-10-25): its only reference is vtable 0x012B30EC (slot 0 0x00C1B320 Free, slot 1 0x00C1B330
//   Allocate), the vptr the PKCDefaultFileManager constructor 0x00C1AC40 stores at +0x1c before handing that object to
//   its +0x20 PKCTRRedblackMap (0x00C1AE20) as the node allocator; 0x18 bytes is one PKCTRRedblackNode (Left, Right,
//   Parent, Colour, Key, Value). Descriptive class and member names (no PDB covers libpk).
// Callees: MSVCR71 scalar operator new 0x00BFEA1A / operator delete 0x00BFE9BC (as retail calls them).
// Label correction: the landed row was CClass_00c1b330::CallWithConstant_00c1b330() over an address-named `Call_00bfea1a(unsigned int)` (catalog "AllocateConstant_00c1b330", manifest "sub_00c1b330").
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block);

struct PKCTRRedblackNode {                      // 0x18 bytes
    PKCTRRedblackNode* Left;
    PKCTRRedblackNode* Right;
    PKCTRRedblackNode* Parent;
    long               Colour;
    unsigned long      Key;
    void*              Value;
};

class CMapNodeAllocator {                       // vtable 0x012B30EC
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
};

void* CMapNodeAllocator::Allocate()
{
    return ::operator new(sizeof(PKCTRRedblackNode));
}
