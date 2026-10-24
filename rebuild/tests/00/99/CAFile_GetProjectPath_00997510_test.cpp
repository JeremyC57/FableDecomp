// Behaviour test for 0x00997510 CAFile::GetProjectPath: module file name -> drive + directory (file name and
// extension dropped), formatted "%s%s" into the 260-character buffer and terminated at index 259.
#include <cstdio>
#include <cwchar>
static const wchar_t* LastOf(const wchar_t* s, wchar_t c)
{
    const wchar_t* found = 0;
    for (; *s; ++s) if (*s == c) found = s;
    return found;
}
static unsigned long FakeGetModuleFileName(void*, wchar_t* name, unsigned long size)
{
    const wchar_t text[] = L"C:\\Games\\Fable\\Fable.exe";
    unsigned long i = 0; for (; text[i] && i + 1 < size; ++i) name[i] = text[i]; name[i] = 0; return i;
}
static void FakeSplitPath(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t*, wchar_t*)
{
    drive[0] = path[0]; drive[1] = path[1]; drive[2] = 0;
    const wchar_t* last = LastOf(path, L'\\');
    unsigned int n = (unsigned int)(last - (path + 2)) + 1;
    for (unsigned int i = 0; i < n; ++i) dir[i] = path[2 + i]; dir[n] = 0;
}
static void GetProjectPath(wchar_t* out)
{
    wchar_t path[260]; wchar_t drive[10]; wchar_t dir[262];
    FakeGetModuleFileName(0, path, 260);
    FakeSplitPath(path, drive, dir, 0, 0);
    std::wcscpy(path, drive); std::wcscat(path, dir);
    path[259] = 0;
    std::wcscpy(out, path);
}
int main()
{
    int fails = 0;
    wchar_t out[300];
    GetProjectPath(out);
    if (std::wcscmp(out, L"C:\\Games\\Fable\\") != 0) ++fails;
    std::printf(fails ? "00997510_TEST FAIL\n" : "00997510_TEST PASS\n");
    return fails ? 1 : 0;
}