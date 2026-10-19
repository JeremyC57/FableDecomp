// CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy out the path component that ends at the (index+1)-th backslash; a root ("\" at the start or after a drive colon) yields "\"; false when the path has too few separators
// Grade: DIFFER(180v179) -- same register-assignment divergence as the fastcall twin 0x00997880 (retail path in esi, c in cx, i in eax)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4CB0, length 179 bytes (__thiscall, `this` unused, ret 0Ch).
// Callees: wcscat (MSVCR71 import thunk @ 0x00BFED5E, cdecl) for the root case; memcpy is the intrinsic `rep movsd/movsb`.
// Globals: the wide literal L"\\" @ 0x0122DE78.
// Shape: the separator scan keeps the current character in cx and the separator count in edi; the hit path sits after
// the `return false` tail. The backward search for the component start runs on unsigned indices compared with -1
// (`cmp ecx,-1`), and its "found" test is threaded straight into the copy. The `__fastcall` twin @ 0x00997880 is the
// same body with path/index in ecx/edx.
#include <string.h>
#include <wchar.h>

struct CDriveManager {
    bool GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
};

bool CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
{
    unsigned long separators = 0;
    item[0] = 0;
    unsigned long i;
    wchar_t c;
    for (i = 0; (c = path[i]) != 0; ++i) {
        if (c == L'\\' && ++separators > index)
            break;
    }
    if (c == 0)
        return false;
    if (i == 0 || path[i - 1] == L':') {
        wcscat(item, L"\\");
        return true;
    }
    unsigned long start = (unsigned long)-1;
    for (unsigned long j = i - 1; start == (unsigned long)-1; --j) {
        if (j == (unsigned long)-1)
            start = 0;
        else if (path[j] == L'\\' || path[j] == L':' || j == 0)
            start = j + 1;
    }
    unsigned long length = i - start;
    memcpy(item, path + start, length * sizeof(wchar_t));
    item[length] = 0;
    return true;
}
