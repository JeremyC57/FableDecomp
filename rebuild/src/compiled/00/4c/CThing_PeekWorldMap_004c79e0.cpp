// CThing::PeekWorldMap() const -- the world map held by the global thing manager
// Leaf: ?PeekWorldMap@CThing@@IBEABVCWorldMap@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C79E0, 9 bytes: mov eax,[0x013B8A1C] ; mov eax,[eax+28h] ; ret (this unused).
// Identity (sweep3; was the descriptive _global::GetGlobal_013b8a1c_deref0x28_004c79e0 (before that CViewBase::GetDisplayEngine)): Ego_r.pdb / FableWin.pdb
//   `?PeekWorldMap@CThing@@IBEABVCWorldMap@@XZ`.
//   Retail order of the CThing accessor run (0x004C7990 +0x20, 0x004C79A0, 0x004C79B0 / 0x004C79C0 forwarding to
//   CThingManager::GetMapAt / PeekMapAt, 0x004C79D0 / 0x004C79E0 +0x28, 0x004C79F0 / 0x004C7A00 +0x30) follows the
//   Ego_r.pdb and FableWin.pdb CThing order PeekDefinitionManager, GetMapAt, PeekMapAt, GetWorldMap, PeekWorldMap,
//   PeekPlayerManager, GetPlayerManager, and the fields are Ego_r CThingManager DefinitionManager +0x2C, WorldMap
//   +0x34, PlayerManager +0x3C (the PC base is 0xC smaller; the global at 0x013B8A1C is the thing manager, as the
//   0x004C79B0 / 0x004C79C0 forwarders pass it as `this` to the CThingManager map lookups).
class CGameDefinitionManager;
class CWorldMap;
class CPlayerManager;

class CThingManager {                          // only the fields read here (PC offsets)
public:
    unsigned char           _pad00[0x20];
    CGameDefinitionManager* DefinitionManager;  // +0x20
    void*                   World;              // +0x24
    CWorldMap*              WorldMap;           // +0x28
    unsigned long*          WorldSeed;          // +0x2C
    CPlayerManager*         PlayerManager;      // +0x30
};

extern CThingManager* g_pThingManager;          // 0x013B8A1C

class CThing {
protected:
    const CWorldMap& PeekWorldMap() const;
};

const CWorldMap& CThing::PeekWorldMap() const
{
    return *g_pThingManager->WorldMap;
}
