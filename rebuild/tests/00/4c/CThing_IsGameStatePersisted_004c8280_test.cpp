// behaviour test for 0x004C8280
#include <stdio.h>
#include <string.h>
enum { TC_COOP_SPIRIT = 0xc7 };                          // CTCCoopSpirit interface type (vtable slot 21 returns 0xc7)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    unsigned char _pad44[0x92 - 0x44];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;     // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;                // +0x92 bit 5
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    bool IsGameStatePersisted() const;
};
bool CThing::IsGameStatePersisted() const
{
    if (HasTC(TC_COOP_SPIRIT))
        return false;
    return GameStatePersisted;
}
int main()
{
    CThing t; memset(&t, 0, sizeof(t)); int ok = 1;
    ok &= !t.IsGameStatePersisted();
    t.GameStatePersisted = 1; ok &= t.IsGameStatePersisted();
    t.UsingComponents[0xc7 >> 5] |= 1UL << (0xc7 & 31); ok &= !t.IsGameStatePersisted();
    ok &= ((unsigned char*)&t)[0x38] == 0x80 && (((unsigned char*)&t)[0x92] & 0x20);
    printf(ok ? "004c8280_TEST PASS\n" : "004c8280_TEST FAIL\n");
    return ok ? 0 : 1;
}