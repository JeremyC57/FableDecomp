// Behaviour test for 0x00997930 CAFile::GetFilenameFromPathname: the result is name + extension only; drive and
// directory are dropped; a path without an extension keeps just the name.
#include <cstdio>
#include <cwchar>
static const wchar_t* LastOf(const wchar_t* s, wchar_t c)
{
    const wchar_t* found = 0;
    for (; *s; ++s) if (*s == c) found = s;
    return found;
}
static void FakeSplit(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname, wchar_t* ext)
{
    drive[0] = 0; dir[0] = 0;
    const wchar_t* slash = LastOf(path, L'\\'); const wchar_t* name = slash ? slash + 1 : path;
    const wchar_t* dot = LastOf(name, L'.');
    unsigned int n = dot ? (unsigned int)(dot - name) : (unsigned int)std::wcslen(name);
    std::wcsncpy(fname, name, n); fname[n] = 0;
    std::wcscpy(ext, dot ? dot : L"");
}
static void GetFilenameFromPathname(const wchar_t* path, wchar_t* out)
{
    wchar_t d[3]; wchar_t di[256]; wchar_t f[256]; wchar_t e[256];
    FakeSplit(path, d, di, f, e);
    std::wcscpy(out, f); std::wcscat(out, e);
}
int main()
{
    int fails = 0;
    wchar_t out[300];
    GetFilenameFromPathname(L"C:\\Data\\Levels\\Town.wld", out);
    if (std::wcscmp(out, L"Town.wld") != 0) ++fails;
    GetFilenameFromPathname(L"Readme", out);
    if (std::wcscmp(out, L"Readme") != 0) ++fails;
    std::printf(fails ? "00997930_TEST FAIL\n" : "00997930_TEST PASS\n");
    return fails ? 1 : 0;
}