// NParticleEngine::GetMeshBank() -- the mesh bank SetMeshBank stored
// Leaf: ?GetMeshBank@NParticleEngine@@YIABVCMeshDataBank@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAAB0, 6 bytes: mov eax,[0x013D2E0C]; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetMeshBank@NParticleEngine@@YIABVCMeshDataBank@@XZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `long GFGetPreMainMemoryUsed()` (old file global_GFGetPreMainMemoryUsed_00aeaab0.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
class CMeshDataBank;
extern "C" const CMeshDataBank* g_GFPreMainMemoryUsed;   // 0x013D2E0C (stored by SetMeshBank 0x00AEAA90); C linkage, symbol unchanged

namespace NParticleEngine {
const CMeshDataBank& __fastcall GetMeshBank()
{
    return *g_GFPreMainMemoryUsed;
}
}
