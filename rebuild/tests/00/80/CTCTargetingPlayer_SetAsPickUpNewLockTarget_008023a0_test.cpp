#include <cstdio>

class CTCTargetingPlayer { public:
    char pad[0x3c];
    unsigned char f3c;
    char pad2[3];
    unsigned int f40;
    unsigned int f44;
};

void __fastcall SetAsPickUpNewLockTarget(CTCTargetingPlayer* player);

// CWorld::GetFrame (0x0049D870) is static and takes no object: the stub only feeds the frame.
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
long __fastcall CWorld::GetFrame() { return (long)0xDEADBEEF; }

int main()
{
    CTCTargetingPlayer obj;
    obj.f3c = 0;
    obj.f40 = 123;
    obj.f44 = 0;
    SetAsPickUpNewLockTarget(&obj);
    if (obj.f3c == 1 && obj.f40 == 0 && obj.f44 == 0xDEADBEEF)
        printf("008023a0_TEST PASS\n");
    else
        printf("FAIL f3c=%d f40=%u f44=%x\n", obj.f3c, obj.f40, obj.f44);
    return 0;
}