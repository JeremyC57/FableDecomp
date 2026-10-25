#include <cstdio>
// Behaviour test for 00f342b0: Resize(count) resizes the wrapped vector to count elements, filling new slots from a
// default-constructed def temporary that is destroyed afterwards.
static int g_ctors = 0, g_dtors = 0;
struct Def { int pattern; Def() : pattern(-1) { ++g_ctors; } Def(const Def& o) : pattern(o.pattern) { ++g_ctors; } ~Def() { ++g_dtors; } };
struct Vec {
    Def items[8]; unsigned int count; Vec() : count(0) {}
    void resize(unsigned int n, const Def& x) { while (count < n && count < 8) items[count++] = x; if (n < count) count = n; }
    void resize(unsigned int n) { resize(n, Def()); }
};
struct Wrapper { Vec* TransVector; void Resize(long count) { TransVector->resize(count); } };
int main()
{
    Vec* v = new Vec;
    int baseCtors = g_ctors, baseDtors = g_dtors;
    Wrapper w; w.TransVector = v;
    w.Resize(3);
    int ok = v->count == 3 && v->items[2].pattern == -1 && g_ctors - baseCtors == 1 && g_dtors - baseDtors == 1;
    w.Resize(1);
    ok = ok && v->count == 1;
    delete v;
    if (!ok) { std::printf("00f342b0_TEST FAIL\n"); return 1; }
    std::printf("00f342b0_TEST PASS\n");
    return 0;
}