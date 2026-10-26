// CMainGameComponent::PeekDefinitionManager() const -- the game definition manager (the static singleton accessor,
// tail-called)
// Leaf: ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415FBC, 5 bytes: jmp CGameDefinitionManager::Get (0x0044C6B0); `this` unused.
// Identity (sweep3; was "CEngine::PeekDefinitionManager", before that the propagated GFGetPreMainMemoryUsed): Ego_r.pdb /
//   FableWin.pdb ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ, a 5-byte function in Ego_r
//   too (0x00415162 -> next 0x00415167), the size of this tail jump; no CEngine::PeekDefinitionManager exists in either
//   PDB; the jump target is CGameDefinitionManager::Get (Ego_r ?Get@CGameDefinitionManager@@SIAAV1@XZ, 0x0044C6B0); the
//   landed caller 0x00859040 already binds this exact PDB symbol here.
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CMainGameComponent {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;
};

const CGameDefinitionManager& CMainGameComponent::PeekDefinitionManager() const
{
    return CGameDefinitionManager::Get();
}
