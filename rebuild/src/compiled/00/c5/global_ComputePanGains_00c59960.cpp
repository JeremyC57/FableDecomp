// ComputePanGains(float pan, float* left, float* right) -- linear stereo pan law for the software voices: pan > 0 attenuates the left gain to 1 - pan, pan <= 0 attenuates the right gain to 1 + pan; an output in mode 2 ignores the pan (both gains 1)
// Leaf: ?ComputePanGains@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59960, length 89 bytes (+ int3 padding); `ret` (__cdecl).
// Callees: CCodaSystem::GetOutputMode 0x00C3EA60 on g_pCodaSystem (0x0143BC2C): `m_188 ? m_188->m_4 : 0x80000000`.
// Callers: 0x00C5CBA9 / 0x00C5CBD3 (the voice's pan set-up: (params->pan, &gains[0], &gains[1]), each gain then
// scaled by the volume).
// Label: the manifest's "GUI_ComputeAlphaBlend" is propagated; GameCODA has no PDB, names are descriptive.
// Shape: `pan = 0.0f` for mode 2 lets VC7.1 thread that path straight into the `pan <= 0` arm with `fld 0.0`
// (the `0.0 + 1.0` is not folded); writing the `pan > 0` arm first keeps retail's block order (the `<=` spelling
// adds two bytes).
class CCodaSystem {
public:
    long GetOutputMode();               // 0x00C3EA60
};
extern CCodaSystem* g_pCodaSystem;      // 0x0143BC2C

void __cdecl ComputePanGains(float pan, float* left, float* right)
{
    if (g_pCodaSystem->GetOutputMode() == 2)
        pan = 0.0f;
    if (pan > 0.0f) {
        *left = 1.0f - pan;
        *right = 1.0f;
    } else {
        *left = 1.0f;
        *right = pan + 1.0f;
    }
}