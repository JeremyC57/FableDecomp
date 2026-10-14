// CEngineStateBlocks::CEngineStateBlocks() -- the 12-byte primary base, this class's vtable, then the ten state-block members at +0xc..+0x30 (each a 4-byte polymorphic object: its vtable store is its whole inline constructor)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B28BC0, length 88 bytes.
// Callees: the base constructor @ 0x00B59710 (the manifest label "CNavigationLayer" is propagated; the same 12-byte base as CEnginePrimitiveTypeInfo). Vtables:
// 0x012A0ECC = CEngineStateBlocks and the ten block vtables 0x012A0E7C.. (rtti-vtables.tsv: Solid, Solid2X, Alpha, Alpha2X, AdditiveAlpha, AddSmoothAlpha,
// Alpha2XTFactor, DiffuseOnly, Diffuse2X, DiffuseEnv2X). Shape: eleven vtable stores in order, `this` returned.
struct CStateBlocksBase_00b59710 {                // 0xc bytes
    virtual void v00();
    unsigned char _pad[8];                         // +0x04 .. +0x0c
    CStateBlocksBase_00b59710();                   // 0x00B59710
};

struct CEngineStateBlock { virtual void v00(); };  // the shared block interface (no data)
struct CEngineStateBlockSolid : CEngineStateBlock { virtual void v00(); CEngineStateBlockSolid() { } };   // vtable 0x012A0E7C
struct CEngineStateBlockSolid2X : CEngineStateBlock { virtual void v00(); CEngineStateBlockSolid2X() { } };   // vtable 0x012A0E84
struct CEngineStateBlockAlpha : CEngineStateBlock { virtual void v00(); CEngineStateBlockAlpha() { } };   // vtable 0x012A0E8C
struct CEngineStateBlockAlpha2X : CEngineStateBlock { virtual void v00(); CEngineStateBlockAlpha2X() { } };   // vtable 0x012A0E94
struct CEngineStateBlockAdditiveAlpha : CEngineStateBlock { virtual void v00(); CEngineStateBlockAdditiveAlpha() { } };   // vtable 0x012A0E9C
struct CEngineStateBlockAddSmoothAlpha : CEngineStateBlock { virtual void v00(); CEngineStateBlockAddSmoothAlpha() { } };   // vtable 0x012A0EA4
struct CEngineStateBlockAlpha2XTFactor : CEngineStateBlock { virtual void v00(); CEngineStateBlockAlpha2XTFactor() { } };   // vtable 0x012A0EAC
struct CEngineStateBlockDiffuseOnly : CEngineStateBlock { virtual void v00(); CEngineStateBlockDiffuseOnly() { } };   // vtable 0x012A0EB4
struct CEngineStateBlockDiffuse2X : CEngineStateBlock { virtual void v00(); CEngineStateBlockDiffuse2X() { } };   // vtable 0x012A0EBC
struct CEngineStateBlockDiffuseEnv2X : CEngineStateBlock { virtual void v00(); CEngineStateBlockDiffuseEnv2X() { } };   // vtable 0x012A0EC4

struct CEngineStateBlocks : CStateBlocksBase_00b59710 {   // vtable 0x012A0ECC
    CEngineStateBlockSolid             m_00;    // +0x0c
    CEngineStateBlockSolid2X           m_01;    // +0x10
    CEngineStateBlockAlpha             m_02;    // +0x14
    CEngineStateBlockAlpha2X           m_03;    // +0x18
    CEngineStateBlockAdditiveAlpha     m_04;    // +0x1c
    CEngineStateBlockAddSmoothAlpha    m_05;    // +0x20
    CEngineStateBlockAlpha2XTFactor    m_06;    // +0x24
    CEngineStateBlockDiffuseOnly       m_07;    // +0x28
    CEngineStateBlockDiffuse2X         m_08;    // +0x2c
    CEngineStateBlockDiffuseEnv2X      m_09;    // +0x30
    virtual void v00();
    CEngineStateBlocks();
};

CEngineStateBlocks::CEngineStateBlocks()
    : CStateBlocksBase_00b59710()
    , m_00()
    , m_01()
    , m_02()
    , m_03()
    , m_04()
    , m_05()
    , m_06()
    , m_07()
    , m_08()
    , m_09()
{
}
