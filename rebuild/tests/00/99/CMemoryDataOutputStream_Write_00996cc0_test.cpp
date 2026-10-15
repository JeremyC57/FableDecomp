#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
#include <cstring>
static char g_store[64];
struct Arr { char* start; char* finish; char* eos; unsigned long size() const { return finish - start; }
    char* GetData() { return size() ? start : (char*)this; }
    void resize(unsigned long n) { start = g_store; finish = g_store + n; eos = g_store + 64; } };
struct Out { Arr Data; unsigned long pos;
    void Write(const void* data, long size)
    {
        if (size > 0) {
            i64 end = (i64)pos + size;
            if (end >= 0 && end <= 0x7fffffff) {
                unsigned long ne = pos + size;
                if (ne > Data.size()) Data.resize(ne);
                std::memcpy(Data.GetData() + pos, data, size);
                pos += size;
            }
        }
    } };
int main()
{
    Out o; o.Data.start = o.Data.finish = o.Data.eos = 0; o.pos = 0;
    o.Write("hello", 5); o.Write(" world", 6); o.Write("x", 0); o.Write("y", -3);
    if (o.pos != 11 || o.Data.size() != 11 || std::memcmp(g_store, "hello world", 11) != 0) { std::printf("00996cc0_TEST FAIL\n"); return 1; }
    o.pos = 0x7ffffff0; o.Write("zz", 0x20);
    if (o.pos != 0x7ffffff0) { std::printf("00996cc0_TEST FAIL\n"); return 1; }
    std::printf("00996cc0_TEST PASS\n");
    return 0;
}