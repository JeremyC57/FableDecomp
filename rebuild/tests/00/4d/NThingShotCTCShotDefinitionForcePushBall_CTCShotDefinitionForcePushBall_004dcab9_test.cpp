#include <cstdio>
static void* g_baseArg = 0;
struct CThing { int x; };
struct CTCShotDefinitionProjectile { virtual void v00() { } unsigned char _body[0xa4 - 4]; CTCShotDefinitionProjectile(CThing* t) { g_baseArg = t; } };
namespace NThingShot { struct CTCShotDefinitionForcePushBall : CTCShotDefinitionProjectile { float DistanceToPush; CTCShotDefinitionForcePushBall(CThing* t) : CTCShotDefinitionProjectile(t), DistanceToPush(0.0f) { } }; }
int main()
{
    CThing thing; NThingShot::CTCShotDefinitionForcePushBall s(&thing);
    if (g_baseArg != &thing || s.DistanceToPush != 0.0f || (char*)&s.DistanceToPush - (char*)&s != 0xa4) { std::printf("004dcab9_TEST FAIL\n"); return 1; }
    std::printf("004dcab9_TEST PASS\n"); return 0;
}