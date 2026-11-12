// CInputProcessGameBase::AddToInterface(CInputProcessManager& manager) -- remember the manager as the game player interface, then the base registration
// Leaf: ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00687A70, 16 bytes: `mov eax,[esp+4] ; mov [ecx+24h],eax ; mov [esp+4],eax ; jmp 0x00A0D2B0`.
// NF28 (2026-11-12, queue land1011eh): was landed through the stand-in CInputProcessGameBase_Methods (non-virtual, pointer argument)
//   calling Helper. Identity: Ego_r.pdb 0x006B2B80 / FableWin.pdb 0x021D3CB0 ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z;
//   the tail jump is CAInputProcess::AddToInterface 0x00A0D2B0 (Ego_r.pdb 0x00B74BA0 / FableWin.pdb 0x0318F2E0, the base override);
//   the landed CInputProcessZTarget::AddToInterface 0x00688D40 calls this one by name with the same reference signature.
// Model: engine/CInputProcessGameBase.h + engine/CAInputProcess.h (FABLE_RETAIL_INPUTPROCESS, FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE);
//   PGamePlayerInterface +0x24 stores the manager (the game player interface is the input process manager it is added to).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::AddToInterface(CInputProcessManager& manager)
{
    PGamePlayerInterface = (CGamePlayerInterface*)&manager;
    return CAInputProcess::AddToInterface(manager);
}
