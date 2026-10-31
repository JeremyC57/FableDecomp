// CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy out the path component that ends at the (index+1)-th backslash; a root ("\" at the start or after a drive colon) yields "\"; false when the path has too few separators
// Grade: DIFFER(178v168) -- the scan as a for(;;) returning false at the terminator, and the backward start search with
//   no -1 init (start = 0 on j == -1, start = j + 1 tested against -1 only where it is set): 54 -> 22 diff lines;
//   residue: i/c in ecx/ax (retail eax/cx) and the backward loop is rotated (retail jumps back to the j == -1 test; ours
//   tests j != -2 / dec / cmp -1 at the bottom with an alignment pad)
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
    unsigned long i = 0;
    for (;;) {
        wchar_t c = path[i];
        if (c == 0)
            return false;
        if (c == L'\\' && ++separators > index)
            break;
        ++i;
    }
    if (i == 0 || path[i - 1] == L':') {
        wcscat(item, L"\\");
        return true;
    }
    unsigned long start;
    unsigned long j = i - 1;
    for (;;) {
        if (j == (unsigned long)-1) {
            start = 0;
            break;
        }
        wchar_t c = path[j];
        if (c == L'\\' || c == L':' || j == 0) {
            start = j + 1;
            if (start != (unsigned long)-1)
                break;
        }
        --j;
    }
    unsigned long length = i - start;
    memcpy(item, path + start, length * sizeof(wchar_t));
    item[length] = 0;
    return true;
}
