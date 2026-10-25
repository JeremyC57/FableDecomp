// CMapNodeAllocator::Free(void* node) -- vtable 0x012B30EC slot 0: release one PKCTRRedblackMap node to the heap (operator delete @ 0x00BFE9BC)
// Leaf: ?Free@CMapNodeAllocator@@UAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B320, 14 bytes (`mov eax,[esp+4] ; push eax ; call operator delete ; pop ecx ; ret 4`), thiscall (`this` unused).
// Identity (sweep1, 2026-10-25): its only reference is vtable 0x012B30EC (slot 0 0x00C1B320 Free, slot 1 0x00C1B330
//   Allocate), the vptr the PKCDefaultFileManager constructor 0x00C1AC40 stores at +0x1c before handing that object to
//   its +0x20 PKCTRRedblackMap (0x00C1AE20) as the node allocator; 0x18 bytes is one PKCTRRedblackNode (Left, Right,
//   Parent, Colour, Key, Value). Descriptive class and member names (no PDB covers libpk).
// Callees: MSVCR71 scalar operator new 0x00BFEA1A / operator delete 0x00BFE9BC (as retail calls them).
// Label correction: the landed row was the free `void __stdcall FreeBlock_00c1b320(void*)` (`delete (char*)block`; catalog "FreeBlock_00c1b320", manifest "sub_00c1b320").
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

void CMapNodeAllocator::Free(void* node)
{
    ::operator delete(node);
}
