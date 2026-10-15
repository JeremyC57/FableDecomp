// CPixelShader::Assign(const CPixelShader& other) -- share the other pixel shader's data: release ours (delete at the last reference) and null it, then adopt theirs and add a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x00987E50, 45 bytes. Manifest label "CShaderRenderManager::ApplyPixelShader" is
// propagated and rejected: `this` is a CPixelShader handle (vptr + Data at +4, PDB 8 bytes), not the render
// manager; the same sequence is inlined into the real CShaderRenderManager::ApplyPixelShader (0x00988140,
// on CurrentPixelShader). The body returns nothing (eax is the adopted data, not `this`), so it is not
// written as operator=; the PDB name of this out-of-line copy is not known. No direct caller references it.
// Callees: CPixelShaderData's virtual scalar deleting destructor (slot 0, `push 1`).
//
// Shape: the old data pointer is a LOCAL copy (`mov ecx,[esi+4] ; dec [ecx+8] ; jne ; mov eax,[ecx] ; push 1 ;
// call [eax]` -- the member form re-reads Data after the decrement); `delete` of the polymorphic object folds
// its own null test into the enclosing `if`; the new data is not null-tested before `inc [eax+8]`.
// Layouts = Ego_r.pdb: CPixelShader {vptr (CountReferences), CPixelShaderData* Data +4}; CPixelShaderData
// {vptr, IDirect3DPixelShader9* PixelShader +4, ReferenceCount +8, ConstCount +0xC, ConstIndices +0x10}.
// Leaf: ?Assign@CPixelShader

struct IDirect3DPixelShader9;

struct CPixelShaderData {                        // Ego_r.pdb, 20 bytes
    virtual ~CPixelShaderData();                 // slot 0: the scalar deleting destructor (`push 1 ; call [eax]`)
    IDirect3DPixelShader9* PixelShader;          // +0x04
    unsigned long          ReferenceCount;       // +0x08
    unsigned long          ConstCount;           // +0x0C
    long*                  ConstIndices;         // +0x10
};

struct CPixelShader {
    virtual unsigned long CountReferences();     // slot 0
    CPixelShaderData* Data;                      // +0x04
    void Assign(const CPixelShader& other);
};

void CPixelShader::Assign(const CPixelShader& other)
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
