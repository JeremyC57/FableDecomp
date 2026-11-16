// CCountedPointer<NUISystem::CAction>::DeleteData(void*) -- the per-T count-block deleter: delete (T*)data.
// Leaf: ?DeleteData@?$CCountedPointer@VCAction@NUISystem@@@@SIXPAX@Z
// NF37 (2026-11-16): moved off its free stand-in onto engine/CCountedPointer.h + engine/NUISystem_CAction.h. Evidence: Ego_r.pdb (0x00438DF1) + FableWin.pdb per-T deleter; retail stores this address in the count block of every expanded CCountedPointer<CAction>(T*); body = ~CAction 0x0053FD00 then operator delete.
#pragma optimize("s",on)
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CAction.h"

template void FABLE_FASTCALL CCountedPointer<NUISystem::CAction>::DeleteData(void*);
