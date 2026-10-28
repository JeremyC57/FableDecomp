#include <cstdio>
// Behaviour test for std::swap<pair<unsigned long, C3DHelperPoint> > (0x00A31220).
struct V { float X, Y, Z; };
struct HP { V Position; int BoneIndex; };
struct P { unsigned int first; HP second; };
template <class T> void swap_(T& a, T& b) { T tmp = a; a = b; b = tmp; }
int main()
{
    P a = { 1, { { 1.0f, 2.0f, 3.0f }, 4 } };
    P b = { 9, { { -1.0f, -2.0f, -3.0f }, -4 } };
    swap_(a, b);
    bool ok = a.first == 9 && a.second.Position.Y == -2.0f && a.second.BoneIndex == -4
           && b.first == 1 && b.second.Position.Z == 3.0f && b.second.BoneIndex == 4;
    if (!ok) { std::printf("00a31220_TEST FAIL\n"); return 1; }
    std::printf("00a31220_TEST PASS\n");
    return 0;
}