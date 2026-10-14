#include <cstdio>

struct Out { long n; unsigned long v[8]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = x; } void WriteBool(bool b) { v[n++] = b ? 1 : 0; } };
struct D { bool flag; unsigned long* b; unsigned long* e;
    void WriteToFile(Out& o) const { o.WriteBool(flag); unsigned long count = (unsigned long)(e - b); o.WriteULong(count); for (unsigned long i = 0; i < count; ++i) o.WriteULong(b[i]); } };
int main()
{
    unsigned long data[2] = { 5, 6 }; D d = { true, data, data + 2 }; Out o; d.WriteToFile(o);
    if (o.n != 4 || o.v[0] != 1 || o.v[1] != 2 || o.v[2] != 5 || o.v[3] != 6) { std::printf("00b20b10_TEST FAIL\n"); return 1; }
    std::printf("00b20b10_TEST PASS\n");
    return 0;
}