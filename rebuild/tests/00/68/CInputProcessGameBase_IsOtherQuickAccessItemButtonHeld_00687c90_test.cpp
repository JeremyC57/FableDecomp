#include <cstdio>
// NF153: MyPlayer is the shared engine/CPlayer.h CPlayer (as the row); its GetJoystickDeviceNumber() const 0x00487510 is stubbed below.
// NF171 (2026-11-20, q11921): the player interface is the shared engine/CGamePlayerInterface.h class (as the row now binds it); its
//   IsOtherQuickAccessItemButtonHeld(long, long) const 0x00446060 is stubbed below (was the Helper24 stand-in).
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_ISOTHERQUICKACCESSITEMBUTTONHELD
#include "engine/CGamePlayerInterface.h"

struct CInputProcessGameBase
{
    char pad0[0x18];
    CPlayer* MyPlayer;
    char pad1[0x24 - 0x1C];
    CGamePlayerInterface* PGamePlayerInterface;

protected:
    bool IsOtherQuickAccessItemButtonHeld(long param) const;   // the row's PDB IBE spelling
    friend int main();
};

static long g_device = 7;
static long g_expectIndex = 42;
static const void* g_this = 0;
long CPlayer::GetJoystickDeviceNumber() const { return g_device; }   // stub of 0x00487510
bool CGamePlayerInterface::IsOtherQuickAccessItemButtonHeld(long index, long player) const   // stub of 0x00446060
{
    g_this = this;
    return player == g_device && index == g_expectIndex;
}

int main()
{
    static double playerBuf[0x100];
    static double interfaceBuf[0x400];
    CPlayer* player = (CPlayer*)playerBuf;
    CGamePlayerInterface* gpi = (CGamePlayerInterface*)interfaceBuf;

    CInputProcessGameBase obj;
    obj.MyPlayer = player;
    obj.PGamePlayerInterface = gpi;

    bool r1 = obj.IsOtherQuickAccessItemButtonHeld(42);
    bool r2 = obj.IsOtherQuickAccessItemButtonHeld(99);

    if (r1 && !r2 && g_this == gpi) {
        printf("QUICKACCESS_OK\n");
        return 0;
    }
    printf("QUICKACCESS_FAIL r1=%d r2=%d\n", r1, r2);
    return 1;
}
