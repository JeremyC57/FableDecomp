// CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight& light) -- convert the light into slot `index`'s generic light, enable it and flag it for update, publish its attenuation pair into the packed attenuation table (slots >= 1), and mark the light constants dirty
// Leaf: ?InitialiseLight@CShaderRenderManager@@QAEXKABVCShaderLight@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00989830, 99 bytes, __fastcall, `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?InitialiseLight@CShaderRenderManager@@QAEXKABVCShaderLight@@@Z (the light by const reference).
// Callees: CShaderLight virtual slot 1 (+0x4) ConvertToGenericLight(CShaderGenericLight&) const (Ego_r.pdb overrides
//   ?ConvertToGenericLight@CShaderDirectionalLight@@UBEXAAVCShaderGenericLight@@@Z / CShaderPointLight).
// Shape: Lights[index] (`imul esi,esi,38h`); both bools set from the `mov eax,1` the `index >= 1` compare reuses; slot
//   j = index - 1 lands in component j % 4 of the C4DVector pair LightAttenuations[(j / 4) * 2] (Attenuation2) / +1
//   (AttenuationBase); UpdateFlags |= 0x700 in memory form.
//   Retail computes index-1 twice (`lea eax,[ebx-1] ; shr eax,2` and `lea ecx,[ebx-1] ; and ecx,3`, 0x00989865 / 0x0098986B):
//   the pair index is the unsigned quotient, the component a signed index (the C4DVector component subscript is an int), so
//   VC7.1 does not merge the two subtractions; both are named before the pair address is formed.
#include "engine/CShaderRenderManager.h"
#include "engine/CShaderLight.h"

void CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight& light)
{
    CLightInfo& info = Lights[index];
    light.ConvertToGenericLight(info.Light);
    info.IsEnabled = true;
    info.UpdateFlag = true;
    if (index >= 1) {
        unsigned long pair = (index - 1) >> 2;
        long component = (long)(index - 1) & 3;
        float* attenuation = &LightAttenuations[pair * 2].X;
        attenuation[component] = info.Light.Attenuation2;
        attenuation[component + 4] = info.Light.AttenuationBase;
    }
    UpdateFlags |= 0x700;
}