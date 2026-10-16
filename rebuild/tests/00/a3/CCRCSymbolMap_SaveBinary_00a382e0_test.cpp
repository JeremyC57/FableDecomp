#include <cstdio>
struct Stream { long buf[64]; int n; Stream() : n(0) {} void WriteSLONG(long v) { buf[n++] = v; } };
struct Pair { unsigned long first; long second; };
struct Map { Pair* f; Pair* l; unsigned int size() const { return (unsigned int)(l - f); } };
static void Save(const Map& m, Stream& s)
{
    s.WriteSLONG((long)m.size());
    for (unsigned int i = 0; i < m.size(); ++i) { s.WriteSLONG((long)m.f[i].first); s.WriteSLONG(m.f[i].second); }
}
int main()
{
    Pair p[3] = { {0x11, 1}, {0x22, 2}, {0x33, 3} };
    Map m = { p, p + 3 };
    Stream s; Save(m, s);
    Map e = { p, p }; Stream t; Save(e, t);
    if (s.n != 7 || s.buf[0] != 3 || s.buf[5] != 0x33 || s.buf[6] != 3 || t.n != 1 || t.buf[0] != 0) { std::printf("00a382e0_TEST FAIL\n"); return 1; }
    std::printf("00a382e0_TEST PASS\n");
    return 0;
}