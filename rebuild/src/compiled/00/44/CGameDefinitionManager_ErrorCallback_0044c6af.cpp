// CGameDefinitionManager::ErrorCallback(const CCharString& error) -- the game definition manager's error callback; empty in
// the retail build
// Leaf: ?ErrorCallback@CGameDefinitionManager@@KIXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C6AF, 1 byte: `ret` (static __fastcall, the argument arrives in ecx).
// Identity (sweep3; was the address-named Stub_ReturnVoid_0044c6af): Ego_r.pdb lists
//   ?ErrorCallback@CGameDefinitionManager@@KIXABVCCharString@@@Z as a 1-byte function directly before
//   ?Get@CGameDefinitionManager@@SIAAV1@XZ (6 bytes) and ?IsAllocated@CGameDefinitionManager@@SI_NXZ, exactly as retail
//   0x0044C6AF / 0x0044C6B0 / 0x0044C6B6; its only reference is the function pointer the CGameDefinitionManager
//   constructor 0x0044C6C2 pushes (0x0044C6D6 `push 0x0044C6AF`) as the definition manager's error callback.
class CCharString;

class CGameDefinitionManager {
protected:
    static void __fastcall ErrorCallback(const CCharString& error);
};

void __fastcall CGameDefinitionManager::ErrorCallback(const CCharString& /*error*/)
{
}
