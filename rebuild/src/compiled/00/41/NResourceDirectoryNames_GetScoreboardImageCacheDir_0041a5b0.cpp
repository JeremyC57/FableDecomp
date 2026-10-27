// NResourceDirectoryNames::GetScoreboardImageCacheDir() -- the user data folder (My Documents + L"My Games\\Fable\\") + L"Scoreboard\\"
// Leaf: ?GetScoreboardImageCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A5B0, 72 bytes, plain `ret` (`__fastcall` free function: hidden result in ecx).
// Identity (proven): FableWin.pdb ?GetScoreboardImageCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ -- the PDB getter order around the
//   landed GetMyDocumentsDir 0x0041A430 / GetSaveDir 0x0041A540 / GetTattooDataDir 0x0041A6A0 matches retail, and the literal
//   0x0122F530 is L"Scoreboard\\".
// Callees: GetMyDocumentsDir 0x0041A430, operator+(const CWideString&, const wchar_t*) 0x0099BF30 (__fastcall: result ecx,
//   left edx, right on the stack), ~CWideString 0x0099B510; L"My Games\\Fable\\" 0x0122F504.
// Shape (family K083, representative 0x0041A4F0): the landed GetSaveDir 0x0041A540 one level shorter: the same-TU
//   GetUserDataDir (out of line at 0x0041A4C0) is inlined, its temporary destroyed right after the next operator+.
#include "fable_string.h"

namespace NResourceDirectoryNames {
CWideString __fastcall GetMyDocumentsDir();                     // 0x0041A430

CWideString __fastcall GetUserDataDir()                         // 0x0041A4C0 (inlined here)
{
    return GetMyDocumentsDir() + L"My Games\\Fable\\";
}

CWideString __fastcall GetScoreboardImageCacheDir()
{
    return GetUserDataDir() + L"Scoreboard\\";
}
}