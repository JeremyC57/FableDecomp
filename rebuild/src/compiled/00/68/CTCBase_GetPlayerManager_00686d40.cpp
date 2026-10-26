// CTCBase::GetPlayerManager() -- the player manager held by the global thing manager
// Leaf: ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686D40: mov eax,[0x013B8A1C] ; mov eax,[eax+30h] ; ret (`this` unused; the
//   global 0x013B8A1C is the thing manager).
// Identity (sweep3; was _global::GetGlobal_013b8a1c_deref0x30_00686d40 (source CViewBase_GetDisplayEngine stand-in)):
//   Ego_r.pdb / FableWin.pdb `?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ` (protected).
// Order: Ego_r.pdb and FableWin.pdb list the CTCBase accessor run as PeekWorldMap, GetWorldMap, GetPlayerManager,
//   PeekPlayerManager, PeekComponent, PeekSampleBank, GetThingSearchTools, PeekThingSearchTools, GetPWorldSeed,
//   GetPLocalSeed (Ego_r 0x006B2110..0x006B21A0); retail 0x00686D20..0x00686DB0 is the same ten bodies in that order:
//   +0x28 twice, +0x30 twice, +0x1C, Component->+0x10, jmp CThingManager::GetThingSearchTools 0x0051E490 /
//   PeekThingSearchTools 0x0051E480 (whose own Ego_r/FableWin order is Peek, Get), +0x2C, Component+0x16140. Fields:
//   Ego_r CThingManager Component +0x28, WorldMap +0x34, WorldSeed +0x38, PlayerManager +0x3C (PC 0xC lower);
//   CMainGameComponent PSampleBank +0x10, LocalSeed +0x16140 (90432).
// Layout: engine/CTCBase.h has no member functions, so the class is restated (no members are read here).
class CGameDefinitionManager;
class CWorld;
class CWorldMap;
class CPlayerManager;
class CASoundBank;

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

extern CThingManager* g_pThingManager;          // 0x013B8A1C

class CTCBase {
protected:
    CPlayerManager& GetPlayerManager();
};

CPlayerManager& CTCBase::GetPlayerManager()
{
    return *g_pThingManager->PlayerManager;
}
