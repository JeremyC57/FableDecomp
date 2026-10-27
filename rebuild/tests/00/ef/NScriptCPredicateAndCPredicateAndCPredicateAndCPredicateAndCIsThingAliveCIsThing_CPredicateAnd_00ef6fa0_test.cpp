#include <cstdio>
// Behaviour model: the conjunction is true only when every operand is.
struct P { virtual bool operator()() const = 0; virtual ~P() { } };
struct Alive : P { bool v; Alive(bool b) : v(b) { } bool operator()() const { return v; } };
struct And : P { const P& l; const P& r; And(const P& a, const P& b) : l(a), r(b) { } bool operator()() const { return l() && r(); } };
int main()
{
    Alive t(true), f(false); And a(t, t), b(a, f), c(a, t);
    if (!a() || b() || !c()) { std::printf("00ef6fa0_TEST FAIL\n"); return 1; }
    std::printf("00ef6fa0_TEST PASS\n");
    return 0;
}