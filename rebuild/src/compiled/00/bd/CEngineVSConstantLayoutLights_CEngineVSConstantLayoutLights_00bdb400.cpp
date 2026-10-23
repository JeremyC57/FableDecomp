// CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights() -- constructor over CVertexShaderConstantLayout: own vtable, then the spotlight, light-array, light-globals and user register ranges assigned from CShaderConstantRange temporaries and LightSize = 2
// Leaf: ??0CEngineVSConstantLayoutLights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDB400, length 384 bytes (with int3 padding; body 0x172).
// Callees: CVertexShaderConstantLayout::CVertexShaderConstantLayout @ 0x00BDBB70 (manifest "CVertexShaderConstantLayout_Initialize"),
//   CBaseClass::CBaseClass @ 0x0099A2D0 and CBaseClass::~CBaseClass @ 0x0099A2E0 (once per temporary).
// Identity: vtable 0x012A7D24 = CEngineVSConstantLayoutLights; the temporaries' vtable 0x0129A04C = CShaderConstantRange
//   (rtti-vtables.tsv). The landed CEngineVSConstantLayoutLightsBones (0x00BDB260) and CEngineVSConstantLayoutParticles
//   (0x00BDAFF0) constructors call this one and use the same range model.
// Layout: Ego_r.pdb CVertexShaderConstantLayout (0x150 bytes, retail offsets equal): User +0x10, LightSize +0x4c,
//   LightArray +0x50, LightGlobals +0x5c, LightAttenuations +0x68, ShadowedSpotlightFalloffTransform +0x104,
//   ShadowedSpotlightAttenuation +0x110, ShadowedSpotlightColour +0x11c (CShaderConstantRange = CBaseClass vptr +
//   {first register, register count}). engine/CEngineVSConstantLayoutLights.h is a flattened 0x150-byte pad, so the
//   members are modelled locally.
// Shape: each `Range = CShaderConstantRange(first, count);` builds the 12-byte stack temporary, copies its two words
//   (interleaved with the temporary's stores), then runs the implicit derived destructor (the out-of-line base dtor only).
struct CBaseClass {
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

struct CShaderConstantRange : CBaseClass {         // vtable 0x0129A04C: a register range {first register, register count}
    long m_firstRegister;                          // +0x04
    long m_registerCount;                          // +0x08
    virtual void v00();
    CShaderConstantRange(long firstRegister, long registerCount) : CBaseClass(), m_firstRegister(firstRegister), m_registerCount(registerCount) { }
};

namespace N_00bdbb70 { struct CVertexShaderConstantLayout {               // 0x150 bytes
    virtual void v00();
    unsigned char        _pad04[0xc];                          // +0x04 Reserved
    CShaderConstantRange User;                                 // +0x10
    unsigned char        _pad1c[0x4c - 0x1c];                  // +0x1c Compression, Ambient, FogColour, CameraPos
    long                 LightSize;                            // +0x4c
    CShaderConstantRange LightArray;                           // +0x50
    CShaderConstantRange LightGlobals;                         // +0x5c
    CShaderConstantRange LightAttenuations;                    // +0x68
    unsigned char        _pad74[0x104 - 0x74];                 // +0x74 matrices, texture transforms, bones, shadow transforms
    CShaderConstantRange ShadowedSpotlightFalloffTransform;    // +0x104
    CShaderConstantRange ShadowedSpotlightAttenuation;         // +0x110
    CShaderConstantRange ShadowedSpotlightColour;              // +0x11c
    unsigned char        _pad128[0x150 - 0x128];               // +0x128 screen-space transforms, ReservedPresets
    CVertexShaderConstantLayout();                             // 0x00BDBB70
}; } using N_00bdbb70::CVertexShaderConstantLayout;

struct CEngineVSConstantLayoutLights : CVertexShaderConstantLayout {   // vtable 0x012A7D24
    virtual void v00();
    CEngineVSConstantLayoutLights();
};

CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights()
    : CVertexShaderConstantLayout()
{
    ShadowedSpotlightFalloffTransform = CShaderConstantRange(0xf, 3);
    ShadowedSpotlightAttenuation = CShaderConstantRange(0x24, 1);
    ShadowedSpotlightColour = CShaderConstantRange(0x25, 1);
    LightArray = CShaderConstantRange(0x13, 0xc);
    LightAttenuations = CShaderConstantRange(0x1f, 4);
    LightGlobals = CShaderConstantRange(0x23, 1);
    LightSize = 2;
    User = CShaderConstantRange(0x26, 0x3a);
}