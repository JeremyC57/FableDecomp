// CCountedPointer<NCameraModes::CBaseInit>::DeleteData(void*) -- the per-T count-block deleter: delete (T*)data.
// Leaf: ?DeleteData@?$CCountedPointer@VCBaseInit@NCameraModes@@@@SIXPAX@Z
// NF37 (2026-11-16): moved off its free stand-in onto engine/CCountedPointer.h + engine/NCameraModes_CBaseInit.h. Evidence: FableWin.pdb name; the camera manager constructor stores 0x0069BBA0 as the CCPPointerInfo DeleteFunc of every CCountedPointer<NCameraModes::CBaseInit> it builds (0x0069B0D9 and five more); body = operator delete of the pointee (no virtual dtor).
#pragma optimize("s",on)
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
#include "engine/NCameraModes_CBaseInit.h"

template void FABLE_FASTCALL CCountedPointer<NCameraModes::CBaseInit>::DeleteData(void*);
