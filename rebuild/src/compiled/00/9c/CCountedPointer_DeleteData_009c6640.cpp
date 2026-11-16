// CCountedPointer<const CFileInstaller::CFileInfo>::DeleteData(void* data) -- the counted-pointer deleter of a file record: destroy its two wide
// strings (DstPath +4, SrcPath +0) and free it, ignoring null
// Leaf: ?DeleteData@?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6640. Callees: CWideString::~CWideString 0x0099B510 (twice), operator delete 0x00BFE9BC.
// Only its address is used: CCountedPointer<const CFileInfo>(T*) 0x009C6A70 and AddFile 0x009C47A0 store it as the count block's
// DeleteFunc ({1, 0x009C6640, object}).
// NF39 (2026-11-16): moved off its record stand-in (SCountedWideStringPair_009c6640::DeleteObject) onto the shared engine/CCountedPointer.h
//   DeleteData (`delete static_cast<T*>(data)`; the explicit instantiation emits the COMDAT, as the landed DeleteData<CParticleSimulationRenderData>
//   0x00BB6330), T the nested CFileInstaller::CFileInfo of engine/CFileInstaller.h (SrcPath / DstPath CWideString +0 / +4): Ego_r.pdb
//   0x00B149C0 + FableWin.pdb 0x0304E5B0 ?DeleteData@?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@SIXPAX@Z; overlay opt-in
//   FABLE_RETAIL_CFILEINSTALLER_CFILEINFO_INLINE_DTOR makes ~CFileInfo visible (retail expands it here).
#define FABLE_RETAIL_CFILEINSTALLER_CFILEINFO_INLINE_DTOR
#include "engine/CCountedPointer.h"
#include "engine/CFileInstaller.h"

template void FABLE_FASTCALL CCountedPointer<const CFileInstaller::CFileInfo>::DeleteData(void*);
