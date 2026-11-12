// CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight& light) -- convert the light into slot `index`'s generic light, enable it and flag it for update, publish its attenuation pair into the packed attenuation table (slots >= 1), and mark the light constants dirty
// Grade: DIFFER(98v99) -- shared engine/CShaderRenderManager.h; one residue: VC7.1 CSEs index-1 (lea eax,[ebx-1]; mov ecx,eax) where retail rematerialises a second lea [ebx-1] (0x0098986B). Block-first spelling (attenuation[(index-1)%4], 99v99) recomputes the lea but hoists the Attenuation2 load above the shl/add. Next: c2prio on the CSE, or a spelling whose comp tuple precedes without CSE.
// Leaf: ?InitialiseLight@CShaderRenderManager@@QAEXKABVCShaderLight@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00989830, 99 bytes, __thiscall, `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?InitialiseLight@CShaderRenderManager@@QAEXKABVCShaderLight@@@Z (the light by const reference).
// Callees: CShaderLight virtual slot 1 (+0x4) ConvertToGenericLight(CShaderGenericLight&) const (Ego_r.pdb overrides
//   ?ConvertToGenericLight@CShaderDirectionalLight@@UBEXAAVCShaderGenericLight@@@Z / CShaderPointLight).
// Shape: Lights[index] (`imul esi,esi,38h`); both bools set from the `mov eax,1` the `index >= 1` compare reuses; slot
//   j = index - 1 lands in component j % 4 of the C4DVector pair LightAttenuations[(j / 4) * 2] (Attenuation2) / +1
//   (AttenuationBase); UpdateFlags |= 0x700 in memory form.
#include "engine/CShaderRenderManager.h"

class CShaderLight : public CBaseClassNonCopyable {                 // no shared header; vtable 0x012A2108
public:
    virtual void ConvertToGenericLight(CShaderGenericLight& generic_light) const = 0;   // slot 1 (+0x4)
};

void CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight& light)
{
    CLightInfo& info = Lights[index];
    light.ConvertToGenericLight(info.Light);
    info.IsEnabled = true;
    info.UpdateFlag = true;
    if (index >= 1) {
        unsigned long component = (index - 1) % 4;
        float* attenuation = &LightAttenuations[((index - 1) / 4) * 2].X;
        attenuation[component] = info.Light.Attenuation2;
        attenuation[component + 4] = info.Light.AttenuationBase;
    }
    UpdateFlags |= 0x700;
}
