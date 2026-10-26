// Test for 0x004C79E0 CThing::PeekWorldMap() const: returns the thing manager's WorldMap (the thing itself is not read).
#include <stdio.h>
class CGameDefinitionManager { public: int id; };
class CWorldMap { public: int id; };
class CPlayerManager { public: int id; };
class CThingManager {
public:
    unsigned char           _pad00[0x20];
    CGameDefinitionManager* DefinitionManager;  // +0x20
    void*                   World;              // +0x24
    CWorldMap*              WorldMap;           // +0x28
    unsigned long*          WorldSeed;          // +0x2C
    CPlayerManager*         PlayerManager;      // +0x30
};
CThingManager* g_pThingManager;
class CThing {
    friend int main();
protected:
    const CWorldMap& PeekWorldMap() const;   // protected, as in retail (the mangled name carries it)
public:
    int Dummy;
};
int main()
{
    CGameDefinitionManager dm; CWorldMap wm; CPlayerManager pm;
    CThingManager tm; tm.DefinitionManager = &dm; tm.World = 0; tm.WorldMap = &wm; tm.WorldSeed = 0; tm.PlayerManager = &pm;
    g_pThingManager = &tm;
    if ((char*)&tm.WorldMap - (char*)&tm != 0x28) { printf("004c79e0_TEST FAIL offset\n"); return 1; }
    CThing t; t.Dummy = 0;
    const CThing& rt = t;
    const CWorldMap& r = rt.PeekWorldMap();
    if (&r != tm.WorldMap) { printf("004c79e0_TEST FAIL\n"); return 1; }
    CPlayerManager pm2; CWorldMap wm2; CGameDefinitionManager dm2;
    tm.PlayerManager = &pm2; tm.WorldMap = &wm2; tm.DefinitionManager = &dm2;
    if (&rt.PeekWorldMap() != tm.WorldMap) { printf("004c79e0_TEST FAIL reread\n"); return 1; }
    printf("004c79e0_TEST PASS\n");
    return 0;
}
