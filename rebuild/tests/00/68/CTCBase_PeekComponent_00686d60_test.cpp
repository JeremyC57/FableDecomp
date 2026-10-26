// Test for 0x00686D60 CTCBase::PeekComponent() const: the main game component held by the global thing manager; `this` is not read.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class CGameDefinitionManager;
class CWorld;
class CWorldMap;
class CPlayerManager;
class CASoundBank;
class CASoundBank { public: int id; };
class CWorldMap { public: int id; };
class CPlayerManager { public: int id; };
class CMainGameComponent {                     // only the fields read here
public:
    unsigned char _pad00[0x10];
    CASoundBank*  PSampleBank;                  // +0x10
    unsigned char _pad14[0x16140 - 0x14];
    unsigned long LocalSeed;                    // +0x16140
    unsigned long* GetLocalSeed() const { return const_cast<unsigned long*>(&LocalSeed); }
};

class CThingManager {                          // only the fields read here (PC offsets)
public:
    unsigned char           _pad00[0x1C];
    CMainGameComponent*     Component;          // +0x1C
    CGameDefinitionManager* DefinitionManager;  // +0x20
    CWorld*                 World;              // +0x24
    CWorldMap*              WorldMap;           // +0x28
    unsigned long*          WorldSeed;          // +0x2C
    CPlayerManager*         PlayerManager;      // +0x30
};

CThingManager* g_pThingManager;
class CTCBase {
protected:
    const CMainGameComponent& PeekComponent() const;
};
const CMainGameComponent& CTCBase::PeekComponent() const
{
    return *g_pThingManager->Component;
}
class CTCProbe : public CTCBase {
public:
    const CMainGameComponent& Call() const { return PeekComponent(); }
};
int main()
{
    CMainGameComponent& mgc = *(CMainGameComponent*)calloc(1, sizeof(CMainGameComponent));
    CASoundBank bank; CWorldMap wm; CPlayerManager pm; unsigned long seed = 7;
    mgc.PSampleBank = &bank; mgc.LocalSeed = 9;
    CThingManager tm; memset(&tm, 0, sizeof(tm));
    tm.Component = &mgc; tm.WorldMap = &wm; tm.WorldSeed = &seed; tm.PlayerManager = &pm;
    g_pThingManager = &tm;
    if ((char*)&tm.Component - (char*)&tm != 0x1C || (char*)&tm.PlayerManager - (char*)&tm != 0x30 ||
        (char*)&mgc.PSampleBank - (char*)&mgc != 0x10 || (char*)&mgc.LocalSeed - (char*)&mgc != 0x16140) { printf("00686d60_TEST FAIL layout\n"); return 1; }
    CTCProbe probe;
    const CMainGameComponent& r = probe.Call();
    if (!(&r == &mgc)) { printf("00686d60_TEST FAIL\n"); return 1; }
    printf("00686d60_TEST PASS\n");
    return 0;
}
