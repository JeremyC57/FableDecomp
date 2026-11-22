// NUISystem::CManager::GetInstance() -- returns the front-end UI manager singleton, creating it (0xD0 bytes) on first use
// Retail: Fable.exe @ 0x0041E5F2, 41 bytes, static. Callees: operator new 0x00BFEA1A, the CManager constructor 0x0041E3F6
// (stores vtable 0x01230134 = NUISystem::CManager : NUISystem::CObservable by RTTI; the catalog's module
// "CFrontEndManager" names the caller family). Global: the instance pointer 0x013B8710.
// Names (NF225, 2026-11-22): the singleton is the protected static ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A (Ego_r.pdb, FableWin.pdb,
//   Ego_d.pdb), respelled in place in this row's model (was a made-up namespace global ?g_pUIManagerInstance@NUISystem@@3PAVCManager@1@A).
// Shape: size TU (`pop ecx` after the allocation call); VC7.1's null-tested new expression; the instance is stored from eax on both paths and eax is returned.
#pragma optimize("s", on)
namespace NUISystem {
class CManager {
public:
    unsigned char _body[0xd0];
    CManager();                                          // 0x0041E3F6
    static CManager* __fastcall GetInstance();          // ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (Ego_r / FableWin)
protected:
    static CManager* GPUniqueInstance;                   // 0x013B8710 ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A
};

CManager* CManager::GetInstance()
{
    if (GPUniqueInstance == 0)
        GPUniqueInstance = new CManager;
    return GPUniqueInstance;
}
}