// NResourceDirectoryNames::GetSaveDir() -- the save-game folder: the user data folder (My Documents + L"My Games\\Fable\\") + L"Saves" + L"\\"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A540, 99 bytes (row padded to 112). FableWin.pdb:
// ?GetSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ (a `__fastcall` free function: hidden result in ecx, `ret`).
// Callees: GetMyDocumentsDir @ 0x0041A430, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (__fastcall: result in
// ecx, left in edx, right on the stack), ~CWideString @ 0x0099B510; literals 0x0122F504 L"My Games\\Fable\\",
// 0x0122F524 L"Saves", 0x0122DE78 L"\\".
// Shape: the same-TU helpers GetUserDataDir (out of line at 0x0041A4C0, landed under the propagated label
// CUserProfileManager::GetAutoSaveCheckPointFileName) and GetSaveDirWithoutTrailingBlackslash (0x0041A4F0) are inlined, which
// is why each intermediate CWideString is destroyed right after the next operator+ (the end of the inlined helper's own
// return statement) instead of at the end of one long expression; the three temporaries take the three frame words.
// The replaced bootstrap fixture copy of this function (rebuild/src/compiled/00/41/...GetSaveDir_0041a540.cpp) is an inline-assembly oracle, not reconstruction.
// Leaf: ?GetSaveDir@
#include "fable_string.h"

namespace NResourceDirectoryNames {
CWideString __fastcall GetMyDocumentsDir();                     // 0x0041A430

CWideString __fastcall GetUserDataDir()                         // 0x0041A4C0 (inlined here)
{
    return GetMyDocumentsDir() + L"My Games\\Fable\\";
}

CWideString __fastcall GetSaveDirWithoutTrailingBlackslash()    // 0x0041A4F0 (inlined here)
{
    return GetUserDataDir() + L"Saves";
}

CWideString __fastcall GetSaveDir()
{
    return GetSaveDirWithoutTrailingBlackslash() + L"\\";
}
}