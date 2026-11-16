// NParticleEngine::GetGraphicBank() -- return the particle engine's graphic data bank (the pointer SetGraphicBank 0x00AEAA80 stores)
// Leaf: ?GetGraphicBank@NParticleEngine@@YIABVCGraphicDataBank@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAAA0, 6 bytes: mov eax,[0x013D2E08]; ret (__fastcall free function, no arguments).
// Identity: Ego_r.pdb 0x00AAEF30 / FableWin.pdb 0x02EF2C80 ?GetGraphicBank@NParticleEngine@@YIABVCGraphicDataBank@@XZ (landing
//   2026-11-03h, chunk 737 NAME_FIXES); the bank's slot 20 (GetNoFramesInGraphic) 0x00B087C0 is called on the result at 0x00B08C19.
// Port (NF40, round 4): replaces the extern "C" `GetGlobal` stand-in. The 0x013D2E08 pointer has no name in either PDB name set
//   (file-static in the particle engine TU): `GParticleGraphicBank` is descriptive (see NOTES); 0x00AEAA80 still calls it g_value.
class CGraphicDataBank;
extern const CGraphicDataBank* GParticleGraphicBank;   // 0x013D2E08 (descriptive)

namespace NParticleEngine {
const CGraphicDataBank& __fastcall GetGraphicBank()
{
    return *GParticleGraphicBank;
}
}
