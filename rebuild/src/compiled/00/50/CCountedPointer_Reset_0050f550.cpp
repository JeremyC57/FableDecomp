// CCountedPointer<CAFile>::Reset(CAFile* data) -- release the current file (its count block's deleter on the last reference, then the
//   block), then hold the new one with a fresh count block (re-read through `this`).
// Leaf: ?Reset@?$CCountedPointer@VCAFile@@@@QAEXPAVCAFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050F550.
// Identity (NF56, round 7, queue q10465 binder): naming wave 1 decision line 131 names this Reset as the one that passes the CAFile deleter
//   0x0050BE40 (?DeleteData@?$CCountedPointer@VCAFile@@@@SIXPAX@Z, FableWin.pdb 0x01CB0BA0) to the new CCPPointerInfo; the landed caller
//   CWorldMap::GetLevelDimensions 0x00506E30 already binds ?Reset@?$CCountedPointer@VCAFile@@@@QAEXPAVCAFile@@@Z. engine/CCountedPointer.h
//   Reset body; the landed DeleteData / Reset rows explicitly instantiate the template member (the 0x00B8EC10 pattern).
//   Was the stand-in CCountedPointer_0050be40::Reset(void*) over local SCountBlock / CCountedPointer_0050be40 models and the stand-in
//   deleter Destroy_0050be40.
#include "engine/CAFile.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<CAFile>::Reset(CAFile*);
