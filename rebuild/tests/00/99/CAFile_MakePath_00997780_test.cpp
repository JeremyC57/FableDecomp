// Behaviour test for 0x00997780 CAFile::MakePath: the four parts are handed to the CRT makepath in drive, dir, fname,
// ext order and the 260-character result is returned.
#include <cstdio>
#include <cwchar>
static void FakeMakePath(wchar_t* out, const wchar_t* drive, const wchar_t* dir, const wchar_t* fname, const wchar_t* ext)
{
    std::wcscpy(out, drive); std::wcscat(out, dir); std::wcscat(out, fname); std::wcscat(out, ext);
}
class CWideString { public: wchar_t buf[300]; CWideString(const wchar_t* t) { std::wcscpy(buf, t); } };
static CWideString MakePath(const wchar_t* drive, const wchar_t* dir, const wchar_t* fname, const wchar_t* ext)
{
    wchar_t path[260];
    FakeMakePath(path, drive, dir, fname, ext);
    return CWideString(path);
}
int main()
{
    int fails = 0;
    CWideString r = MakePath(L"C:", L"\\Saves\\", L"Hero", L".sav");
    if (std::wcscmp(r.buf, L"C:\\Saves\\Hero.sav") != 0) ++fails;
    std::printf(fails ? "00997780_TEST FAIL\n" : "00997780_TEST PASS\n");
    return fails ? 1 : 0;
}