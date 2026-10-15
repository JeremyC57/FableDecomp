// Behaviour test for CTCOnHeroPush::IsAngry @ 0x007BBA50: strictly beyond the anger distance.
#include <cstdio>
static float g_angryPushDistance = 2.5f;
struct CTCOnHeroPush {
    float DistPushed;
    long IsAngry() const { if (DistPushed > g_angryPushDistance) return 1; return 0; }
};
int main()
{
    CTCOnHeroPush p;
    p.DistPushed = 2.5f; if (p.IsAngry() != 0) { std::printf("007bba50_TEST FAIL\n"); return 1; }
    p.DistPushed = 3.0f; if (p.IsAngry() != 1) { std::printf("007bba50_TEST FAIL\n"); return 1; }
    p.DistPushed = 1.0f; if (p.IsAngry() != 0) { std::printf("007bba50_TEST FAIL\n"); return 1; }
    std::printf("007bba50_TEST PASS\n");
    return 0;
}