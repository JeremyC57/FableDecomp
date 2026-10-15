// SCountedNamedRecord_008ebdd0::DeleteObject(SCountedNamedRecord_008ebdd0* object) -- the counted-pointer deleter: destroy the record (its CCharString at +4) and free it, ignoring null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EBDD0, length 26 bytes.
// Leaf: DeleteObject
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated), operator delete @ 0x00BFE9BC.
// The chunk label "CTCMapwho::OnDie" is propagated over hundreds of byte-identical deleters: the only reference is the
// counted-pointer constructor @ 0x008EBE90, which allocates the 12-byte count block {1, 0x008EBDD0, object} -- the block's
// destroy step calls the deleter with the object in ecx (`mov ecx,[block+8] ; call [block+4]`). The record type is not
// identified; it holds a CCharString at +4. Shape: a `__fastcall` free function (object in ecx, plain `ret`) with an explicit
// null test, the inline destructor and `operator delete` (a bare `delete object` routes through an out-of-line `??_G`).
#include "fable_string.h"

void operator delete(void* block);                 // 0x00BFE9BC

struct SCountedNamedRecord_008ebdd0 {
    long        m_key;                             // +0x00
    CCharString m_name;                            // +0x04
    static void __fastcall DeleteObject(SCountedNamedRecord_008ebdd0* object);
};

void __fastcall SCountedNamedRecord_008ebdd0::DeleteObject(SCountedNamedRecord_008ebdd0* object)
{
    if (object) {
        object->~SCountedNamedRecord_008ebdd0();
        operator delete(object);
    }
}
