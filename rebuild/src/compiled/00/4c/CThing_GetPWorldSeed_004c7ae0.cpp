// CThing::GetPWorldSeed() const -- the world random seed pointer held by the global thing manager
// Leaf: ?GetPWorldSeed@CThing@@IBEPAKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7AE0, 9 bytes: mov eax,[0x013B8A1C] ; mov eax,[eax+2Ch] ; ret (this unused).
// Identity: Ego_r.pdb ?GetPWorldSeed@CThing@@IBEPAKXZ. The CThing accessor run reads the global thing manager
//   (0x013B8A1C, landed as g_pThingManager by 0x004C7990 / 0x004C79D0): 0x004C7AA0 / 0x004C7AB0 read +0x24 (World), this
//   row +0x2C and 0x004C7AF0 adds 0x16140 to the component at +0x1C (the local seed), the Ego_r.pdb CThing order
//   GetWorld, PeekWorld, GetPWorldSeed, GetPLocalSeed. Ego_r CThingManager has WorldSeed (ulong*) at +0x38; the PC
//   CThingManager base is 0xC smaller (PeekDefinitionManager +0x20 vs Ego_r +0x2C), so WorldSeed sits at +0x2C.
//   The catalog label "CViewBase::GetDisplayEngine" is propagated from byte-identical bodies.
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
    unsigned long* GetPWorldSeed() const;
};

unsigned long* CThing::GetPWorldSeed() const
{
    return g_pThingManager->WorldSeed;
}