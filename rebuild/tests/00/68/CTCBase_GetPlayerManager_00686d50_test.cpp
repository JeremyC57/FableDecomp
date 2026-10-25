// Test for 0x00686D50 CTCBase::GetPlayerManager: the source object must return the player manager stored at +0x30 of
// the global thing manager, whatever `this` is.
#include <stdio.h>
#include <string.h>
class CPlayerManager { public: int tag; };
class CThingManager { public: unsigned char _pad00[0x30]; CPlayerManager* PlayerManager; };
CThingManager* g_pThingManager;
class CTCBase {
protected:
    CPlayerManager& GetPlayerManager();
};
class CTCProbe : public CTCBase {
public:
    CPlayerManager* Get() { return &GetPlayerManager(); }
};
int main()
{
    CThingManager tm; memset(&tm, 0, sizeof(tm));
    CPlayerManager pm; pm.tag = 7;
    tm.PlayerManager = &pm;
    g_pThingManager = &tm;
    CTCProbe probe;
    bool ok = probe.Get() == &pm;
    CPlayerManager pm2; tm.PlayerManager = &pm2;
    ok = ok && probe.Get() == &pm2;
    if (!ok) { puts("00686d50_TEST FAIL"); return 1; }
    puts("00686d50_TEST PASS");
    return 0;
}
