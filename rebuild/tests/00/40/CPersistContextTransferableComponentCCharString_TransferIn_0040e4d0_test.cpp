// behaviour test for 0x0040E4D0 CPersistContext_TransferableComponent<CCharString>::TransferIn (model: the traits temporary forwards to the object)
#include <cstdio>
static int g_ctor = 0, g_dtor = 0; static void* g_target = 0;
struct Traits { Traits() { ++g_ctor; } ~Traits() { ++g_dtor; } void Transfer(void*, void*, int& v) { g_target = &v; } };
struct Component { int* Object; void Transfer(void* c, void* p) { Traits().Transfer(c, p, *Object); } };
int main()
{
    int value = 0; Component comp; comp.Object = &value;
    comp.Transfer(0, 0);
    int ok = g_target == &value && g_ctor == 1 && g_dtor == 1;
    if (!ok) { std::printf("0040e4d0_TEST FAIL\n"); return 1; }
    std::printf("0040e4d0_TEST PASS\n");
    return 0;
}