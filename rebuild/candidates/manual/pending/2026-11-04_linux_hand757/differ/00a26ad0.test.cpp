// Behaviour test for 0x00A26AD0 C3DAnimationInfo::BuildAnimationInstance: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct A { int failUntil; int attempts; };
static int Build(A& a, int& freed, int& defragged)
{
    int attempt = 0;
    while (a.attempts++ < a.failUntil) {
        switch (attempt) { case 0: freed++; break; case 1: defragged++; break; default: goto done; }
        ++attempt;
    }
done:
    return attempt;
}
int main() {
    int f = 0, d = 0; A a = { 0, 0 }; int ok = Build(a, f, d) == 0 && f == 0;
    A b = { 2, 0 }; ok &= Build(b, f, d) == 2 && f == 1 && d == 1;
    A c = { 10, 0 }; f = d = 0; ok &= Build(c, f, d) == 2 && f == 1 && d == 1;
    std::printf(ok ? "00a26ad0_TEST PASS\n" : "00a26ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
