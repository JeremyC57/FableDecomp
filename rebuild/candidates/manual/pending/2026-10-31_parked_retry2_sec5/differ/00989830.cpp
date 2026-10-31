// CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight* light) -- convert the light into slot `index`'s generic light, enable it and flag it for update, publish its attenuation pair into the packed attenuation table (slots >= 1), and mark the light constants dirty
// Leaf: InitialiseLight
// Grade: DIFFER(98v99) -- one instruction: with the component index declared before the block pointer the schedule
//   matches retail (table load, shr, and, shl, add, then the Attenuation2 load); the only residue is that retail
//   recomputes index - 1 with a second lea ecx,[ebx-1] where VC7.1 here copies it (mov ecx,eax). %4 and /4, a named
//   light index, an inline component helper, signed casts do not move it (sec5 retry: 8 grades, 9 -> 2 diff lines)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00989830, length 99 bytes.
// Callees: CShaderLight virtual slot 1 (+0x4) ConvertToGenericLight(CShaderGenericLight&) (the landed
// CShaderDirectionalLight override is 0x00F39D40).
// Shape: a reference local to Lights[index] (`imul esi,esi,38h`); both bools set from the `mov eax,1`
// register that the `index >= 1` compare reuses; slot j = index - 1 (computed twice) lands in component
// j & 3 of the C4DVector pair (j >> 2) * 2 / +1 (Attenuation2, AttenuationBase); `UpdateFlags |= 0x700` in
// memory form.
// Layout: PDB CLightInfo (0x38): CShaderGenericLight Light (Attenuation2 +0x28, AttenuationBase +0x2c),
// IsEnabled +0x34, UpdateFlag +0x35. CShaderRenderManager: PDB Lights +0x16c, LightAttenuations +0x1ac,
// UpdateFlags +0x3f8; retail +0x164, +0x1a0 (the CArray's data pointer), +0x3d8.
struct C4DVector {
    float v[4];
};

struct SAttenuationBlock {                       // two C4DVectors: four lights' Attenuation2, then AttenuationBase
    C4DVector Attenuation2;
    C4DVector AttenuationBase;
};

struct CShaderGenericLight {                     // 0x34 bytes
    C4DVector     Parameters[2];                 // +0x00
    long          Types[2];                      // +0x20
    float         Attenuation2;                  // +0x28
    float         AttenuationBase;               // +0x2c
    unsigned long Size;                          // +0x30
};

struct CLightInfo {                              // 0x38 bytes
    CShaderGenericLight Light;                   // +0x00
    bool                IsEnabled;               // +0x34
    bool                UpdateFlag;              // +0x35
};

struct CShaderLight {
    virtual void v00();
    virtual void ConvertToGenericLight(CShaderGenericLight& light) const;   // slot 1 (+0x4)
};

struct CShaderRenderManager {
    unsigned char _pad000[0x164];
    CLightInfo*   Lights;                        // +0x164
    unsigned char _pad168[0x1a0 - 0x168];
    SAttenuationBlock* LightAttenuations;        // +0x1a0 (CArray<C4DVector> data, viewed as pairs)
    unsigned char _pad1a4[0x3d8 - 0x1a4];
    unsigned int  UpdateFlags;                   // +0x3d8
    void InitialiseLight(unsigned long index, const CShaderLight* light);
};

void CShaderRenderManager::InitialiseLight(unsigned long index, const CShaderLight* light)
{
    CLightInfo& info = Lights[index];
    light->ConvertToGenericLight(info.Light);
    info.IsEnabled = true;
    info.UpdateFlag = true;
    if (index >= 1) {
        unsigned long component = (index - 1) & 3;
        SAttenuationBlock* block = &LightAttenuations[(index - 1) >> 2];
        block->Attenuation2.v[component] = info.Light.Attenuation2;
        block->AttenuationBase.v[component] = info.Light.AttenuationBase;
    }
    unsigned int* flags = &UpdateFlags;          // a pointer local gives the memory-form `or`
    *flags |= 0x700u;
}
