// CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName() -- returns "EnableScreenEffectRadialBlurRenderer" as a CCharString
// Leaf: ?GetConsoleEnableFunctionName@CEngineScreenEffectRadialBlurRenderer@@UAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EAD0: the hidden result is constructed through CCharString(const char*, long) 0x0099EBF0
//   with length -1, its address returned in eax.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CEngineScreenEffectRadialBlurRenderer.h
//   (FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER; was ?GetConsoleEnableFunctionName@CEngineScreenEffectRadialBlurRenderer_Methods@@...
//   over a local CCharString model); the override is declared by the add-only opt-in
//   FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_GETCONSOLEENABLEFUNCTIONNAME_DECL (both PDB name sets; the RTTI placement names it
//   here), CCharString is fable_string.h's (through engine/CEngineComponent.h), as the landed water / outline-glow overrides.
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_GETCONSOLEENABLEFUNCTIONNAME_DECL
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

CCharString CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("EnableScreenEffectRadialBlurRenderer", -1);
}
