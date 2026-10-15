// CUserProfileManager::GetAutoSaveFileName() -- virtual getter that returns an empty CWideString (L"")
// Retail: Fable.exe @ 0x007D7C10, 23 bytes (+ int3 padding), `ret 4`. Callees: CWideString::CWideString(const wchar_t*)
// @ 0x0099B6B0, empty literal @ 0x0122D70C.
// Label note: the only reference is slot 42 of the CTCTavernGameSpotTheAddition vtable (0x0126F814 + 42*4 =
// 0x0126F8BC), so this is a tavern-game component's string getter, not a profile-manager function
// ("GetAutoSaveFileName" is a propagated manifest label).
// Shape: a __fastcall member returning a class by value: the hidden result pointer is the first stack argument
// (`mov esi,[esp+8]`), `this` is unused, the temporary is constructed in place.
#include "fable_string.h"

struct CUserProfileManager {
    CWideString GetAutoSaveFileName();
};

CWideString CUserProfileManager::GetAutoSaveFileName()
{
    return CWideString(L"");
}