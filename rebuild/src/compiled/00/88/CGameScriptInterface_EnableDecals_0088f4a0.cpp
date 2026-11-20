// CGameScriptInterface::EnableDecals(bool enable) const -- script switch for decals: sets the particle emitter manager's EnableDecals bit
// Leaf: ?EnableDecals@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F4A0, thiscall `ret 4` (`this` unused).
// Identity: Ego_r.pdb 0x008CC250 + FableWin.pdb 0x02AA7F10 ?EnableDecals@CGameScriptInterface@@UBEX_N@Z (virtual, slot 688 of
//   engine/CGameScriptInterface.h, which already names 0x0088F4A0); no landed row references 0x0088F4A0 (exe scan).
// Naming (NF158, 2026-11-20, q4754): was the stand-in ?EnableDecals@CGameScriptInterface_Methods@@QAEXD@Z calling a made-up
//   ?GetDecalObj@@YAPAUGObj@@XZ. Now on the shared headers: NParticleEngine::GetParticleEmitterManager 0x00AEAAF0 (Ego_r + FableWin
//   ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ, the self row's spelling) and the inline
//   CParticleEmitterManager::SetEnableDecals (FableWin ?SetEnableDecals@CParticleEmitterManager@NParticleEngine@@QAEX_N@Z; the bit at +0x1010).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_SETENABLEDECALS
#include "engine/CParticleEmitterManager.h"

void CGameScriptInterface::EnableDecals(bool enable) const
{
    NParticleEngine::GetParticleEmitterManager().SetEnableDecals(enable);
}
