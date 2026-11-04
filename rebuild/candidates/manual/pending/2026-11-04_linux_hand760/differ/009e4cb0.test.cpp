// Behaviour test for 0x009E4CB0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <cwchar>
static bool GetItemFromPath(const wchar_t* path, unsigned long index, wchar_t* item) {
    unsigned long separators = 0; item[0] = 0; unsigned long i = 0;
    for (;;) { wchar_t c = path[i]; if (c == 0) return false; if (c == L'\\' && ++separators > index) break; ++i; }
    if (i == 0 || path[i - 1] == L':') { std::wcscat(item, L"\\"); return true; }
    unsigned long start = (unsigned long)-1, j = i - 1;
    while (start == (unsigned long)-1) { if (j == (unsigned long)-1) start = 0; else { if (path[j] == L'\\' || path[j] == L':' || j == 0) start = j + 1; --j; } }
    if (start == 1 && path[0] != L'\\' && path[0] != L':') start = 0;
    std::wcsncat(item, path + start, i - start); return true;
}
int main() {
    wchar_t item[64];
    bool ok = GetItemFromPath(L"c:\\games\\fable\\", 0, item) && std::wcscmp(item, L"\\") == 0;
    ok = ok && GetItemFromPath(L"c:\\games\\fable\\", 1, item) && std::wcscmp(item, L"games") == 0;
    ok = ok && GetItemFromPath(L"c:\\games\\fable\\", 2, item) && std::wcscmp(item, L"fable") == 0;
    ok = ok && !GetItemFromPath(L"c:\\games", 3, item);
    std::printf(ok ? "009e4cb0_TEST PASS\n" : "009e4cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
