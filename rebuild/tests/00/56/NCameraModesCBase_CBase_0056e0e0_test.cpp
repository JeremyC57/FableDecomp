// Behaviour test for NCameraModes::CBase::~CBase (0x0056E0E0): lists and vector destroyed in reverse, the def released (destroyed
// at count zero) and cleared.
#include <cstdio>
static int g_order[8]; static int g_n; static int g_destroyed;
struct List { int id; ~List() { g_order[g_n++] = id; } };
struct Def { unsigned int RefCount; void Destroy() { ++g_destroyed; } void ReleaseTemporary() { if (--RefCount == 0) Destroy(); } };
struct DefPtr { Def* D; ~DefPtr() { if (D) { D->ReleaseTemporary(); D = 0; } g_order[g_n++] = 99; } };
struct SubVec { ~SubVec() { g_order[g_n++] = 50; } };
struct State { List Attractors, Incidental; };
struct CameraBase {
    State LookAt; DefPtr PCameraModeDef; SubVec SubComponents; State GoTo;
};
int main()
{
    Def d; d.RefCount = 1;
    {
        CameraBase* b = new CameraBase;
        b->LookAt.Attractors.id = 1; b->LookAt.Incidental.id = 2; b->GoTo.Attractors.id = 3; b->GoTo.Incidental.id = 4;
        b->PCameraModeDef.D = &d;
        delete b;
    }
    const int want[6] = { 4, 3, 50, 99, 2, 1 };
    for (int i = 0; i < 6; ++i) if (g_order[i] != want[i]) { std::printf("0056e0e0_TEST FAIL\n"); return 1; }
    if (g_destroyed != 1 || d.RefCount != 0) { std::printf("0056e0e0_TEST FAIL\n"); return 1; }
    std::printf("0056e0e0_TEST PASS\n");
    return 0;
}