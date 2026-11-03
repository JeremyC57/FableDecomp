// NParticleEngine::GetTickRate() -- the particle engine's tick rate (updates per second): `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: ?GetTickRate@NParticleEngine@@YIJXZ
// Name: FableWin.pdb / Ego_r.pdb ?GetTickRate@NParticleEngine@@YIJXZ (the Set/GetTickRate, GetOneOverTickRate family); the body returns
//   the long at 0x01399504 (15), its neighbour 0x00AEAA00 the float 1/15 at 0x01399508 (GetOneOverTickRate), and every caller
//   (0x00B1DB00, 0x00B18AF0, 0x00B10220, 0x00B19C60) converts the result to the tick-rate float. Data symbol as established.
extern int g_value_01399504;   // 0x01399504
namespace NParticleEngine {
long __fastcall GetTickRate() { return g_value_01399504; }
}
