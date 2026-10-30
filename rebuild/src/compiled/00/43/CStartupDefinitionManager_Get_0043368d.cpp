// CStartupDefinitionManager::Get() -- the startup definition manager singleton: `mov eax,[0x013B8760] ; ret` (static __fastcall, no `this`)
// Leaf: ?Get@CStartupDefinitionManager@@SIAAV1@XZ
// Identity (naming wave 11): ?Get@CStartupDefinitionManager@@SIAAV1@XZ (Ego_r.pdb 0x00441A6D + FableWin.pdb); was the free extern "C" GetGlobal.
// The singleton pointer at 0x013B8760 keeps its bound spelling (no PDB data name; CStartupDefinitionManager::Set stores it).
extern int g_value_013b8760;
class CStartupDefinitionManager {
public:
    static CStartupDefinitionManager& __fastcall Get();
};
CStartupDefinitionManager& __fastcall CStartupDefinitionManager::Get()
{
    return *(CStartupDefinitionManager*)g_value_013b8760;
}
