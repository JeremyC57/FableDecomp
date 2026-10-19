// CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy out the path component that ends at the (index+1)-th backslash; a root ("\" at the start or after a drive colon) yields "\"; false when the path has too few separators
// Grade: DIFFER(172v168) -- register assignment of the scan (retail c in cx, i in eax, count in edi) and the backward start search (retail has no start=-1 init; j+1 tested against -1 inside the loop)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997880, length 176 bytes (`__fastcall`: path in ecx, index in edx, item on the stack, ret 4).
// Callees: wcscat (MSVCR71 import thunk @ 0x00BFED5E, cdecl) for the root case; memcpy is the intrinsic `rep movsd/movsb`.
// Globals: the wide literal L"\\" @ 0x0122DE78.
// Shape: the separator scan keeps the current character in cx and the separator count in edi; the hit path sits after
// the `return false` tail. The backward search for the component start runs on unsigned indices compared with -1
// (`cmp ecx,-1`), and its "found" test is threaded straight into the copy. The member twin @ 0x009E4CB0 is the
// same body as a __thiscall that ignores `this` (ret 0Ch).
#include <string.h>
#include <wchar.h>

struct CDriveManager {
    static bool __fastcall GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
};

bool __fastcall CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
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
