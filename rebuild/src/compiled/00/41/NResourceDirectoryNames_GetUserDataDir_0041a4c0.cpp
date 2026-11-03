// NResourceDirectoryNames::GetUserDataDir() -- the user's data folder: the documents folder + L"My Games\\Fable\\" (__fastcall, CWideString returned through the hidden pointer in ecx)
// Leaf: ?GetUserDataDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe @ 0x0041A4C0, 41 bytes (+ int3 padding).
// Identity (naming wave 34): Ego_r.pdb 0x0042BD10 / FableWin.pdb 0x019135D0 ?GetUserDataDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ,
// donor order directly after GetMyDocumentsDir (Ego_r 0x0042BCB0 = retail 0x0041A430, the callee); the body appends the
// "My Games\Fable\" folder to the documents folder. Its caller CUserProfileManager::IsEnoughSpaceForProfile (0x004067C0) checks
// the free space of this folder. (Was landed as CUserProfileManager::GetAutoSaveCheckPointFileName, a propagated label.)
// Callees: NResourceDirectoryNames::GetMyDocumentsDir @ 0x0041A430 (__fastcall, SHGetFolderPathW(CSIDL_PERSONAL) into a
// CWideString), operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (__fastcall: result in ecx, left in edx,
// right on the stack), CWideString::~CWideString @ 0x0099B510; literal @ 0x0122F504.
// Shape: `return GetMyDocumentsDir() + L"..."`: the literal is pushed first (right-to-left), the folder temporary
// lives in the `push ecx` frame slot and is destroyed after operator+ built the result in place.
#include "fable_string.h"

namespace NResourceDirectoryNames {
    CWideString __fastcall GetMyDocumentsDir();       // 0x0041A430
    CWideString __fastcall GetUserDataDir();
}

CWideString __fastcall NResourceDirectoryNames::GetUserDataDir()
{
    return GetMyDocumentsDir() + L"My Games\\Fable\\";
}
