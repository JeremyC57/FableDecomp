#include <cstdio>
struct CThingPhysicsMeshInfo { int dummy; };
struct CThing { unsigned long Flags; };
static CThingPhysicsMeshInfo g_info;
struct CTCPhysicsBase {
    CThing* sub;
    int calls;
    int answer[2];                          // results of the first and second slot-93 calls
    virtual ~CTCPhysicsBase() { }
    virtual CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return answer[calls++ & 1] ? &g_info : 0; }
    bool IsSphereShaped() { return GetPhysicsMeshInfo() != 0 || (sub->Flags & 8) == 0; }
    long IsPhysicsMeshTypeSphere() { return !GetPhysicsMeshInfo() && IsSphereShaped(); }
};
static long Run(int first, int second, unsigned long flags, int* calls)
{
    CThing t; t.Flags = flags; CTCPhysicsBase b; b.sub = &t; b.calls = 0; b.answer[0] = first; b.answer[1] = second;
    long r = b.IsPhysicsMeshTypeSphere(); *calls = b.calls; return r;
}
int main()
{
    int calls;
    if (Run(1, 1, 0, &calls) != 0 || calls != 1) { std::printf("006b0190_TEST FAIL\n"); return 1; }
    if (Run(0, 0, 0, &calls) != 1 || calls != 2) { std::printf("006b0190_TEST FAIL\n"); return 1; }
    if (Run(0, 0, 8, &calls) != 0 || calls != 2) { std::printf("006b0190_TEST FAIL\n"); return 1; }
    if (Run(0, 1, 8, &calls) != 1 || calls != 2) { std::printf("006b0190_TEST FAIL\n"); return 1; }
    std::printf("006b0190_TEST PASS\n");
    return 0;
}
