// CRenderManagerCore::DrawAsIndexedTris(CTexture* const* textures, unsigned long noTextures, const void* vertices, unsigned long stride, unsigned long vertexCount, const unsigned short* indices, unsigned long triangleCount) -- attach every texture of the array to its stage, realise the render states, then draw user-memory vertices/16-bit indices as an indexed triangle list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AEA0, length 146 bytes (+14 int3 padding).
// Shape: the overload taking a CTexture array; the loop `for (i = 0; i < noTextures; ++i) AttachTextureToStage(i, textures[i])`
// is the inlined AttachTextureToStage (ebx walks CurrentlySelectedTextures +0x3D00, esi the stage, `inc esi` is the
// stage + 1 of the TextureStagesInUse grow). Then the same tail as the landed four-argument overload.
// Callees: CRenderStateManager::RealiseRenderState @ 0x00A058C0; IDirect3DDevice9::SetTexture (slot 65, +0x104);
// IDirect3DDevice9::DrawIndexedPrimitiveUP (slot 84, +0x150), D3DPT_TRIANGLELIST, D3DFMT_INDEX16 (0x65).
#include "engine/CRenderManagerCore.h"

// d3d9.h is not on the verifier include path, so the IDirect3DDevice9 vtable is declared locally
// (COM methods are __stdcall with `this` pushed; unnamed slots are placeholders).
struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual long __stdcall SetTransform(long state, const void* matrix);   // slot 44 (+0xB0)
    virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc(); virtual void __stdcall vc0();
    virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc(); virtual void __stdcall vd0();
    virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc(); virtual void __stdcall ve0();
    virtual long __stdcall SetRenderState(long state, unsigned long value);   // slot 57 (+0xE4)
    virtual void __stdcall ve8(); virtual void __stdcall vec(); virtual void __stdcall vf0(); virtual void __stdcall vf4();
    virtual void __stdcall vf8(); virtual void __stdcall vfc(); virtual void __stdcall v100();
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture);   // slot 65 (+0x104)
    virtual void __stdcall v108();
    virtual long __stdcall SetTextureStageState(unsigned long stage, long type, unsigned long value);   // slot 67 (+0x10C)
    virtual void __stdcall v110();
    virtual long __stdcall SetSamplerState(unsigned long sampler, long type, unsigned long value);   // slot 69 (+0x114)
    virtual void __stdcall v118(); virtual void __stdcall v11c(); virtual void __stdcall v120(); virtual void __stdcall v124();
    virtual void __stdcall v128(); virtual void __stdcall v12c(); virtual void __stdcall v130(); virtual void __stdcall v134();
    virtual void __stdcall v138(); virtual void __stdcall v13c(); virtual void __stdcall v140();
    virtual long __stdcall DrawPrimitive(long primitiveType, unsigned long startVertex, unsigned long primitiveCount);   // slot 81 (+0x144)
    virtual long __stdcall DrawIndexedPrimitive(long primitiveType, long baseVertexIndex, unsigned long minVertexIndex, unsigned long numVertices, unsigned long startIndex, unsigned long primitiveCount);   // slot 82 (+0x148)
    virtual long __stdcall DrawPrimitiveUP(long primitiveType, unsigned long primitiveCount, const void* vertices, unsigned long stride);   // slot 83 (+0x14C)
    virtual long __stdcall DrawIndexedPrimitiveUP(long primitiveType, unsigned long minVertexIndex, unsigned long numVertices, unsigned long primitiveCount, const void* indices, long indexFormat, const void* vertices, unsigned long stride);   // slot 84 (+0x150)
    virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c(); virtual void __stdcall v160();
    virtual void __stdcall v164(); virtual void __stdcall v168(); virtual void __stdcall v16c(); virtual void __stdcall v170();
    virtual void __stdcall v174(); virtual void __stdcall v178(); virtual void __stdcall v17c(); virtual void __stdcall v180();
    virtual void __stdcall v184(); virtual void __stdcall v188(); virtual void __stdcall v18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, void* data, unsigned long offset, unsigned long stride);   // slot 100 (+0x190)
};

// CTexture: its first word is the IDirect3DBaseTexture9* the stage binding wants (a null CTexture binds no texture).
struct CTexture {
    IDirect3DBaseTexture9* PD3DTexture;   // +0x00
};

struct CRenderManagerCore_Methods : CRenderManagerCore {
    void RealiseRenderState();      // 0x00A058C0 (CRenderStateManager, the primary base at +0)

    // CRenderManagerCore::AttachTextureToStage (out-of-line copy landed at 0x009A0CF0), inlined into the
    // texture-array overloads: rebind only a changed stage, then grow TextureStagesInUse to cover it.
    void AttachTextureToStage(long stage, const CTexture* texture)
    {
        IDirect3DBaseTexture9* deviceTexture = texture != 0 ? texture->PD3DTexture : 0;
        if (CurrentlySelectedTextures[stage] != deviceTexture)
        {
            CurrentlySelectedTextures[stage] = deviceTexture;
            PD3DDevice->SetTexture(stage, deviceTexture);
        }
        if (TextureStagesInUse < stage + 1)
            TextureStagesInUse = stage + 1;
    }
    void DrawAsIndexedTris(CTexture* const* textures, unsigned long noTextures, const void* vertices, unsigned long stride, unsigned long vertexCount, const unsigned short* indices, unsigned long triangleCount);
};

void CRenderManagerCore_Methods::DrawAsIndexedTris(CTexture* const* textures, unsigned long noTextures, const void* vertices, unsigned long stride, unsigned long vertexCount, const unsigned short* indices, unsigned long triangleCount)
{
    for (unsigned long i = 0; i < noTextures; ++i)
        AttachTextureToStage(i, textures[i]);
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitiveUP(4 /* D3DPT_TRIANGLELIST */, 0, vertexCount, triangleCount, indices, 0x65 /* D3DFMT_INDEX16 */, vertices, stride);
}
