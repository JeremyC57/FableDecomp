// CMainGameComponent::InitPlayerManager() -- create the player manager for this game component and hand it to PPlayerManager
// Leaf: ?InitPlayerManager@CMainGameComponent@@IAEXXZ
// Retail: Fable.exe @ 0x0041732A, 48 bytes; Ego_r.pdb ?InitPlayerManager@CMainGameComponent@@IAEXXZ (protected); caller
// 0x004186E4. Body: operator new(0x2c) (0x00BFEA1A); when it succeeds, CPlayerManager::CPlayerManager(CMainGameComponent&,
// const CGameDefinitionManager&) (0x0044A3B0; Ego_r.pdb ??0CPlayerManager@@QAE@AAVCMainGameComponent@@ABVCGameDefinitionManager@@@Z)
// with the definition manager from CGameDefinitionManager::Get (0x0044C6B0, evaluated first); then scoped_ptr::reset
// (0x004193A0: store the new pointer, virtual-delete the old one) on PPlayerManager (+0x1c).
// Layout: Ego_r.pdb CMainGameComponent PPlayerManager +0x1c (retail agrees below +0x24, as PWorld +0x24 in 0x00416392).
// Opt: size TU (unaligned entry, `pop ecx` after the cdecl operator new); no EH frame around the new-expression.
#pragma optimize("s", on)
class CMainGameComponent;
class CGameDefinitionManager {
public:
    static const CGameDefinitionManager& Get();          // 0x0044C6B0
};
class CPlayerManager {
public:
    unsigned char _body[0x2c];
    CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& defs);   // 0x0044A3B0
};
template <class T> class scoped_ptr {
public:
    T* Ptr;
    void reset(T* p);                                    // 0x004193A0
};
class CMainGameComponent {
public:
    unsigned char              _pad00[0x1c];
    scoped_ptr<CPlayerManager> PPlayerManager;           // +0x1c
protected:
    void InitPlayerManager();
};
void CMainGameComponent::InitPlayerManager()
{
    PPlayerManager.reset(new CPlayerManager(*this, CGameDefinitionManager::Get()));
}