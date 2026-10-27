// Behaviour test for 0x0076DD10: pair<C3DVector,C3DVector> copy constructor copies both vectors
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SVec { float X, Y, Z; SVec() {} SVec(const SVec& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct SPair { SVec first, second; SPair() {} SPair(const SPair& o) : first(o.first), second(o.second) {} };
int main()
{
    SPair a; a.first.X = 1; a.first.Y = 2; a.first.Z = 3; a.second.X = 4; a.second.Y = 5; a.second.Z = 6;
    SPair b(a);
    CHECK(b.first.X == 1 && b.first.Y == 2 && b.first.Z == 3 && b.second.X == 4 && b.second.Y == 5 && b.second.Z == 6);
    if (!g_fail) printf("0076dd10_TEST PASS\n");
    return g_fail;
}