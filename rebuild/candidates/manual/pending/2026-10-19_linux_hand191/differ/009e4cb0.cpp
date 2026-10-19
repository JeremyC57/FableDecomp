// CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) -- copy the path component that ends at the (index+1)-th backslash into `item` ("\" when it is the root); false when the path has fewer separators
// Grade: DIFFER(179v179) -- same shape as the NFile twin 0x00997880: the lane folds `start = j + 1 ; start != -1` to `cmp eax,-2` (retail `lea edx,[ecx+1] ; cmp edx,-1`) and swaps the scan index / character registers (eax/ecx); first attempt of the twin, parked with the NFile row
// Leaf: GetItemFromPath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4CB0, length 179 bytes, `ret 0Ch`
// (?GetItemFromPath@CDriveManager@@AAE_NPB_WKPA_W@Z: private member that never reads `this`; the NFile free-function
// twin is 0x00997880).
// Callees: wcscpy @ 0x00BFED5E (the static CRT, with L"\\" @ 0x0122DE78); the copy itself is the memcpy intrinsic
// (`rep movsd` + `rep movsb`).
// Shape: the forward scan reads each character into a wchar_t local (16-bit compares); counts backslashes and handles the hit inside the loop (VC places that block after the
// `return false` exit); the item start is searched backwards with a `start == -1` sentinel in the loop condition (the
// redundant `cmp edx,-1` after `start = j + 1`), defaulting to 0; the terminator is stored at the copied byte count.
#include <stddef.h>
#include <string.h>
#pragma intrinsic(memcpy)
extern "C" wchar_t* __cdecl wcscpy(wchar_t* destination, const wchar_t* source);   // 0x00BFED5E
#pragma function(wcscpy)

struct CSystemManager;
class CDriveManager {
    void* __vftable;                               // +0x00
    CSystemManager* SystemManager;                 // +0x04
    bool GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item);
};

bool CDriveManager::GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item)
{
    unsigned long separators = 0;
    *item = 0;
    long i = 0;
    for (wchar_t c = path[0]; c != 0; c = path[++i]) {
        if (c == L'\\' && ++separators > index) {
            if (i == 0 || path[i - 1] == L':') {
                wcscpy(item, L"\\");
                return true;
            }
            long start;
            for (long j = i - 1; ; --j) {
                if (j == -1) {
                    start = 0;
                    break;
                }
                wchar_t d = path[j];
                if (d == L'\\' || d == L':' || j == 0) {
                    start = j + 1;
                    if (start != -1)
                        break;
                }
            }
            memcpy(item, path + start, (i - start) * sizeof(wchar_t));
            item[i - start] = 0;
            return true;
        }
    }
    return false;
}
