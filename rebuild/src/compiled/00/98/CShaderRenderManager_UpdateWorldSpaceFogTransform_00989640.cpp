// CShaderRenderManager::UpdateWorldSpaceFogTransform() -- upload FogTransform as one float4 vertex-shader constant at the layout's fog register and move the fog dirty bit from 0x2000 to 0x4000
// Leaf: UpdateWorldSpaceFogTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00989640, length 112 bytes (105 + int3 padding).
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, +0x178).
// Shape: FogTransform copied into a C4DVector frame local through its inline memberwise copy ctor (direct
// member loads scheduled among the pushes; the implicit copy re-bases the source with `lea`), the register
// read from the current layout's FogTransform range (+0x84 = Offset), then a load / and / or / store of
// UpdateFlags (a plain member expression, not the memory-form `or`).
// Layout: CurrentVSConstantLayout +4 (engine/CVertexShaderConstantLayout.h), D3DDevice +0x1d4, FogTransform
// +0x370, UpdateFlags +0x3d8 (PDB +0x1e4 / +0x380 / +0x3f8; retail's 12-byte CArrays put them lower).
#include "engine/CVertexShaderConstantLayout.h"

class CShaderConstantRange { public:                    // the 12-byte ranges of engine/CVertexShaderConstantLayout.h
    void*         __vftable;                     // +0
    long          Offset;                        // +4: first constant register
    unsigned long Count;                         // +8: vector count
};

class C4DVector { public:
    float X, Y, Z, W;
    C4DVector(const C4DVector& o) : X(o.X), Y(o.Y), Z(o.Z), W(o.W) {}
};

struct IDirect3DDevice9 {
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual void __stdcall v02();
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual void __stdcall v06();
    virtual void __stdcall v07();
    virtual void __stdcall v08();
    virtual void __stdcall v09();
    virtual void __stdcall v10();
    virtual void __stdcall v11();
    virtual void __stdcall v12();
    virtual void __stdcall v13();
    virtual void __stdcall v14();
    virtual void __stdcall v15();
    virtual void __stdcall v16();
    virtual void __stdcall v17();
    virtual void __stdcall v18();
    virtual void __stdcall v19();
    virtual void __stdcall v20();
    virtual void __stdcall v21();
    virtual void __stdcall v22();
    virtual void __stdcall v23();
    virtual void __stdcall v24();
    virtual void __stdcall v25();
    virtual void __stdcall v26();
    virtual void __stdcall v27();
    virtual void __stdcall v28();
    virtual void __stdcall v29();
    virtual void __stdcall v30();
    virtual void __stdcall v31();
    virtual void __stdcall v32();
    virtual void __stdcall v33();
    virtual void __stdcall v34();
    virtual void __stdcall v35();
    virtual void __stdcall v36();
    virtual void __stdcall v37();
    virtual void __stdcall v38();
    virtual void __stdcall v39();
    virtual void __stdcall v40();
    virtual void __stdcall v41();
    virtual void __stdcall v42();
    virtual void __stdcall v43();
    virtual void __stdcall v44();
    virtual void __stdcall v45();
    virtual void __stdcall v46();
    virtual void __stdcall v47();
    virtual void __stdcall v48();
    virtual void __stdcall v49();
    virtual void __stdcall v50();
    virtual void __stdcall v51();
    virtual void __stdcall v52();
    virtual void __stdcall v53();
    virtual void __stdcall v54();
    virtual void __stdcall v55();
    virtual void __stdcall v56();
    virtual void __stdcall v57();
    virtual void __stdcall v58();
    virtual void __stdcall v59();
    virtual void __stdcall v60();
    virtual void __stdcall v61();
    virtual void __stdcall v62();
    virtual void __stdcall v63();
    virtual void __stdcall v64();
    virtual void __stdcall v65();
    virtual void __stdcall v66();
    virtual void __stdcall v67();
    virtual void __stdcall v68();
    virtual void __stdcall v69();
    virtual void __stdcall v70();
    virtual void __stdcall v71();
    virtual void __stdcall v72();
    virtual void __stdcall v73();
    virtual void __stdcall v74();
    virtual void __stdcall v75();
    virtual void __stdcall v76();
    virtual void __stdcall v77();
    virtual void __stdcall v78();
    virtual void __stdcall v79();
    virtual void __stdcall v80();
    virtual void __stdcall v81();
    virtual void __stdcall v82();
    virtual void __stdcall v83();
    virtual void __stdcall v84();
    virtual void __stdcall v85();
    virtual void __stdcall v86();
    virtual void __stdcall v87();
    virtual void __stdcall v88();
    virtual void __stdcall v89();
    virtual void __stdcall v90();
    virtual void __stdcall v91();
    virtual void __stdcall v92();
    virtual void __stdcall v93();
    virtual long __stdcall SetVertexShaderConstantF(unsigned int startRegister, const float* data, unsigned int vector4fCount);   // slot 94 (+0x178)
};

class CShaderRenderManager { public:
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char                _pad008[0x1d4 - 8];
    IDirect3DDevice9*            D3DDevice;                 // +0x1d4
    unsigned char                _pad1d8[0x370 - 0x1d8];
    C4DVector                    FogTransform;              // +0x370
    unsigned char                _pad380[0x3d8 - 0x380];
    unsigned int                 UpdateFlags;               // +0x3d8
    void UpdateWorldSpaceFogTransform();
};

void CShaderRenderManager::UpdateWorldSpaceFogTransform()
{
    C4DVector fog = FogTransform;
    D3DDevice->SetVertexShaderConstantF(
        ((const CShaderConstantRange*)CurrentVSConstantLayout->FogTransform)->Offset, &fog.X, 1);
    UpdateFlags = (UpdateFlags & ~0x2000u) | 0x4000u;
}