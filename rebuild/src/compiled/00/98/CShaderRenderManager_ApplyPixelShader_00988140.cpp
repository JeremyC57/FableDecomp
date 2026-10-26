// CShaderRenderManager::ApplyPixelShader(const CPixelShader& shader) -- make the shader current when it differs from the current one (release the old data, share the new) and mark the pixel-shader state dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988140, 70 bytes (`?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z`, Ego_r.pdb).
// Callees: CPixelShaderData's virtual scalar deleting destructor (slot 0, `push 1`) at the last reference.
//
// Shape: `shader.Data != CurrentPixelShader.Data` (the argument loaded first: `cmp eax,ecx` in source operand
// order); the handle assignment is the inline CPixelShader::Assign (its out-of-line copy is 0x00987E50: the
// old data as a local copy, the new one re-read from the argument and not null-tested); `UpdateFlags |= 3`
// is a read-modify-write `or dword ptr [esi+3D8h],3`.
// Retail layout: CurrentPixelShader +0x1E0 and UpdateFlags +0x3D8 (Ego_r.pdb +0x1F0 / +0x3F8: the PDB
// CArrays before them are 16 bytes, the retail ones 12). No engine header exists for CShaderRenderManager.
// Leaf: ?ApplyPixelShader@CShaderRenderManager

struct IDirect3DPixelShader9;

struct CPixelShaderData {                        // Ego_r.pdb, 20 bytes
    virtual ~CPixelShaderData();                 // slot 0: the scalar deleting destructor (`push 1 ; call [eax]`)
    IDirect3DPixelShader9* PixelShader;          // +0x04
    unsigned long          ReferenceCount;       // +0x08
    unsigned long          ConstCount;           // +0x0C
    long*                  ConstIndices;         // +0x10
};

class CPixelShader { public:
    virtual unsigned long CountReferences();     // slot 0
    CPixelShaderData* Data;                      // +0x04
    void Assign(const CPixelShader& other)       // 0x00987E50 (inlined)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        ++Data->ReferenceCount;
    }
};

class CShaderRenderManager { public:
    unsigned char _pad_0x000[0x1e0];
    CPixelShader  CurrentPixelShader;            // +0x1E0
    unsigned char _pad_0x1e8[0x3d8 - 0x1e8];
    unsigned long UpdateFlags;                   // +0x3D8
    void ApplyPixelShader(const CPixelShader& shader);
};

void CShaderRenderManager::ApplyPixelShader(const CPixelShader& shader)
{
    if (shader.Data != CurrentPixelShader.Data) {
        CurrentPixelShader.Assign(shader);
        UpdateFlags |= 3;
    }
}