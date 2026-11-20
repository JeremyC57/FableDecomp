// CCountedPointer<CWaveFile>::Reset(CWaveFile* data) -- release the shared count block (destroy the wave file and delete the block on the last reference), then adopt the new object with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCWaveFile@@@@QAEXPAVCWaveFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AADFF0, 103 bytes, thiscall, `ret 4`.
// Identity: engine/CCountedPointer.h Reset(T*) with T = CWaveFile: the CWaveLoader TU's only CCountedPointer<T>::Reset (LoadWaveFile
//   0x00AADB8A / landed 0x00AAD650 call it with the new CWaveFile); the block it builds stores CCountedPointer<CWaveFile>::DeleteData
//   0x00AADFC0. FableWin.pdb has the ?Reset@?$CCountedPointer@V..@@@@QAEXPAV..@@@Z family.
// Name fix (NF189, 2026-11-20, q6208): was the placeholder CCountedPointer_00aadfc0::Reset over local SCountBlock / Destroy_00aadfc0
//   stand-ins; now the shared header's default Reset (ReleaseData, Data = data, then a CCPPointerInfo(1, DeleteData, Data) re-read
//   through this) over the shared engine/CWaveFile.h, explicit instantiation of Reset only. Taking DeleteData's address emits the
//   header's inline DeleteData<CWaveFile> as a pick-any copy of the owner 0x00AADFC0.
#include "rebuild_abi.h"
#include "engine/CWaveFile.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<CWaveFile>::Reset(CWaveFile* data);
