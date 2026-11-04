// NParticleEngine::GetFadeOutStartDistance() -- the camera distance at which particle systems start fading out
// Leaf: ?GetFadeOutStartDistance@NParticleEngine@@YIMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA20, 7 bytes: fld dword [0x0129F19C]; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetFadeOutStartDistance@NParticleEngine@@YIMXZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" float GetGlobal()` (old file CAIStateGroupEnemyCreatureBase_GetCombatMinDistance_00aeaa20.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
extern float g_value_0129f19c;                   // 0x0129F19C

namespace NParticleEngine {
float __fastcall GetFadeOutStartDistance()
{
    return g_value_0129f19c;
}
}
