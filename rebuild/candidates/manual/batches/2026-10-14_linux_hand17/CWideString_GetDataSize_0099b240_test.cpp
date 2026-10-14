#include <cstdio>
struct CWideStringData { wchar_t* Start; wchar_t* Finish; wchar_t* EndOfStorage; long NoRefs; };
class CWideString {
public:
    CWideStringData* PStringData;
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->Finish - PStringData->Start; }
    long GetDataSize() const { return GetLength() * sizeof(wchar_t); }
};
int main()
{
    wchar_t text[] = L"abc";
    CWideStringData d; d.Start = text; d.Finish = text + 3; d.EndOfStorage = text + 4; d.NoRefs = 1;
    CWideString s; s.PStringData = &d;
    if (s.GetDataSize() != 6) { std::printf("0099b240_TEST FAIL\n"); return 1; }
    CWideString e; e.PStringData = 0;
    if (e.GetDataSize() != 0) { std::printf("0099b240_TEST FAIL\n"); return 1; }
    std::printf("0099b240_TEST PASS\n"); return 0;
}
