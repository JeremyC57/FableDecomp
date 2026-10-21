// behaviour test for 0x004497C0
#include <stdio.h>
#include <string.h>
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingTypeTag;                          // +0x10 (2 = creature)
    unsigned char _pad11[0x90 - 0x11];
    signed char   PlayerNumber;                          // +0x90
};
class CPlayerManager {
public:
    unsigned char _pad00[0x1c];
    long          MainPlayer;                            // +0x1c
    bool IsThingMainPlayer(const CThing& thing) const;
};
bool CPlayerManager::IsThingMainPlayer(const CThing& thing) const
{
    if (thing.ThingTypeTag == 2 && thing.PlayerNumber == MainPlayer)
        return true;
    return false;
}

int main()
{
    CThing t; memset(&t, 0, sizeof(t)); CPlayerManager pm; memset(&pm, 0, sizeof(pm)); pm.MainPlayer = 1;
    t.ThingTypeTag = 2; t.PlayerNumber = 1; int ok = pm.IsThingMainPlayer(t);
    t.PlayerNumber = 0; ok &= !pm.IsThingMainPlayer(t);
    t.PlayerNumber = 1; t.ThingTypeTag = 1; ok &= !pm.IsThingMainPlayer(t);
    pm.MainPlayer = -1; t.ThingTypeTag = 2; t.PlayerNumber = -1; ok &= pm.IsThingMainPlayer(t);
    printf(ok ? "004497c0_TEST PASS\n" : "004497c0_TEST FAIL\n");
    return ok ? 0 : 1;
}