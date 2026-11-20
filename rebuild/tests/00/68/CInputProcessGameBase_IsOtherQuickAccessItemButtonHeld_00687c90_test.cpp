#include <cstdio>
// NF153: MyPlayer is the shared engine/CPlayer.h CPlayer (as the row); its GetJoystickDeviceNumber() const 0x00487510 is stubbed below.
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"

struct Helper24
{
    int expectA;
    long expectB;
    bool Check(long b, int a);
};

struct CInputProcessGameBase
{
    char pad0[0x18];
    CPlayer* MyPlayer;
    char pad1[0x24 - 0x1C];
    Helper24* PGamePlayerInterface;

    bool IsOtherQuickAccessItemButtonHeld(long param);
};

static long g_device = 7;
long CPlayer::GetJoystickDeviceNumber() const { return g_device; }   // stub of 0x00487510
bool Helper24::Check(long b, int a) { return a == expectA && b == expectB; }

int main()
{
    static double playerBuf[0x100];
    CPlayer* player = (CPlayer*)playerBuf;
    Helper24 h24; h24.expectA = 7; h24.expectB = 42;

    CInputProcessGameBase obj;
    obj.MyPlayer = player;
    obj.PGamePlayerInterface = &h24;

    bool r1 = obj.IsOtherQuickAccessItemButtonHeld(42);
    bool r2 = obj.IsOtherQuickAccessItemButtonHeld(99);

    if (r1 && !r2) {
        printf("QUICKACCESS_OK\n");
        return 0;
    }
    printf("QUICKACCESS_FAIL r1=%d r2=%d\n", r1, r2);
    return 1;
}