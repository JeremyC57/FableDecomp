// CUserProfileManager::GetManualSaveDisplayName(long slot) const -- the localised "manual save <n>" name of a save slot: the
// TEXT_GUI_SAVE_MANUAL_SAVE_NAME entry of the main game component's text bank, else of the new front end's, followed by a space
// and the slot number; with neither component up, the slot's file name.
// Leaf: ?GetManualSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00406AE0, length 314 bytes, `ret 8` (hidden CWideString result).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString() 0x0099AED0,
// CWideString::Sprintf 0x0099BA70, CWideString(const CWideString&) 0x0099B720, ~CWideString 0x0099B510,
// NGameText::CDataBank::GetTextBySymbol 0x009C95E0, operator+(const CWideString&, const wchar_t*) 0x0099BF30,
// operator+(const CWideString&, const CWideString&) 0x0099BE70, GFIntToWideString 0x00405F00 (the inline's out-of-line copy,
// emitted by this TU: the first arm expands it, the second calls it), CUserProfileManager::GetManualSaveFileName 0x00406610.
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (text bank +0x14), CNewFrontendGameComponent::m_pNewFrontendGameComponent
// 0x013B871C (text bank +0x60); "TEXT_GUI_SAVE_MANUAL_SAVE_NAME" 0x0122DF9C, L"%d" 0x0122DE44, L" " 0x0122D820.
// Shape: the symbol is an implicit CCharString temporary of the return statement (GetTextBySymbol takes const CCharString&), not
// a named local: VC7.1 builds class temporaries of a full expression first, so its constructor still runs before the number, but
// as a temporary of the same statement it interferes with the dead `slot` word and keeps the frame at five words (0x14) with the
// second arm reusing the first arm's slots, which is what lets the two destructor tails merge. A named `CCharString symbol`
// homes the second arm's operator+ temporary in the dead argument word and the tails stay apart (DIFFER 344v314).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CUserProfileManager.h"
inline CWideString __fastcall GFIntToWideString(long value)    // 0x00405F00
{
    CWideString result;
    result.Sprintf(L"%d", value);
    return result;
}
CWideString CUserProfileManager::GetManualSaveDisplayName(long slot) const
{
    if (const CMainGameComponent* game = CMainGameComponent::Get()) {
        return game->GetTextBank().GetTextBySymbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME") + L" " + GFIntToWideString(slot);
    }
    if (const CNewFrontendGameComponent* frontEnd = CNewFrontendGameComponent::Get()) {
        return frontEnd->GetTextBank().GetTextBySymbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME") + L" " + GFIntToWideString(slot);
    }
    return GetManualSaveFileName(slot);
}