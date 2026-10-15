#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CThing { unsigned char pad[0x10]; unsigned char type; };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CModeStandBase : CModeBase { unsigned char pad[0x14]; long idle; long frames; bool force; float MaxRunSpeed; float MaxJogSpeed; float MaxWalkSpeed; };
struct CMode : CModeStandBase { virtual float GetMovementSpeed(long, long, long mt) { float speed = 0.0f; if (thing->type != 2) { switch (mt) { case 3: speed = MaxWalkSpeed; break; case 5: speed = MaxRunSpeed; break; } } return speed; } };
int main()
{
    CThing t; t.type = 1; CMode m; m.thing = &t; m.MaxRunSpeed = 7.0f; m.MaxJogSpeed = 5.0f; m.MaxWalkSpeed = 2.0f;
    if (m.GetMovementSpeed(0, 0, 3) != 2.0f || m.GetMovementSpeed(0, 0, 5) != 7.0f || m.GetMovementSpeed(0, 0, 4) != 0.0f || m.GetMovementSpeed(0, 0, 0) != 0.0f) { std::printf("00861900_TEST FAIL\n"); return 1; }
    t.type = 2; if (m.GetMovementSpeed(0, 0, 3) != 0.0f || m.GetMovementSpeed(0, 0, 5) != 0.0f) { std::printf("00861900_TEST FAIL\n"); return 1; }
    std::printf("00861900_TEST PASS\n");
    return 0;
}