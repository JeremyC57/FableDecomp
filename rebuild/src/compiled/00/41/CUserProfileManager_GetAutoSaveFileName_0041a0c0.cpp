// CUserProfileManager::GetAutoSaveFileName() -- return the data folder path L"Data\\Misc\\" as a CWideString (a static __fastcall getter: hidden result in ecx)
// Retail: Fable.exe @ 0x0041A0C0, 17 bytes. Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, literal @ 0x0122F3E8.
// Label note: the body builds only the constant folder L"Data\\Misc\\" (callers append the file name with
// operator+); "GetAutoSaveFileName" is a propagated manifest label.
// Shape: a by-value CWideString return with the hidden result pointer in ecx and no stack arguments = a static
// `__fastcall` function; the returned temporary is constructed straight into the result (`mov eax,esi`).
#include "fable_string.h"

struct CUserProfileManager {
    static CWideString __fastcall GetAutoSaveFileName();
};

CWideString __fastcall CUserProfileManager::GetAutoSaveFileName()
{
    return CWideString(L"Data\\Misc\\");
}