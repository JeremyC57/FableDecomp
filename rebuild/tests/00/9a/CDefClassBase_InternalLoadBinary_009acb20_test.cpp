// Behaviour test for 0x009ACB20 CDefClassBase::InternalLoadBinary (model copy).
#include <cstdio>
#include <cstring>

struct CStream {
    unsigned long StreamPos; const unsigned char* Cur; long Remaining; int overflowCalls;
    void Overflow(unsigned char* dst, long size) { ++overflowCalls; std::memset(dst, 1, size); StreamPos += size; }
    void Read(void* dst, long size)
    {
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (Remaining >= size) { std::memcpy(dst, Cur, size); Cur += size; Remaining -= size; StreamPos += size; }
        else Overflow((unsigned char*)dst, size);
    }
};
struct CDef {
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
    void Load(CStream& in)
    {
        { bool v; in.Read(&v, 1); Setup = v; }
        { bool v; in.Read(&v, 1); Template = v; }
        { bool v; in.Read(&v, 1); DefaultValsApplied = v; }
    }
};

int main()
{
    unsigned char data[3] = { 1, 0, 1 };
    CStream s; s.StreamPos = 0; s.Cur = data; s.Remaining = 3; s.overflowCalls = 0;
    CDef d; d.Setup = 0; d.Template = 1; d.DefaultValsApplied = 0;
    d.Load(s);
    if (d.Setup != 1 || d.Template != 0 || d.DefaultValsApplied != 1 || s.StreamPos != 3 || s.overflowCalls != 0)
    { std::printf("009acb20_TEST FAIL\n"); return 1; }
    unsigned char two[1] = { 0 };
    CStream t; t.StreamPos = 10; t.Cur = two; t.Remaining = 1; t.overflowCalls = 0;
    d.Load(t);
    if (d.Setup != 0 || d.Template != 1 || d.DefaultValsApplied != 1 || t.overflowCalls != 2)
    { std::printf("009acb20_TEST FAIL\n"); return 1; }
    std::printf("009acb20_TEST PASS\n");
    return 0;
}