// CEngineWaterRenderer::SetEffectQuality(const CEffectQualitySettings* quality) -- enable high-end water only when the quality settings ask for it and the hardware supports it; always reports true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D6A0, length 48 bytes.
// Callees: none. Shape: both flags are tested through a byte register (`mov dl,[eax+70h] ; test dl,dl` / `mov al,[ecx+275h] ; test al,al`);
// the conjunction is materialised as a full-register 0/1 (`mov eax,1` / `xor eax,eax`: a bool-typed `&&` result stored to the member),
// the store of al is tail-duplicated into both paths, and `return true` only needs `mov al,1` on the zero path. The engine header carries the Ego PDB layout
// (HighendWaterEnabled +0x2a8 / HighendWaterSupported +0x2a9); retail keeps them at +0x274/+0x275, so the layout is modelled locally.
// Layout (supersedes any local-model note above): engine/CEngineWaterRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CEngineWaterRenderer CEngineWaterRendererDonor
#include "engine/CEngineWaterRenderer.h"
#undef CEngineWaterRenderer
struct CEffectQualitySettings_00b6d6a0 {
    unsigned char _pad[0x70];
    bool          HighendWater;                    // +0x70
};

class CEngineWaterRenderer : public CEngineWaterRendererDonor {
 public:
    bool SetEffectQuality(const CEffectQualitySettings_00b6d6a0* quality);
};

bool CEngineWaterRenderer::SetEffectQuality(const CEffectQualitySettings_00b6d6a0* quality)
{
    HighendWaterEnabled = quality->HighendWater && HighendWaterSupported;
    return true;
}