// CShaderRenderManager::UpdateWorldSpaceLighting() -- upload the lights whose UpdateFlag is set (or that are enabled) into the
//   vertex-shader light array: each light fills LightSize float4 registers of LightBuffer (its Parameters when enabled, else
//   (0,0,0,1)), contiguous dirty runs are flushed with one SetVertexShaderConstantF, then the attenuation table for the
//   lights in use is uploaded and the dirty flags move from 0x200 to 0x100.
// Leaf: UpdateWorldSpaceLighting
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098A540, 544 bytes (539 + int3 padding), plain `ret`.
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, +0x178) through the inlined SetVSConstants 0x0098B930
//   (`if (count != 0)` around the device call).
// Layout: retail CShaderRenderManager = Ego_r.pdb minus 8 from Lights (the 16-byte PDB CArray at +0xc is 12 bytes in retail),
//   as the landed UpdateLightGlobals 0x0098A760 (LightGlobals +0x194) and SetVSConstants (D3DDevice +0x1d4):
//   CurrentVSConstantLayout +0x4, Lights +0x164, LightBuffer +0x168, LightCount +0x16c, LightAttenuations +0x1a0,
//   UpdateFlags +0x3d8. CVertexShaderConstantLayout (engine header, ranges opaque there): LightSize +0x4c,
//   LightArray.Offset +0x54, LightAttenuations.Offset +0x6c (CShaderConstantRange = {vptr, Offset, Count}).
//   CLightInfo (0x38): CShaderGenericLight {C4DVector Parameters[2]; EParameterType Types[2]; float Attenuation2,
//   AttenuationBase; ulong Size +0x30}, IsEnabled +0x34, UpdateFlag +0x35.
// Shape: the enabled copy reads Size into a local (count-down loop), the disabled fill builds its (0,0,0,1) temporary
//   inside the loop (hoisted with the zero register); the attenuation register and count are locals before the call, and
//   the table's data pointer defaults to the array object itself and becomes _M_start only when it is non-empty
//   (`test; jbe`, as retail's GetData-style accessor).
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct CShaderConstantRange {
    virtual ~CShaderConstantRange();
    long          Offset;                        // +0x4
    unsigned long Count;                         // +0x8
};
struct CVertexShaderConstantLayout {
    unsigned char        _pad00[0x4c];
    unsigned long        LightSize;              // +0x4c
    CShaderConstantRange LightArray;             // +0x50
    CShaderConstantRange LightGlobals;           // +0x5c
    CShaderConstantRange LightAttenuations;      // +0x68
};
struct CShaderGenericLight {
    C4DVector     Parameters[2];                 // +0x00
    long          Types[2];                      // +0x20
    float         Attenuation2;                  // +0x28
    float         AttenuationBase;               // +0x2c
    unsigned long Size;                          // +0x30
};
struct CLightInfo {
    CShaderGenericLight Light;                   // +0x00
    bool          IsEnabled;                     // +0x34
    bool          UpdateFlag;                    // +0x35
};
template <class T> struct CArray {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T* GetData() const
    {
        const T* p = (const T*)this;
        if (size() > 0)
            p = _M_start;
        return p;
    }
};
struct IDirect3DDevice9 {
    virtual void d000() const; virtual void d001() const; virtual void d002() const; virtual void d003() const; virtual void d004() const; virtual void d005() const;
    virtual void d006() const; virtual void d007() const; virtual void d008() const; virtual void d009() const; virtual void d010() const; virtual void d011() const;
    virtual void d012() const; virtual void d013() const; virtual void d014() const; virtual void d015() const; virtual void d016() const; virtual void d017() const;
    virtual void d018() const; virtual void d019() const; virtual void d020() const; virtual void d021() const; virtual void d022() const; virtual void d023() const;
    virtual void d024() const; virtual void d025() const; virtual void d026() const; virtual void d027() const; virtual void d028() const; virtual void d029() const;
    virtual void d030() const; virtual void d031() const; virtual void d032() const; virtual void d033() const; virtual void d034() const; virtual void d035() const;
    virtual void d036() const; virtual void d037() const; virtual void d038() const; virtual void d039() const; virtual void d040() const; virtual void d041() const;
    virtual void d042() const; virtual void d043() const; virtual void d044() const; virtual void d045() const; virtual void d046() const; virtual void d047() const;
    virtual void d048() const; virtual void d049() const; virtual void d050() const; virtual void d051() const; virtual void d052() const; virtual void d053() const;
    virtual void d054() const; virtual void d055() const; virtual void d056() const; virtual void d057() const; virtual void d058() const; virtual void d059() const;
    virtual void d060() const; virtual void d061() const; virtual void d062() const; virtual void d063() const; virtual void d064() const; virtual void d065() const;
    virtual void d066() const; virtual void d067() const; virtual void d068() const; virtual void d069() const; virtual void d070() const; virtual void d071() const;
    virtual void d072() const; virtual void d073() const; virtual void d074() const; virtual void d075() const; virtual void d076() const; virtual void d077() const;
    virtual void d078() const; virtual void d079() const; virtual void d080() const; virtual void d081() const; virtual void d082() const; virtual void d083() const;
    virtual void d084() const; virtual void d085() const; virtual void d086() const; virtual void d087() const; virtual void d088() const; virtual void d089() const;
    virtual void d090() const; virtual void d091() const; virtual void d092() const; virtual void d093() const;
    virtual long __stdcall SetVertexShaderConstantF(unsigned int startRegister, const float* data, unsigned int vector4fCount); // slot 94 (+0x178)
};
struct CShaderRenderManager {
    void*                        __vftable;
    CVertexShaderConstantLayout* CurrentVSConstantLayout;    // +0x004
    unsigned char                _pad008[0x164 - 0x8];
    CLightInfo*                  Lights;                     // +0x164
    C4DVector*                   LightBuffer;                // +0x168
    unsigned long                LightCount;                 // +0x16c
    unsigned char                _pad170[0x1a0 - 0x170];
    CArray<C4DVector>            LightAttenuations;          // +0x1a0
    unsigned char                _pad1ac[0x1d4 - 0x1ac];
    IDirect3DDevice9*            D3DDevice;                  // +0x1d4
    unsigned char                _pad1d8[0x3d8 - 0x1d8];
    unsigned long                UpdateFlags;                // +0x3d8
    void SetVSConstants(long startRegister, const C4DVector* data, unsigned long count)
    {
        if (count != 0)
            D3DDevice->SetVertexShaderConstantF(startRegister, (const float*)data, count);
    }
    void UpdateWorldSpaceLighting();
};

void CShaderRenderManager::UpdateWorldSpaceLighting()
{
    long first = -1;
    long last = 0;
    long lightsUsed = 0;
    for (unsigned long i = 0; i < LightCount; ++i) {
        CLightInfo& light = Lights[i];
        if (!light.UpdateFlag && !light.IsEnabled) {
            if (first != -1) {
                SetVSConstants(CurrentVSConstantLayout->LightArray.Offset + first, &LightBuffer[first], last - first);
                first = -1;
            }
        } else {
            light.UpdateFlag = false;
            unsigned long lightSize = CurrentVSConstantLayout->LightSize;
            long base = lightSize * i;
            C4DVector* dst = &LightBuffer[base];
            if (first == -1)
                first = base;
            last = base + lightSize;
            lightsUsed = i + 1;
            if (light.IsEnabled) {
                unsigned long size = light.Light.Size;
                for (unsigned long j = 0; j < size; ++j)
                    dst[j] = light.Light.Parameters[j];
            } else {
                for (unsigned long j = 0; j < CurrentVSConstantLayout->LightSize; ++j)
                    dst[j] = C4DVector(0.0f, 0.0f, 0.0f, 1.0f);
            }
        }
    }
    if (first != -1)
        SetVSConstants(CurrentVSConstantLayout->LightArray.Offset + first, &LightBuffer[first], last - first);
    if (lightsUsed > 1) {
        long reg = CurrentVSConstantLayout->LightAttenuations.Offset;
        unsigned long count = (lightsUsed + 2) / 4 * 2;
        SetVSConstants(reg, LightAttenuations.GetData(), count);
    }
    UpdateFlags = (UpdateFlags & ~0x200) | 0x100;
}