// Behaviour test for 0x00997620 CAFile::SplitPath (CWideString): every non-null output receives its part of the path;
// null outputs are skipped without touching the others.
#include <cstdio>
#include <cwchar>
static const wchar_t* LastOf(const wchar_t* s, wchar_t c)
{
    const wchar_t* found = 0;
    for (; *s; ++s) if (*s == c) found = s;
    return found;
}
struct CWideString { wchar_t buf[300]; int assigns; CWideString() : assigns(0) { buf[0] = 0; }
    CWideString& operator=(const wchar_t* t) { std::wcscpy(buf, t); ++assigns; return *this; } };
static void FakeSplit(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname, wchar_t* ext)
{
    // "C:\dir\name.ext" only
    std::wcsncpy(drive, path, 2); drive[2] = 0;
    const wchar_t* slash = LastOf(path, L'\\'); const wchar_t* dot = LastOf(path, L'.');
    unsigned int n = (unsigned int)(slash + 1 - (path + 2)); std::wcsncpy(dir, path + 2, n); dir[n] = 0;
    n = (unsigned int)(dot - (slash + 1)); std::wcsncpy(fname, slash + 1, n); fname[n] = 0;
    std::wcscpy(ext, dot);
}
static void SplitPath(const wchar_t* path, CWideString* drive, CWideString* dir, CWideString* fname, CWideString* ext)
{
    wchar_t d[3]; wchar_t di[256]; wchar_t f[256]; wchar_t e[256];
    FakeSplit(path, d, di, f, e);
    if (drive) *drive = d;
    if (dir) *dir = di;
    if (fname) *fname = f;
    if (ext) *ext = e;
}
int main()
{
    int fails = 0;
    CWideString a, b, c, d;
    SplitPath(L"C:\\Data\\Levels\\Town.wld", &a, &b, &c, &d);
    if (std::wcscmp(a.buf, L"C:") || std::wcscmp(b.buf, L"\\Data\\Levels\\") || std::wcscmp(c.buf, L"Town") ||
        std::wcscmp(d.buf, L".wld")) ++fails;
    CWideString e, f;
    SplitPath(L"D:\\x\\y.z", 0, &e, 0, &f);
    if (std::wcscmp(e.buf, L"\\x\\") || std::wcscmp(f.buf, L".z") || e.assigns != 1 || f.assigns != 1) ++fails;
    std::printf(fails ? "00997620_TEST FAIL\n" : "00997620_TEST PASS\n");
    return fails ? 1 : 0;
}