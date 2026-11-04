// NParticleEngine::SetMeshBank(const CMeshDataBank&) -- remember the mesh bank the particle mesh components draw from (stores its address)
// Leaf: ?SetMeshBank@NParticleEngine@@YIXABVCMeshDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA90, 7 bytes: mov [0x013D2E0C],ecx; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?SetMeshBank@NParticleEngine@@YIXABVCMeshDataBank@@@Z (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `void SetCurrentThingManager(void*)` (old file global_SetCurrentThingManager_00aeaa90.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
class CMeshDataBank;
extern void* g_value_013d2e0c;                     // 0x013D2E0C (read back by GetMeshBank 0x00AEAAB0)

namespace NParticleEngine {
void __fastcall SetMeshBank(const CMeshDataBank& bank)
{
    g_value_013d2e0c = (void*)&bank;
}
}
