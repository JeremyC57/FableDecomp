// CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy out the path component that ends at the (index+1)-th backslash; a root ("\" at the start or after a drive colon) yields "\"; false when the path has too few separators
// Grade: DIFFER(179v179) -- dl=19 (try d, was 186v179): `for(;;){ wchar_t c=path[i]; if(!c) return false; ...}` folds the end test; left: path/index esi-edi swap + start=-1 materialised
// Leaf: ?GetItemFromPath@CDriveManager@@AAE_NPB_WKPA_W@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E4CB0, 179 bytes (__thiscall, `this` unused, ret 0Ch).
// Identity: Ego_r.pdb ?GetItemFromPath@CDriveManager@@AAE_NPB_WKPA_W@Z (private member, native wchar_t); the same body as the
//   __fastcall NFile::GetItemFromPath 0x00997880.
// Callees: wcscat (MSVCR71 import thunk 0x00BFED5E, cdecl) for the root case; memcpy is the intrinsic `rep movsd/movsb`.
#include <string.h>
#include <wchar.h>
class CDriveManager {
    bool GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
};
bool CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
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
