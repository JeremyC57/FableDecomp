// NParticleEngine::GetFadeInEndDistance() -- the camera distance at which particle systems are fully faded in
// Leaf: ?GetFadeInEndDistance@NParticleEngine@@YIMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA30, 7 bytes: fld dword [0x0129F198]; ret (the same float as GetFadeInStartDistance 0x00AEAA40) (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?GetFadeInEndDistance@NParticleEngine@@YIMXZ (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `extern "C" float GetGlobal()` (old file CAIStateGroupEnemyCreatureBase_GetCombatMinDistance_00aeaa30.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
extern float g_value_0129f198;                   // 0x0129F198

namespace NParticleEngine {
float __fastcall GetFadeInEndDistance()
{
    return g_value_0129f198;
}
}
