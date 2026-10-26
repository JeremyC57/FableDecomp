// CRenderManagerCore::SetWorldTransform(unsigned long index, const CMatrix3x4& matrix) -- load a 3x4 matrix as device world matrix `index`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B1E0, length 157 bytes (+3 int3 padding).
// PDB: ?SetWorldTransform@CRenderManagerCore@@QAEXKABVCMatrix3x4@@@Z.
// Shape: the inlined private SetD3DMatrix(D3DMATRIX&, 16 floats) builds the 4x4 on the stack (the 12 matrix floats copied
// as dwords, the 0,0,0,1 column stored after the pushes), then IDirect3DDevice9::SetTransform (slot 44, +0xB0) on
// PD3DDevice (+0x3CF0).
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

struct _D3DMATRIX {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

// CMatrix3x4: three basis rows and the translation row, 3 floats each (48 bytes).
class CMatrix3x4 { public:
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
    float _41, _42, _43;
};

struct CRenderManagerCore_Methods : CRenderManagerCore {
    // PDB: private ?SetD3DMatrix@CRenderManagerCore@@AAEXAAU_D3DMATRIX@@MMMMMMMMMMMMMMMM@Z, inlined here.
    void SetD3DMatrix(_D3DMATRIX& m, float m11, float m12, float m13, float m14,
                      float m21, float m22, float m23, float m24,
                      float m31, float m32, float m33, float m34,
                      float m41, float m42, float m43, float m44)
    {
        m._11 = m11; m._12 = m12; m._13 = m13; m._14 = m14;
        m._21 = m21; m._22 = m22; m._23 = m23; m._24 = m24;
        m._31 = m31; m._32 = m32; m._33 = m33; m._34 = m34;
        m._41 = m41; m._42 = m42; m._43 = m43; m._44 = m44;
    }
    void SetWorldTransform(unsigned long index, const CMatrix3x4& matrix);
};

void CRenderManagerCore_Methods::SetWorldTransform(unsigned long index, const CMatrix3x4& matrix)
{
    _D3DMATRIX d3dMatrix;
    SetD3DMatrix(d3dMatrix, matrix._11, matrix._12, matrix._13, 0.0f,
                            matrix._21, matrix._22, matrix._23, 0.0f,
                            matrix._31, matrix._32, matrix._33, 0.0f,
                            matrix._41, matrix._42, matrix._43, 1.0f);
    PD3DDevice->SetTransform(index + 0x100 /* D3DTS_WORLDMATRIX(index) */, &d3dMatrix);
}