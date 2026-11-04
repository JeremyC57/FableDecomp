// NParticleEngine::SetGraphicBank(const CGraphicDataBank&) -- remember the graphic bank the particle renderer draws sprites from (stores its address)
// Leaf: ?SetGraphicBank@NParticleEngine@@YIXABVCGraphicDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAA80, 7 bytes: mov [0x013D2E08],ecx; ret (__fastcall, no `this`).
// Naming wave 35c: Ego_r.pdb + FableWin.pdb ?SetGraphicBank@NParticleEngine@@YIXABVCGraphicDataBank@@@Z (the NParticleEngine accessor family 0x00AEAA00..0x00AEAB20 in
//   declaration order); the catalog carries the name. Was the stand-in `void SetCurrentThingManager(void*)` (old file global_SetCurrentThingManager_00aeaa80.cpp); the data symbol is unchanged
//   (its retail name waits on the owner's cvdump).
class CGraphicDataBank;
extern void* g_value;                              // 0x013D2E08 (read back by GetGraphicBank 0x00AEAAA0)

namespace NParticleEngine {
void __fastcall SetGraphicBank(const CGraphicDataBank& bank)
{
    g_value = (void*)&bank;
}
}
