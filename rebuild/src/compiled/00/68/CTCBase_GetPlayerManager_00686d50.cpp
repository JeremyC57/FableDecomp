// CTCBase::GetPlayerManager() -- the thing manager's player manager
// Leaf: ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686D50, 9 bytes: mov eax,[g_pThingManager 0x013B8A1C] ; mov eax,[eax+30h] ; ret
// Identity (FINDINGS chunk 389: the row carried two labels, CTCBase::GetPlayerManager and the descriptive
//   GetGlobal_013b8a1c_deref0x30 with a `__fastcall CViewBase_GetDisplayEngine` stand-in body): Ego_r.pdb names
//   ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ (protected, returns CPlayerManager&); xrefs: all 56 call sites
//   are CTCBase-derived component methods (CTCCreatureOpinionOfHero, CTCInventoryMap, CTCShop, CTCHeroCentre,
//   CTCInventory/Base/Abilities, CTCFishing, CTCTeleporter, CTCHeroStats, CTCTavernGame, ...), and landed callers already
//   declare 0x00686D50 as CTCBase::GetPlayerManager; struct_layouts: CThingManager PlayerManager (PDB +0x3C, retail
//   +0x30 with the same -0xC shift as WorldMap PDB +0x34 / retail +0x28, cf. landed 0x00661D20).
// Shape: thiscall member that ignores `this` (no ecx use).
class CPlayerManager;

class CThingManager {
public:
    unsigned char   _pad00[0x30];
    CPlayerManager* PlayerManager;                      // +0x30 (retail)
};

extern CThingManager* g_pThingManager;                 // 0x013B8A1C

class CTCBase {
protected:
    CPlayerManager& GetPlayerManager();
};

CPlayerManager& CTCBase::GetPlayerManager()
{
    return *g_pThingManager->PlayerManager;
}
