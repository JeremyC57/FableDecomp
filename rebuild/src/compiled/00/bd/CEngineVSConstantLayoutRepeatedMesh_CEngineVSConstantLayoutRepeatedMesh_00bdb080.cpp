// CEngineVSConstantLayoutRepeatedMesh::CEngineVSConstantLayoutRepeatedMesh() -- constructor over CVertexShaderConstantLayout: own vtable, four empty CShaderConstantRange members, then the lighting-result, object-matrix/offset and user register ranges assigned from temporaries
// Leaf: ??0CEngineVSConstantLayoutRepeatedMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDB080, length 384 bytes.
// Callees: CVertexShaderConstantLayout::CVertexShaderConstantLayout @ 0x00BDBB70, CBaseClass::CBaseClass @ 0x0099A2D0
//   (members and temporaries), CBaseClass::~CBaseClass @ 0x0099A2E0 (temporaries).
// Identity: vtable 0x012A7CE4 = CEngineVSConstantLayoutRepeatedMesh, 0x0129A04C = CShaderConstantRange (rtti-vtables.tsv);
//   same range model as the landed sibling constructors 0x00BDAFF0 / 0x00BDB260 and 0x00BDB400 (this chunk).
// Layout: Ego_r.pdb CEngineVSConstantLayoutRepeatedMesh (0x180) puts LightingResults at +0x150; retail builds the four
//   ranges at +0x14c/+0x158/+0x164/+0x170 (sizeof 0x17c), so the retail CVertexShaderConstantLayout base ends at 0x14c.
//   User (+0x10) is the base's user range. engine/CEngineVSConstantLayoutRepeatedMesh.h is a flattened pad: modelled locally.
// Shape: the default range constructor zeroes both words after the base ctor; each assignment builds the stack temporary,
//   copies its two words and runs the implicit derived destructor (out-of-line base dtor only).
class CBaseClass { public:
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CShaderConstantRange : public CBaseClass { public:         // vtable 0x0129A04C: a register range {first register, register count}
    long m_firstRegister;                          // +0x04
    long m_registerCount;                          // +0x08
    virtual void v00();
    CShaderConstantRange() : CBaseClass(), m_firstRegister(0), m_registerCount(0) { }
    CShaderConstantRange(long firstRegister, long registerCount) : CBaseClass(), m_firstRegister(firstRegister), m_registerCount(registerCount) { }
};

namespace N_00bdbb70 { struct CVertexShaderConstantLayout {               // retail 0x14c bytes
    virtual void v00();
    unsigned char        _pad04[0xc];              // +0x04 Reserved
    CShaderConstantRange User;                     // +0x10
    unsigned char        _pad1c[0x14c - 0x1c];     // +0x1c the remaining ranges
    CVertexShaderConstantLayout();                 // 0x00BDBB70
}; } using N_00bdbb70::CVertexShaderConstantLayout;

class CEngineVSConstantLayoutRepeatedMesh : public CVertexShaderConstantLayout { public:   // vtable 0x012A7CE4
    CShaderConstantRange LightingResults;          // +0x14c
    CShaderConstantRange MainLightLightingResults; // +0x158
    CShaderConstantRange ObjectMatricies;          // +0x164
    CShaderConstantRange ObjectOffsets;            // +0x170
    virtual void v00();
    CEngineVSConstantLayoutRepeatedMesh();
};

CEngineVSConstantLayoutRepeatedMesh::CEngineVSConstantLayoutRepeatedMesh()
    : CVertexShaderConstantLayout()
{
    LightingResults = CShaderConstantRange(0x33, 0x10);
    MainLightLightingResults = CShaderConstantRange(0x43, 0x10);
    ObjectMatricies = CShaderConstantRange(0x13, 0x10);
    ObjectOffsets = CShaderConstantRange(0x23, 0x10);
    User = CShaderConstantRange(0x53, 0xd);
}