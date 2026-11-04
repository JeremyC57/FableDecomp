// Behaviour test for GetItemFromPath (0x00997880): a model of the row -- the component ending at the (index+1)-th backslash is
// copied out; a root (separator at the start or after a drive colon) yields "\\"; too few separators gives false.
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static bool Get(const wchar_t* path, unsigned long index, wchar_t* item)
{
    unsigned long separators = 0; item[0] = 0; unsigned long i = 0;
    while (path[i] != 0) { if (path[i] == L'\\' && ++separators > index) break; ++i; }
    if (path[i] == 0) return false;
    if (i == 0 || path[i - 1] == L':') { wcscat(item, L"\\"); return true; }
    unsigned long start = (unsigned long)-1, j = i - 1;
    while (start == (unsigned long)-1) {
        if (j == (unsigned long)-1) start = 0;
        else { if (path[j] == L'\\' || path[j] == L':' || j == 0) start = j + 1; --j; }
    }
    unsigned long length = i - start;
    memcpy(item, path + start, length * sizeof(wchar_t)); item[length] = 0;
    return true;
}
int main()
{
    wchar_t a[64], b[64], c[64], d[64];
    int ok = Get(L"C:\\Games\\Fable\\", 0, a) && wcscmp(a, L"\\") == 0
          && Get(L"C:\\Games\\Fable\\", 1, b) && wcscmp(b, L"Games") == 0
          && Get(L"C:\\Games\\Fable\\", 2, c) && wcscmp(c, L"Fable") == 0
          && !Get(L"C:\\Games", 1, d);
    printf(ok ? "00997880_TEST PASS\n" : "00997880_TEST FAIL\n");
    return ok ? 0 : 1;
}
