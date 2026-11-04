// NParticleEngine::GetMaxSpawnDistance() -- the farthest camera distance at which particle systems are spawned
// Leaf: ?GetMaxSpawnDistance@NParticleEngine@@YIMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA60, 7 bytes: fld dword [0x01399510]; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetMaxSpawnDistance@NParticleEngine@@YIMXZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `float GetMaxDrawDistance()` (old file global_GetMaxDrawDistance_00aeaa60.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
extern float g_MaxDrawDistance;                  // 0x01399510

namespace NParticleEngine {
float __fastcall GetMaxSpawnDistance()
{
    return g_MaxDrawDistance;
}
}
