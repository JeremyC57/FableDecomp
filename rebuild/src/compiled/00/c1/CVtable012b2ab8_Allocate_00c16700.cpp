// CVtable_012b2ab8::Allocate(unsigned int size) -- heap allocator slot 1: a CRT malloc of size bytes
// Leaf: ?Allocate@CVtable_012b2ab8@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C16700, 17 bytes, thiscall, `ret 4`: `push size ; call [malloc] ; add esp,4` (the live result makes VC clean the slot with `add esp,4`).
// Identity (descriptive): slots 1 and 2 of vtable 0x012B2AB8 (slot 0 is the deleting destructor 0x00C16730 that ends in
//   LHA::CAllocator::~CAllocator 0x00C16620; the landed CVtable_012b2ab8 constructor 0x00C166F0 installs it). libpk has no
//   RTTI or PDB, so the class keeps its vtable name; the base is the RTTI class LHA::CAllocator (vtable 0x0129D264, slots
//   1 and 2 are _purecall), whose CBBBSoundAllocator sibling (0x00A3F960 / 0x00A3F970) has the same two slots.
// Callees: MSVCR71 malloc / free through the import table (0x01440158 / 0x01440154).
#include <stddef.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(size_t size);
extern "C" __declspec(dllimport) void __cdecl free(void* block);

namespace LHA {
class CAllocator {                              // RTTI .?AVCAllocator@LHA@@, vtable 0x0129D264
public:
    virtual ~CAllocator() { }
    virtual void* Allocate(unsigned int size) = 0;
    virtual void Free(void* block) = 0;
};
}

class CVtable_012b2ab8 : public LHA::CAllocator {   // vtable 0x012B2AB8: malloc / free heap allocator
public:
    virtual void* Allocate(unsigned int size);
    virtual void Free(void* block);
};

void* CVtable_012b2ab8::Allocate(unsigned int size)
{
    return malloc(size);
}