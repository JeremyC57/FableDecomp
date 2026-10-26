// CGameDefinitionManager::Get() -- the game definition manager singleton (static accessor)
// Leaf: ?Get@CGameDefinitionManager@@SIAAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C6B0, 6 bytes: mov eax,[0x013B879C] ; ret.
// Identity (sweep3; was "CDefinitionManager::Get", before that GFGetPreMainMemoryUsed; manifest CGameDefinitionManager::Get):
//   Ego_r.pdb / FableWin.pdb ?Get@CGameDefinitionManager@@SIAAV1@XZ (static, returns CGameDefinitionManager&; no
//   CDefinitionManager::Get exists), 6 bytes in Ego_r between ?ErrorCallback@CGameDefinitionManager@@KI... (1 byte) and
//   ?IsAllocated@CGameDefinitionManager@@SI_NXZ, exactly as retail 0x0044C6AF / 0x0044C6B0 / 0x0044C6B6 (IsAllocated tests
//   the same global 0x013B879C). CMainGameComponent::PeekDefinitionManager 0x00415FBC tail-jumps here.
class CGameDefinitionManager;

extern CGameDefinitionManager* g_pGameDefinitionManager;   // 0x013B879C (set by CGameDefinitionManager::Set)

class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
};

CGameDefinitionManager& __fastcall CGameDefinitionManager::Get()
{
    return *g_pGameDefinitionManager;
}
