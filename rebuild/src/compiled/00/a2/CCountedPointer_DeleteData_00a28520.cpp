// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<C3DMeshStripifier>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DMeshStripifier@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A28520, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A28520`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a28520, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VC3DMeshStripifier@@@@SIXPAX@Z; stored by Reset 0x00A29FC0 (reloc_bindings stand-in ?Reset@CMeshStripifierPointer@@QAEXPAVC3DMeshStripifier@@@Z), called at 0x00A27252 with a fresh C3DMeshStripifier (ctor 0x00A8DF70, RTTI vtable); Ego_r.pdb CMeshDataBank::MeshStripifier is CCountedPointer<C3DMeshStripifier>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class C3DMeshStripifier {
public:
    virtual ~C3DMeshStripifier();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<C3DMeshStripifier>::DeleteData(void*);
