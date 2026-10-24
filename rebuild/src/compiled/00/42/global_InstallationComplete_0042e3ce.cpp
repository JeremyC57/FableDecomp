// InstallationComplete() -- nonzero when no file installer was ever created or the installer has finished copying
// Leaf: ?InstallationComplete@@YIHXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042E3CE, length 32 bytes, plain `ret` (__fastcall, no arguments).
// Name: FableWin.pdb / Ego_r.pdb `?InstallationComplete@@YI_NXZ` (the only installer-completion free function); its address is
// taken (`mov eax,0x0042E3CE`) by CNewFrontendGameComponent::Run @ 0x0042EC7C, which stores it as a loading-screen callback.
// The catalog's "FrontEnd_CheckQuickAssignInventory" is a label.
// Callees: CFileInstallerSingleton::Get @ 0x00404440 (landed, __fastcall, returns CFileInstaller&),
// CFileInstaller::IsComplete() const @ 0x009C32B0 (FableWin `?IsComplete@CFileInstaller@@QBE_NXZ`, returns the bool at +0x80;
// the catalog's "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated from an identical body).
// Global: the singleton's counted pointer at 0x013CA818 (its object word is tested inline).
// Return type: retail returns a FULL-REGISTER 0/1 (`xor eax,eax` / `xor eax,eax ; inc eax`), which VC7.1 emits only for an
// int-sized result: the same body declared `bool` (the donor PDB's `_N`) compiles to `xor al,al` / `mov al,1` (graded DIFFER 29v32).
// So the retail function returns an int-sized flag (BOOL); the movie player's callback parameter is `bool (__fastcall*)()` in the
// donor (`P6I_NXZ` in ?PlayXBoxMovie@CXMVPlayer@@), so retail stores this address through a cast or a typedef'd BOOL callback.
// Shape: the singleton null test and the IsComplete result both branch to the `return 1` exit; `return 0` falls through.
// Pragma: #pragma optimize("s", on) (the `xor ; inc` constant).
#pragma optimize("s", on)
class CFileInstaller {
public:
    bool IsComplete() const;                             // 0x009C32B0
};
template <class T> class CCountedPointer {
public:
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
};
class CFileInstallerSingleton {
public:
    static CFileInstaller& __fastcall Get();             // 0x00404440
    static bool Exists() { return Instance.Data != 0; }
    static CCountedPointer<CFileInstaller> Instance;     // 0x013CA818
};

int __fastcall InstallationComplete()
{
    if (CFileInstallerSingleton::Exists() && !CFileInstallerSingleton::Get().IsComplete())
        return 0;
    return 1;
}