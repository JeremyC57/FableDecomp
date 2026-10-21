// NUISystem::CManager::GetInstance() -- returns the front-end UI manager singleton, creating it (0xD0 bytes) on first use
// Retail: Fable.exe @ 0x0041E5F2, 41 bytes, static. Callees: operator new 0x00BFEA1A, the CManager constructor 0x0041E3F6
// (stores vtable 0x01230134 = NUISystem::CManager : NUISystem::CObservable by RTTI; the catalog's module
// "CFrontEndManager" names the caller family). Global: the instance pointer 0x013B8710.
// Shape: size TU (`pop ecx` after the allocation call); VC7.1's null-tested new expression; the instance is stored from eax on both paths and eax is returned.
#pragma optimize("s", on)
namespace NUISystem {
class CManager {
public:
    unsigned char _body[0xd0];
    CManager();                                          // 0x0041E3F6
    static CManager* GetInstance();
};
extern CManager* g_pUIManagerInstance;                   // 0x013B8710

CManager* CManager::GetInstance()
{
    if (g_pUIManagerInstance == 0)
        g_pUIManagerInstance = new CManager;
    return g_pUIManagerInstance;
}
}