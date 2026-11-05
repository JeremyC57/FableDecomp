// CUnprovenRecord_00b43ef0::~CUnprovenRecord_00b43ef0() -- implicit destructor: release the counted pointer member (+0xC) -- drop the shared count, on zero run the deleter and free the count block, then clear it
// Leaf: ??1CUnprovenRecord_00b43ef0@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b43ef0, 49 bytes, thiscall, plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT): the implicit destructor of a record whose only non-trivial member is a CCountedPointer at +0x0C; the record and pointee types are unproven.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L028): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CUnprovenPointee_00b43ef0;                                               // descriptive: the pointee is unproven
class CUnprovenRecord_00b43ef0 {                                              // descriptive: the record type is unproven
public:
    unsigned char Head[0xC];                              // +0x00 (trivially destroyed)
    CCountedPointer<CUnprovenPointee_00b43ef0> Pointer;                        // +0x0C
    ~CUnprovenRecord_00b43ef0();
};
CUnprovenRecord_00b43ef0::~CUnprovenRecord_00b43ef0()
{
}