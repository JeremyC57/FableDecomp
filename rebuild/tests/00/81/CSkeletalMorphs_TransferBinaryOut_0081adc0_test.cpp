// Behaviour test for 0x0081ADC0: serialisation order count, then key/morph/index/flag per entry.
#include <cstdio>
struct Out { unsigned int log[32]; int n; void W(unsigned int v) { log[n++] = v; } };
struct Entry { int Morph; unsigned short Index; bool Enabled; };
struct Pair { unsigned int first; Entry second; };
static void Save(const Pair* b, const Pair* e, Out& o)
{
    unsigned int count = (unsigned int)(e - b); o.W(count);
    for (unsigned int i = 0; i < count; ++i) { o.W(b[i].first); const Entry& en = b[i].second; o.W((unsigned int)en.Morph); o.W(en.Index); o.W(en.Enabled); }
}
int main()
{
    Pair p[2] = {{7, {-1, 3, true}}, {9, {5, 65535, false}}}; Out o; o.n = 0; Save(p, p + 2, o);
    bool ok = o.n == 9 && o.log[0] == 2 && o.log[1] == 7 && o.log[2] == 0xffffffffu && o.log[3] == 3 && o.log[4] == 1 && o.log[5] == 9 && o.log[7] == 65535 && o.log[8] == 0;
    Out z; z.n = 0; Save(p, p, z); ok = ok && z.n == 1 && z.log[0] == 0;
    if (!ok) { std::printf("0081adc0_TEST FAIL\n"); return 1; }
    std::printf("0081adc0_TEST PASS\n");
    return 0;
}