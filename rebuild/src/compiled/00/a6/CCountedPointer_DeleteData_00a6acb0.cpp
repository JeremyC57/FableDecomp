// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CVolumeTexture>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6ACB0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A6ACB0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a6acb0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@SIXPAX@Z; stored by CCountedPointer<CVolumeTexture>::Reset 0x00A6AE60 (reloc_bindings, caller 0x00A699D0); Ego_r.pdb CManagedTextureData::VolumeTexture is CCountedPointer<CVolumeTexture>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CVolumeTexture {
public:
    virtual ~CVolumeTexture();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CVolumeTexture>::DeleteData(void*);
