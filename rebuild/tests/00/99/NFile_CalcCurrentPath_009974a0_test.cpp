// Behaviour test for 0x009974A0 NFile::CalcCurrentPath: asks the CRT for the working directory with a 260-character
// buffer and returns exactly the text it wrote.
#include <cstdio>
#include <cwchar>
static int g_maxSeen = 0;
static wchar_t* FakeGetCwd(wchar_t* buffer, int maxLength)
{
    g_maxSeen = maxLength;
    const wchar_t text[] = L"C:\\Games\\Fable";
    for (unsigned int i = 0; i < sizeof(text) / sizeof(text[0]); ++i) buffer[i] = text[i];
    return buffer;
}
struct CWideString {
    wchar_t buf[300];
    CWideString(const wchar_t* t) { unsigned int i = 0; for (; t[i] && i < 299; ++i) buf[i] = t[i]; buf[i] = 0; }
};
static CWideString CalcCurrentPath()
{
    wchar_t path[260];
    FakeGetCwd(path, 260);
    return CWideString(path);
}
int main()
{
    int fails = 0;
    CWideString s = CalcCurrentPath();
    if (g_maxSeen != 260) ++fails;
    if (std::wcscmp(s.buf, L"C:\\Games\\Fable") != 0) ++fails;
    std::printf(fails ? "009974a0_TEST FAIL\n" : "009974a0_TEST PASS\n");
    return fails ? 1 : 0;
}