// CEngineScreenEffectGlowRenderer::SetConfiguration(int configuration) -- load one of the eight preset glow configurations (RGB threshold, RGB intensity, motion blur) into the renderer; out-of-range indices are ignored
// Leaf: ?SetConfiguration@CEngineScreenEffectGlowRenderer@@QAEXH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5EBF0, 103 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetConfiguration@CEngineScreenEffectGlowRenderer@@QAEXH@Z, the only int-argument
//   setter of the class; its one caller 0x00B5F056 is the console command GlowSetConfiguration (static fastcall: renderer in
//   ecx, the CGenericVar vector in edx, passes the first argument's int), and the seven dwords it writes are exactly the PDB
//   members ThresholdR +0x1FC .. MotionBlur +0x214. It sits between the radial-blur renderer's and the glow renderer's
//   methods (0x00B5EAD0 / 0x00B5EC60).
// Data: the preset table at 0x0139A580 -- 8 entries of 0x1C bytes (seven floats in member order); its name is descriptive.
// Layout: local model -- engine/CEngineScreenEffectGlowRenderer.h is a plain layout struct (no methods can be declared on
//   it), so the class is re-modelled here with the header's offsets for the members this body touches.
class CGlowConfiguration_0139a580 {
public:
    float ThresholdR;                                    // +0x00
    float ThresholdG;                                    // +0x04
    float ThresholdB;                                    // +0x08
    float IntensityR;                                    // +0x0C
    float IntensityG;                                    // +0x10
    float IntensityB;                                    // +0x14
    float MotionBlur;                                    // +0x18
};
extern CGlowConfiguration_0139a580 g_GlowConfigurations_0139a580[8];   // 0x0139A580

class CEngineScreenEffectGlowRenderer {
public:
    unsigned char _pad_0x000[0x1fc];                     // vptr, CEngineComponent base, textures, shaders, vertex arrays
    float         ThresholdR;                            // +0x1FC
    float         ThresholdG;                            // +0x200
    float         ThresholdB;                            // +0x204
    float         IntensityR;                            // +0x208
    float         IntensityG;                            // +0x20C
    float         IntensityB;                            // +0x210
    float         MotionBlur;                            // +0x214
    void SetConfiguration(int configuration);
};

void CEngineScreenEffectGlowRenderer::SetConfiguration(int configuration)
{
    if (configuration >= 0 && configuration <= 7)
    {
        ThresholdR = g_GlowConfigurations_0139a580[configuration].ThresholdR;
        ThresholdG = g_GlowConfigurations_0139a580[configuration].ThresholdG;
        ThresholdB = g_GlowConfigurations_0139a580[configuration].ThresholdB;
        IntensityR = g_GlowConfigurations_0139a580[configuration].IntensityR;
        IntensityG = g_GlowConfigurations_0139a580[configuration].IntensityG;
        IntensityB = g_GlowConfigurations_0139a580[configuration].IntensityB;
        MotionBlur = g_GlowConfigurations_0139a580[configuration].MotionBlur;
    }
}