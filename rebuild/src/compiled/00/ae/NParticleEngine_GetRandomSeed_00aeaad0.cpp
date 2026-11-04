// NParticleEngine::GetRandomSeed() -- return a reference to the particle engine's random seed word (the global at 0x013D2E10), which callers pass as the seed of GFFloatRandom 0x004978A0 / C3DVector::GetRandomUnitVector 0x00A145C0
// Leaf: ?GetRandomSeed@NParticleEngine@@YIAAKXZ
// Naming wave 35c: FableWin.pdb + Ego_r.pdb `?GetRandomSeed@NParticleEngine@@YIAAKXZ` (sibling SetRandomSeed `?SetRandomSeed@NParticleEngine@@YIXK@Z`);
// was the propagated label AddChildPrimitive. The seed global's own symbol is not in the name sets yet (owner's cvdump),
// so the body keeps the landed absolute address: `mov eax, 13D2E10h ; ret`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAAD0.
namespace NParticleEngine {
unsigned long& __fastcall GetRandomSeed()
{
    return *(unsigned long*)0x013D2E10;
}
}
