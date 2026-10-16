#include <cstdio>
// Behaviour model of the CEnginePrimitiveHandle member destruction in 0x004dc9d2: the primitive is told about its last handle
// only when this handle holds the sole reference; the reference is dropped and the pointer cleared.
struct Prim { long refs; int lastHandleCalls; int destroyed; };
struct Handle { Prim* data;
    void Destroy() {
        long rc = data ? data->refs : 0;
        if (rc == 1) data->lastHandleCalls++;
        if (data) { if (--data->refs == 0) data->destroyed++; data = 0; }
        data = 0;
    } };
int main()
{
    Prim a = { 1, 0, 0 }, b = { 3, 0, 0 };
    Handle h1 = { &a }, h2 = { &b }, h3 = { 0 };
    h1.Destroy(); h2.Destroy(); h3.Destroy();
    if (a.lastHandleCalls != 1 || a.destroyed != 1 || b.lastHandleCalls || b.refs != 2 || h1.data || h2.data) { std::printf("004dc9d2_TEST FAIL\n"); return 1; }
    std::printf("004dc9d2_TEST PASS\n");
    return 0;
}
