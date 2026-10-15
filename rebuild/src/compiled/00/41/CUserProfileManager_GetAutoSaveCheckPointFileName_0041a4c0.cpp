// CUserProfileManager::GetAutoSaveCheckPointFileName() -- return the user's documents folder + L"My Games\\Fable\\" (static __fastcall getter: hidden result in ecx)
// Retail: Fable.exe @ 0x0041A4C0, 41 bytes (+ int3 padding).
// Callees: the documents-folder getter @ 0x0041A430 (static __fastcall, SHGetFolderPathW(CSIDL_PERSONAL) into a
// CWideString), operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (__fastcall: result in ecx, left in edx,
// right on the stack), CWideString::~CWideString @ 0x0099B510; literal @ 0x0122F504.
// Label note: the body returns the save-game root folder, not a checkpoint file name ("GetAutoSaveCheckPointFileName"
// is a propagated manifest label).
// Shape: `return GetDocumentsFolder() + L"..."`: the literal is pushed first (right-to-left), the folder temporary
// lives in the `push ecx` frame slot and is destroyed after operator+ built the result in place.
#include "fable_string.h"

struct CUserProfileManager {
    static CWideString __fastcall GetDocumentsFolder();               // 0x0041A430
    static CWideString __fastcall GetAutoSaveCheckPointFileName();
};

CWideString __fastcall CUserProfileManager::GetAutoSaveCheckPointFileName()
{
    return GetDocumentsFolder() + L"My Games\\Fable\\";
}