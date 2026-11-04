// NParticleEngine::GetOneOverTickRate() -- the particle tick interval (1 / tick rate) the update code steps by
// Leaf: ?GetOneOverTickRate@NParticleEngine@@YIMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA00, 7 bytes: fld dword [0x01399508]; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetOneOverTickRate@NParticleEngine@@YIMXZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `float GetMaxDrawDistance()` (old file global_GetMaxDrawDistance_00aeaa00.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
extern float g_maxDrawDistance;                  // 0x01399508

namespace NParticleEngine {
float __fastcall GetOneOverTickRate()
{
    return g_maxDrawDistance;
}
}

float g_maxDrawDistance = 0.0f;
