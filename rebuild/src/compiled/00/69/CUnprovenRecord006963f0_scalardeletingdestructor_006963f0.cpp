// CUnprovenRecord_006963f0::`scalar deleting destructor'(unsigned int flags) -- run the implicit destructor inline (release the counted pointer at +0x04, clearing Data then Info; then CCharString::~CCharString 0x0099EAE0 (on +0x00)) and free the object when bit 0 of the flags is set
// Leaf: ??_GCUnprovenRecord_006963f0@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006963f0, 81 bytes, thiscall, `ret 4`, returns this.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT) and no vtable holds it; its own bytes destroy a CCountedPointer at +0x04 and then a CCharString at +0x00, so the record is a non-polymorphic {CCharString, CCountedPointer} (VC7.1 emits a ??_G for such a class wherever it is deleted); types unproven.
// Callees: the CCPPointerInfo deleter (fastcall), operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (on +0x00).
// Shape (family L036): ~CCountedPointer inline (`--Info->RefCount`, deleter, delete Info, Data = Info = 0), then the
//   base or first-member destructor out of line; the `delete p` makes VC7.1 emit the non-virtual ??_G.
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
private:
    void* PStringData;                                    // +0x00
};
class CUnprovenPointee_006963f0;                          // descriptive: the pointee is unproven
class CUnprovenRecord_006963f0 {                           // descriptive: the record type is unproven
public:
    CCharString Name;                                     // +0x00
    CCountedPointer<CUnprovenPointee_006963f0> Pointer;   // +0x04
};
void DeleteCUnprovenRecord_006963f0(CUnprovenRecord_006963f0* p) { delete p; }