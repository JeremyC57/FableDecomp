// NFile::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy out the path component that ends at the (index+1)-th backslash; a root ("\" at the start or after a drive colon) yields "\"; false when the path has too few separators
// Grade: DIFFER(ours v retail) -- DIFFER(186v168) (try h): the Ego_r.pdb identity (NFile:: __fastcall, native wchar_t) and a backward search with the decrement in the not-found arm fix most of the body; left: retail caches the scanned character in cx while keeping separators edi / path esi / index eax -- a character variable here permutes all four registers, without one VC7.1 re-tests path[i] after the scan and aligns the loop (186v168); the start search still materialises start = -1 (`or edx,-1`) and `length * sizeof(wchar_t)` is `add eax,eax` where retail has `shl eax,1`.
// Leaf: ?GetItemFromPath@NFile@@YI_NPB_WKPA_W@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00997880, 168 bytes (`__fastcall`: path in ecx, index in edx, item on the stack, ret 4).
// Identity: Ego_r.pdb ?GetItemFromPath@NFile@@YI_NPB_WKPA_W@Z (a __fastcall free function in namespace NFile, native wchar_t);
//   the __thiscall twin 0x009E4CB0 is ?GetItemFromPath@CDriveManager@@AAE_NPB_WKPA_W@Z.
// Callees: wcscat (MSVCR71 import thunk 0x00BFED5E, cdecl) for the root case; memcpy is the intrinsic `rep movsd/movsb`.
// Globals: the wide literal L"\\" @ 0x0122DE78.
#include <string.h>
#include <wchar.h>
namespace NFile {
    bool __fastcall GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
}
bool __fastcall NFile::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
{
    unsigned long separators = 0;
    item[0] = 0;
    unsigned long i = 0;
    while (path[i] != 0) {
        if (path[i] == L'\\' && ++separators > index)
            break;
        ++i;
    }
    if (path[i] == 0)
        return false;
    if (i == 0 || path[i - 1] == L':') {
        wcscat(item, L"\\");
        return true;
    }
    unsigned long start = (unsigned long)-1;
    unsigned long j = i - 1;
    while (start == (unsigned long)-1) {
        if (j == (unsigned long)-1)
            start = 0;
        else {
            if (path[j] == L'\\' || path[j] == L':' || j == 0)
                start = j + 1;
            --j;
        }
    }
    unsigned long length = i - start;
    memcpy(item, path + start, length * sizeof(wchar_t));
    item[length] = 0;
    return true;
}
