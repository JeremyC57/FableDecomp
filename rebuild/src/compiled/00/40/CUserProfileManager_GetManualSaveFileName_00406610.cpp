// CUserProfileManager::GetManualSaveFileName @ 0x00406610
// Leaf: ?GetManualSaveFileName@CUserProfileManager@@QBE?AVCWideString@@J@Z
// Name-fix pass (2026-11-12): the return type is the shared CWideString (fable_string.h; Ego_r.pdb 0x00405270 / FableWin.pdb 0x0187B5B0
//   ?GetManualSaveFileName@CUserProfileManager@@QBE?AVCWideString@@J@Z) in place of the local 4-byte CWS stand-in.
// NF143 (2026-11-20, q4285): ported off the CWSraw stand-in and its extern "C" seams onto fable_string.h. Retail 0x00406610..0x00406685 is
//   `GetManualSaveBaseFileName() + GFIntToWideString(slot)`: the right operand first (default ctor 0x0099AED0, Sprintf(L"%d") 0x0099BA70
//   with the literal 0x0122DE44, copy ctor 0x0099B720 into the argument temporary, destructor 0x0099B510: the inline GFIntToWideString of
//   engine/GFIntToWideString.h), then the left operand, its TU-mate GetManualSaveBaseFileName 0x004065F0 expanded (L"Save" 0x0122DE90
//   through ??0CWideString@@QAE@PBG@Z 0x0099B6B0; Ego_r.pdb / FableWin.pdb donor order puts it right before this function), then the
//   fastcall operator+ ??H@YI?AVCWideString@@ABV0@0@Z 0x0099BE70 into the hidden result, then both temporaries destroyed.
//   Form 1, `CWideString(L"Save") + ...`, graded DIFFER(118v120): an explicit temporary pushes the constructor's eax and takes the other slot.
//   Was: ?fromLit@CWSraw@@QAEXPBG@Z (0x0099B6B0), the free seams WSctor / WSdtor / FmtWide / AddWide and the data stand-ins
//   kFmtSave_0122de44 / kSavePrefix_0122de90 (now the wide literals).
//   One definition: this object emits the header inline as a pick-any COMDAT, as does the owner row 0x004065F0 (scaffolding).
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/GFIntToWideString.h"

// CUserProfileManager: the shared engine/CUserProfileManager.h (SAVENAMES opt-in declares GetManualSaveFileName) in place of the local model.
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE
#include "engine/CUserProfileManager.h"

CWideString CUserProfileManager::GetManualSaveFileName(long slot) const
{
    return GetManualSaveBaseFileName() + GFIntToWideString(slot);
}
