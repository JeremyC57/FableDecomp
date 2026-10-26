// CShaderRenderManager::UpdateBoneMatrices() -- uploads the whole pre-transposed bone-matrix array as vertex-shader constants (three float4 registers per bone) at the current layout's bone range, then clears the bone-dirty flag (0x20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098A8E0, length 139 bytes.
// Leaf: UpdateBoneMatrices
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, vtable +0x178, `__stdcall`, `this` pushed).
// Label: the manifest's "C3DMeshFileXUVTangentChunk::WriteToFile" is a propagated label; every caller is a
// CShaderRenderManager render path (PrepareForRender 0x009E12FA, the water renderers 0x00B75953..) that tests
// UpdateFlags & 0x20 first, and the body sits between SetBoneTransform (0x0098A7F0) and CreatePixelShader.
// Layout (PDB CShaderRenderManager, retail 0x20 lower at the tail because retail CArray is three pointers):
// CurrentVSConstantLayout +0x04, D3DDevice +0x1d4, UpdateFlags +0x3d8, BoneMatrices +0x3dc (48-byte
// CPreTransposedBoneMatrix: the count is `imul 0x2AAAAAAB ; sar edx,3`). The layout's BoneMatrices range
// sits at +0xd4 of CVertexShaderConstantLayout (PDB), its Offset at +0xd8.
// Shape: three separate Size() evaluations (the guard, the data select, the vector count) and the data
// pointer through ONE inline GetData() (`count ? first : this`, the retail CArray idiom); the device in a
// named local taken first inside the if (retail loads it into edi before the select branch).
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[94];                                                       // IUnknown + device slots 0..93
    long (__stdcall *SetVertexShaderConstantF)(IDirect3DDevice9* self, unsigned long startRegister,
                                               const float* data, unsigned long vector4fCount);   // slot 94 (+0x178)
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };

class CShaderConstantRange { public:                   // 12 bytes (PDB)
    void*         __vftable;                    // +0
    long          Offset;                       // +4: first constant register
    unsigned long Count;                        // +8: vector count
};
class CVertexShaderConstantLayout { public:            // PDB layout up to the bone range
    unsigned char        _pad000[0xd4];
    CShaderConstantRange BoneMatrices;          // +0xd4
};

struct CPreTransposedBoneMatrix { float Row[3][4]; };   // 48 bytes: three float4 constant registers

template<class T> class CArray { public:               // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetData() const { return Size() != 0 ? m_begin : (const T*)this; }
};

class CShaderRenderManager { public:
    void*                            __vftable;                 // +0x000
    CVertexShaderConstantLayout*     CurrentVSConstantLayout;   // +0x004
    unsigned char                    _pad008[0x1cc];
    IDirect3DDevice9*                D3DDevice;                 // +0x1d4
    unsigned char                    _pad1d8[0x200];
    unsigned long                    UpdateFlags;               // +0x3d8 (PDB +0x3f8)
    CArray<CPreTransposedBoneMatrix> BoneMatrices;              // +0x3dc (PDB +0x3fc)

    void UpdateBoneMatrices();
};

void CShaderRenderManager::UpdateBoneMatrices()
{
    if (BoneMatrices.Size() != 0) {
        IDirect3DDevice9* device = D3DDevice;
        device->lpVtbl->SetVertexShaderConstantF(device, CurrentVSConstantLayout->BoneMatrices.Offset,
                                                 BoneMatrices.GetData()->Row[0], BoneMatrices.Size() * 3);
    }
    UpdateFlags &= ~0x20;
}