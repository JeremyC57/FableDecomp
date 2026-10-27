// behaviour test for 0x004D9D2F: pair(a, b) copy-constructs first then second
#include <cstdio>
static int g_order[2]; static int g_n = 0;
struct A { int v; A(int x) : v(x) { } A(const A& o) : v(o.v) { g_order[g_n++] = 1; } };
struct B { int v; B(int x) : v(x) { } B(const B& o) : v(o.v) { g_order[g_n++] = 2; } };
struct P { A first; B second; P(const A& a, const B& b) : first(a), second(b) { } };
int main()
{
    A a(5); B b(9); P p(a, b);
    if (p.first.v != 5 || p.second.v != 9 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("004d9d2f_TEST FAIL\n"); return 1; }
    std::printf("004d9d2f_TEST PASS\n");
    return 0;
}