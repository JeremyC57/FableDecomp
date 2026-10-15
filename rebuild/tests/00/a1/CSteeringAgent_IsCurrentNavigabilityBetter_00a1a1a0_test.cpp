#include <cstdio>
// Behaviour test for 00a1a1a0: 1 when either current best clearance exceeds the next-waypoint clearance, else 0.
struct CSteeringAgent {
    float BestDistLeft;
    float BestDistRight;
    float BestNextDistLeft;
    float BestNextDistRight;
    long IsCurrentNavigabilityBetter() const;
};
long CSteeringAgent::IsCurrentNavigabilityBetter() const
{
    return BestDistLeft > BestNextDistLeft || BestDistRight > BestNextDistRight;
}
static long Run(float l, float r, float nl, float nr)
{
    CSteeringAgent a; a.BestDistLeft = l; a.BestDistRight = r; a.BestNextDistLeft = nl; a.BestNextDistRight = nr;
    return a.IsCurrentNavigabilityBetter();
}
int main()
{
    if (Run(2.0f, 0.0f, 1.0f, 5.0f) != 1 || Run(0.0f, 3.0f, 1.0f, 2.0f) != 1 || Run(1.0f, 2.0f, 1.0f, 2.0f) != 0
        || Run(0.5f, 0.5f, 1.0f, 1.0f) != 0 || Run(3.0f, 3.0f, 1.0f, 1.0f) != 1) {
        std::printf("00a1a1a0_TEST FAIL\n"); return 1;
    }
    std::printf("00a1a1a0_TEST PASS\n");
    return 0;
}