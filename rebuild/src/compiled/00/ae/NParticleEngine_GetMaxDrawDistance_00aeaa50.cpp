// NParticleEngine::GetMaxDrawDistance() -- the farthest camera distance at which particle systems are drawn
// Leaf: ?GetMaxDrawDistance@NParticleEngine@@YIMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA50, 7 bytes: fld dword [0x01399514]; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetMaxDrawDistance@NParticleEngine@@YIMXZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" float GetGlobal()` (old file global_GetMaxDrawDistance_00aeaa50.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
extern float g_value_01399514;                   // 0x01399514

namespace NParticleEngine {
float __fastcall GetMaxDrawDistance()
{
    return g_value_01399514;
}
}
