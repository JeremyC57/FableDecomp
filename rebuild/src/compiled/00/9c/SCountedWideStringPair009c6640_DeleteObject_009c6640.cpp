// SCountedWideStringPair_009c6640::DeleteObject(SCountedWideStringPair_009c6640* object) -- the counted-pointer deleter of a record of two wide strings: destroy both strings and free the record, ignoring null
// Leaf: DeleteObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009c6640, 33 bytes.
// Callees: CWideString::~CWideString 0x0099B510 (twice: +4 then +0), operator delete 0x00BFE9BC.
// Identity: the catalog label `_Cons_val<allocator<CIntelligentPointer<const CThing>>,..>` is
// propagated and wrong.  The only references are absolute: two counted-pointer constructors
// (0x009C4AC5.., 0x009C6A8B..) build the 12-byte count block {1, 0x009C6640, object}; the block's
// destroy step calls this with the object in ecx.  The record type is not identified; it holds two
// CWideStrings (+0, +4).  Same shape as the landed deleter SCountedNamedRecord_008ebdd0::DeleteObject
// 0x008EBDD0: a `__fastcall` static (object in ecx, plain `ret`) with an explicit null test, the
// inline (implicit) destructor -- members in reverse order -- and `operator delete`.
#include "fable_string.h"

void operator delete(void* block);                 // 0x00BFE9BC

struct SCountedWideStringPair_009c6640 {
    CWideString m_first;                           // +0x00
    CWideString m_second;                          // +0x04
    static void __fastcall DeleteObject(SCountedWideStringPair_009c6640* object);
};

void __fastcall SCountedWideStringPair_009c6640::DeleteObject(SCountedWideStringPair_009c6640* object)
{
    if (object) {
        object->~SCountedWideStringPair_009c6640();
        operator delete(object);
    }
}