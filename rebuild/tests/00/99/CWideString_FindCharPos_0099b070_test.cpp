#include <cstdio>
#include <cwchar>
struct CWideStringData { wchar_t* Start; wchar_t* Finish; wchar_t* EndOfStorage; long NoRefs; };
static wchar_t g_emptyWideString[1] = { 0 };
class CWideString {
public:
    CWideStringData* PStringData;
    const wchar_t* GetText() const { return PStringData == 0 ? g_emptyWideString : PStringData->Start; }
    long FindCharPos(long c) const
    {
        const wchar_t* found = std::wcschr(GetText(), (wchar_t)c);
        if (found != 0) return found - GetText();
        return -1;
    }
};
int main()
{
    wchar_t text[] = L"hello";
    CWideStringData d; d.Start = text; d.Finish = text + 5; d.EndOfStorage = text + 6; d.NoRefs = 1;
    CWideString s; s.PStringData = &d;
    if (s.FindCharPos('l') != 2 || s.FindCharPos('h') != 0 || s.FindCharPos('o') != 4 || s.FindCharPos('z') != -1) { std::printf("0099b070_TEST FAIL\n"); return 1; }
    CWideString e; e.PStringData = 0;
    if (e.FindCharPos('a') != -1) { std::printf("0099b070_TEST FAIL\n"); return 1; }
    std::printf("0099b070_TEST PASS\n"); return 0;
}