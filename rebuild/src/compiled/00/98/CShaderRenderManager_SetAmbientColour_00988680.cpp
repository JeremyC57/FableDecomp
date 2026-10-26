// CShaderRenderManager::SetAmbientColour(const CRGBFloatColour& colour) -- store the ambient colour with its alpha forced to 1.0f and mark the ambient term dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988680, 62 bytes (`?SetAmbientColour@CShaderRenderManager@@QAEXABVCRGBFloatColour@@@Z`,
// Ego_r.pdb). Manifest label "CShaderRenderManager::SetViewTransform" is rejected: the body copies 16 bytes
// into AmbientColour (retail +0x1AC) and overwrites its A word with 1.0f (0x3F800000); a view transform is a
// 3x4 matrix into ViewMatrix. No callees.
//
// Shape: the colour copied word by word through esi (struct assignment), then the dead-store-kept
// `A = 1.0f` immediate scheduled between the load and the store of `UpdateFlags |= 4`.
// Retail layout: AmbientColour +0x1AC and UpdateFlags +0x3D8 (Ego_r.pdb +0x1BC / +0x3F8). No engine header
// exists for CShaderRenderManager.
// Leaf: ?SetAmbientColour@CShaderRenderManager

class CRGBFloatColour { public:
    float R, G, B, A;
};

class CShaderRenderManager { public:
    unsigned char   _pad_0x000[0x1ac];
    CRGBFloatColour AmbientColour;               // +0x1AC
    unsigned char   _pad_0x1bc[0x3d8 - 0x1bc];
    unsigned long   UpdateFlags;                 // +0x3D8
    void SetAmbientColour(const CRGBFloatColour& colour);
};

void CShaderRenderManager::SetAmbientColour(const CRGBFloatColour& colour)
{
    AmbientColour = colour;
    AmbientColour.A = 1.0f;
    UpdateFlags |= 4;
}