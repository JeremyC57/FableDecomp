// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NShape::CShape>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCShape@NShape@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00763A10, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00763A10`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00763a10, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCShape@NShape@@@@SIXPAX@Z; stored by Reset 0x007642C0 (reloc_bindings stand-in ?Reset@CShapePointer@NShape@@QAEXPAVCShape@2@@Z, caller 0x007624E0), whose callers 0x00762541 / 0x00762A40 pass a fresh NShape::CBasicShape / CTrack (RTTI vtables of the ctors 0x00762440 / 0x00761F40), and by InitialiseFrom 0x00762AB0 / ShareData 0x00764180.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
namespace NShape {
class CShape {
public:
    virtual ~CShape();                                   // slot 0 (scalar deleting destructor)
};
}

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NShape::CShape>::DeleteData(void*);
