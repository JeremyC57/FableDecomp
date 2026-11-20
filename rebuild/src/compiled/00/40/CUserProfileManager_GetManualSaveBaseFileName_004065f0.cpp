// CUserProfileManager::GetManualSaveBaseFileName() const -- the base file name of a manual save, L"Save"
// Leaf: ?GetManualSaveBaseFileName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x004065F0, 23 bytes, __fastcall `ret 4` (hidden CWideString result, this unused).
// Identity: Ego_r.pdb 0x00405250 / FableWin.pdb 0x0187B570 ?GetManualSaveBaseFileName@CUserProfileManager@@QBE?AVCWideString@@XZ; donor
// order GetManualSaveBaseFileName, GetManualSaveFileName = retail 0x004065F0, 0x00406610, and 0x00406610 expands this body as the left
// operand of its operator+ (L"Save" at 0x0122DE90 through 0x0099B6B0). Nothing else references 0x004065F0.
// Callee: CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (bound ??0CWideString@@QAE@PBG@Z).
// NF143 (2026-11-20, q4285, one definition): the body is the shared engine/CUserProfileManager.h inline (opt-in
//   FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE), which 0x00406610 expands; this row emits it out of line as a
//   pick-any COMDAT (the nf86 / nf92 scaffolding below) instead of the local CWideString / CUserProfileManager models it had.
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE
#include "engine/CUserProfileManager.h"

namespace {
// Fidelity: scaffolding, not retail: a call of the header's inline GetManualSaveBaseFileName with inline expansion switched off for this
// one function, so this object emits the body out of line as a pick-any COMDAT (retail's unreferenced copy 0x004065F0). Internal to this
// TU; never called.
#pragma inline_depth(0)
CWideString ReferenceGetManualSaveBaseFileName(const CUserProfileManager& manager)
{
    return manager.GetManualSaveBaseFileName();
}
#pragma inline_depth()
}
