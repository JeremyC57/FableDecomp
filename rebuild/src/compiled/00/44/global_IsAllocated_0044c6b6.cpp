// CGameDefinitionManager::IsAllocated() -- true when the game definition manager singleton exists
// Leaf: ?IsAllocated@CGameDefinitionManager@@SI_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C6B6, 12 bytes: xor eax,eax ; cmp [0x013B879C],eax ; setne al ; ret (size TU).
// Identity: Ego_r.pdb / FableWin.pdb ?IsAllocated@CGameDefinitionManager@@SI_NXZ (static __fastcall), directly after
//   ?Get@CGameDefinitionManager@@SIAAV1@XZ (0x0044C6B0, which returns the same global) as in Ego_r. sweep3: the static
//   member replaces the free function CGameDefinitionManager_IsAllocated, so the object symbol is the PDB one.
#pragma optimize("s",on)
class CGameDefinitionManager;

extern CGameDefinitionManager* g_pGameDefinitionManager;   // 0x013B879C

class CGameDefinitionManager {
public:
    static bool __fastcall IsAllocated();
};

bool __fastcall CGameDefinitionManager::IsAllocated()
{
    return g_pGameDefinitionManager != 0;
}
