#include <cstdio>

struct Out { unsigned long count; const void* data; unsigned long size; void WriteULong(unsigned long x) { count = x; } void WriteData(const void* d, unsigned long s) { data = d; size = s; } };
struct Arr { unsigned short* b; unsigned short* e; unsigned short* c; };
struct Strip { unsigned char pad[16]; Arr v;
    unsigned long Size() const { return (unsigned long)(v.e - v.b); }
    const void* GetList() const { if (Size() > 0) return v.b; return &v; }
    void WriteToFile(Out& o) const { o.WriteULong(Size()); o.WriteData(GetList(), Size() * 2); } };
int main()
{
    unsigned short data[3] = { 1, 2, 3 }; Strip s; s.v.b = data; s.v.e = data + 3; s.v.c = data + 3; Out o; s.WriteToFile(o);
    if (o.count != 3 || o.data != data || o.size != 6) { std::printf("00aa5930_TEST FAIL\n"); return 1; }
    s.v.e = data; s.WriteToFile(o); if (o.count != 0 || o.data != &s.v || o.size != 0) { std::printf("00aa5930_TEST FAIL\n"); return 1; }
    std::printf("00aa5930_TEST PASS\n");
    return 0;
}
