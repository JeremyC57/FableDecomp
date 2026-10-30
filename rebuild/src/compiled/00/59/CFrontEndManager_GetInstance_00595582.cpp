// NUISystem::CFrontEndManager::GetInstance() @ 0x00595582 -- the front-end manager singleton, created (0xE0 bytes) on first use
// Leaf: ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ
// Identity (naming wave 11): Ego_r.pdb 0x006043BE + FableWin.pdb 0x02010AF6 ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ; the ctor 0x005953E2 precedes it as in Ego_r.
//
// Shape: a size TU (`pop ecx` after the allocation call); VC7.1's null-tested new expression (`jmp` / `xor eax,eax`) stores
// the instance from eax on both paths and returns it.

#pragma optimize("s", on)

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char _body[0xe0];                           // retail sizeof (engine/CFrontEndManager.h)
    CFrontEndManager();                                  // 0x005953E2 ??0CFrontEndManager@NUISystem@@QAE@XZ
    static CFrontEndManager* __fastcall GetInstance();
};
}
extern "C" NUISystem::CFrontEndManager* FableFrontEndManagerSingleton;   // 0x013B8B5C

NUISystem::CFrontEndManager* NUISystem::CFrontEndManager::GetInstance()
{
    if (FableFrontEndManagerSingleton == 0)
        FableFrontEndManagerSingleton = new CFrontEndManager;
    return FableFrontEndManagerSingleton;
}

#pragma optimize("s", off)
