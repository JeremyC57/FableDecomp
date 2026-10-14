#include <cstdio>

struct Out { long n; unsigned long v[8]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = x; } };
struct Chunk { unsigned long* b; unsigned long* e;
    void WriteToFile(Out& o) const { unsigned long count = (unsigned long)(e - b); o.WriteULong(count); for (unsigned long i = 0; i < count; ++i) o.WriteULong(b[i]);  } };
int main()
{
    unsigned long data[3] = { 7, 8, 9 }; Chunk c = { data, data + 3 }; Out o; c.WriteToFile(o);
    if (o.n != 4 || o.v[0] != 3 || o.v[1] != 7 || o.v[3] != 9) { std::printf("00abc040_TEST FAIL\n"); return 1; }
    std::printf("00abc040_TEST PASS\n");
    return 0;
}
