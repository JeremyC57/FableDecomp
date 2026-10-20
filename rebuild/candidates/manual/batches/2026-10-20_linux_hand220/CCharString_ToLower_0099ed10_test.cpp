#include <cstdio>
#include <cstring>
#include <cctype>
static long g_count = 0;
struct Data { char* PData; unsigned long StringLength; long NoRefs; };
struct Str {
    Data* PStringData;
    Str(const char* t) { PStringData = new Data; PStringData->StringLength = (unsigned long)std::strlen(t); PStringData->PData = new char[PStringData->StringLength + 1]; std::strcpy(PStringData->PData, t); PStringData->NoRefs = 1; ++g_count; }
    Str(const Str& o) : PStringData(0) { ++g_count; if (o.PStringData) { PStringData = o.PStringData; ++PStringData->NoRefs; } }
    ~Str() { Unassign(); --g_count; }
    void Unassign() { if (PStringData) { if (--PStringData->NoRefs <= 0) { delete[] PStringData->PData; delete PStringData; } PStringData = 0; } }
    long GetLength() const { return PStringData ? (long)PStringData->StringLength : 0; }
    void MakeOwnCopyOfData()
    {
        Data* d = PStringData;
        if (d != 0 && d->NoRefs > 1) { Str copy(d->PData); Unassign(); PStringData = copy.PStringData; copy.PStringData = 0; }
    }
    Str ToLower() const
    {
        Str result(*this);
        result.MakeOwnCopyOfData();
        for (long i = 0; i < result.GetLength(); ++i)
            result.PStringData->PData[i] = (char)std::tolower(result.PStringData->PData[i]);
        return result;
    }
};
int main()
{
    {
        Str a("HeLLo World");
        Str b = a.ToLower();
        if (std::strcmp(b.PStringData->PData, "hello world") || std::strcmp(a.PStringData->PData, "HeLLo World") || a.PStringData == b.PStringData || a.PStringData->NoRefs != 1) { std::printf("0099ed10_TEST FAIL\n"); return 1; }
    }
    if (g_count != 0) { std::printf("0099ed10_TEST FAIL\n"); return 1; }
    std::printf("0099ed10_TEST PASS\n");
    return 0;
}
