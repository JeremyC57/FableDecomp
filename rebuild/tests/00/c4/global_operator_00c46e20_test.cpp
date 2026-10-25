// Behaviour test for 00c46e20: operator<(C2DVector, C2DVector) -- lexicographic by X then Y
#include <cstdio>

struct C2DVector { float X, Y; };
static bool Less(const C2DVector& a, const C2DVector& b)
{
    if (a.X == b.X)
        return a.Y < b.Y;
    return a.X < b.X;
}

int main()
{
    C2DVector a = { 1.0f, 5.0f }, b = { 2.0f, 0.0f }, c = { 1.0f, 6.0f };
    if (!Less(a, b) || Less(b, a) || !Less(a, c) || Less(c, a) || Less(a, a)) { std::printf("00c46e20_TEST FAIL\n"); return 1; }
    std::printf("00c46e20_TEST PASS\n");
    return 0;
}