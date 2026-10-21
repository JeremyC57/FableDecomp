// CShaderRenderManager::PrepareForRender(unsigned long renderFlags) -- before a draw, push every piece of shader state whose dirty bit is set (vertex-shader constants, pixel shader, transforms, fog, lights, ambient...), steered by the draw's render flags
// Leaf: PrepareForRender
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1280, length 411 bytes (`ret 4`).
// Callees: the CShaderRenderManager updaters named in the model (manifest labels; 0x0098A8E0 is propagated from
// C3DMeshFileXUVTangentChunk::WriteToFile and 0x00989700 duplicates 0x00989760's UpdateAmbient, so both get neutral
// names after the dirty bit that guards them).
// Layout: engine has no CShaderRenderManager header (the PDB entry is opaque padding); local model: shader state
// pointer +0x04 (its VS constant base +0x08 and a 12-byte STLport vector of 16-byte constants +0x140), the world
// transform identity flag +0x1E8, dirty flags +0x3D8.
// Shape: one `if` per dirty bit in retail order, with if/else pairs for the world/object-space texture, fog and
// lighting variants; the constant upload passes `size() > 0 ? _M_start : (T*)&vector` and size() as an unsigned
// count (a signed size gives `test edx,0FFFFFFF0h ; jg` instead of `sar ; test ; ja`). The low-byte, `ah` and dword
// flag tests are VC's own choices for the masks.
struct CVector4 { float X, Y, Z, W; };
struct CConstantVector {                           // STLport vector<16-byte constant>
    CVector4* _M_start;
    CVector4* _M_finish;
    CVector4* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CVector4* data() { return size() > 0 ? _M_start : (CVector4*)this; }
};
struct CShaderState {
    unsigned char   _pad00[8];
    long            VSConstantBase;                // +0x08
    unsigned char   _pad0c[0x134];
    CConstantVector VSConstants;                   // +0x140
};
struct CShaderRenderManager {
    unsigned char _pad000[4];
    CShaderState* State;                           // +0x04
    unsigned char _pad008[0x1e0];
    bool          WorldTransformIsIdentity;        // +0x1e8
    unsigned char _pad1e9[0x1ef];
    unsigned long DirtyFlags;                      // +0x3d8
    void SetVSConstants(long base, void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                      // 0x00988A20
    void UpdatePixelShaderConstants();             // 0x00989BF0
    void UpdateFlag20State();                      // 0x0098A8E0 (manifest label C3DMeshFileXUVTangentChunk::WriteToFile, propagated)
    void UpdateWorldSpaceTextureTransform();       // 0x009894B0
    void UpdateObjectSpaceTextureTransform();      // 0x00989230
    void UpdateWorldSpaceFogTransform();           // 0x00989640
    void UpdateObjectSpaceFogTransform();          // 0x00989540
    void UpdateAmbient();                          // 0x00989760
    void UpdateFogColour();                        // 0x009897C0
    void UpdateLightGlobals();                     // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long renderFlags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();               // 0x0098A540
    void UpdateLocalisedSphereMapping();           // 0x00988E80
    void ResetWorldTransform();                    // 0x00988290
    void UpdateCombinedProjectionTransform();      // 0x00988A50
    void UpdateFlag8State();                       // 0x00989700 (manifest label UpdateAmbient, duplicated with 0x00989760)
    void PrepareForRender(unsigned long renderFlags);
};

void CShaderRenderManager::PrepareForRender(unsigned long renderFlags)
{
    if (DirtyFlags & 4)
        SetVSConstants(State->VSConstantBase, State->VSConstants.data(), State->VSConstants.size());
    if (DirtyFlags & 1)
        UpdatePixelShader();
    if (DirtyFlags & 2)
        UpdatePixelShaderConstants();
    if ((renderFlags & 0x400) && (DirtyFlags & 0x20))
        UpdateFlag20State();
    if (renderFlags & 0x40) {
        if (DirtyFlags & 0x1000)
            UpdateWorldSpaceTextureTransform();
    } else if ((renderFlags & 0x80) && (DirtyFlags & 0x800))
        UpdateObjectSpaceTextureTransform();
    if (renderFlags & 0x200) {
        if (DirtyFlags & 0x2000)
            UpdateWorldSpaceFogTransform();
    } else if ((renderFlags & 0x100) && (DirtyFlags & 0x4000))
        UpdateObjectSpaceFogTransform();
    if (DirtyFlags & 4)
        UpdateAmbient();
    if (DirtyFlags & 0x20000)
        UpdateFogColour();
    if (DirtyFlags & 0x400)
        UpdateLightGlobals();
    if (renderFlags & 8) {
        if (DirtyFlags & 0x100)
            UpdateObjectSpaceLighting(renderFlags);
    } else if ((renderFlags & 0x10) && (DirtyFlags & 0x200))
        UpdateWorldSpaceLighting();
    if ((renderFlags & 0x20) && (DirtyFlags & 0x8000))
        UpdateLocalisedSphereMapping();
    if (!(renderFlags & 1) && !WorldTransformIsIdentity)
        ResetWorldTransform();
    if (DirtyFlags & 0x80)
        UpdateCombinedProjectionTransform();
    if ((renderFlags & 4) && (DirtyFlags & 8))
        UpdateFlag8State();
}