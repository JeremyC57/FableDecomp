// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CMeshDataBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCMeshDataBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00428AE7, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc by Reset<CMeshDataBank> 0x0042FC86 and OpenMeshBank 0x00A09F20.
// Names (NF223, 2026-11-22; was the behavioural extern "C" DeleteIfSet, catalog _global::DeleteViaVtable0_00428ae7): FableWin.pdb, Ego_r.pdb and
//   Ego_d.pdb ?DeleteData@?$CCountedPointer@VCMeshDataBank@@@@SIXPAX@Z; both landed binders (0x0042FC86, 0x00A09F20) bind that spelling here.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T (CMeshDataBank : CBankFile, polymorphic under
//   FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS: virtual ~CBankFile slot 0); the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMESHDATABANK_BASES
#include "engine/CMeshDataBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CMeshDataBank>::DeleteData(void*);
