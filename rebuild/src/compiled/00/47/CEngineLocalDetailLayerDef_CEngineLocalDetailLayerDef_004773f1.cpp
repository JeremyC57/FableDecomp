#pragma optimize("s", on)
// CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other) -- copy constructor: the CParentDefClassBase base, this class's vtable, then the two vectors at +0x3c and +0x48 copy-constructed out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004773F1, length 51 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase(const&) @ 0x0046EE98, the first vector's copy constructor @ 0x00477428 (the manifest label "lua_stackspace" is
// propagated), the object-def vector's copy constructor @ 0x004774D6 (CEngineLocalDetailObjectDef_vector_CopyAndResize). Vtable 0x012362E4 =
// CEngineLocalDetailLayerDef (rtti-vtables.tsv). Shape: the other object stays in edi; the first member address is formed with `lea`, the second with
// `add edi,48h` after the first call (edi is dead afterwards).
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _pad[0x38];                      // +0x04 .. +0x3c
    CParentDefClassBase(const CParentDefClassBase& other);   // 0x0046EE98
};

struct CRetailLayerVector_00477428 {               // 12 bytes at +0x3c
    unsigned char _body[0xc];
    CRetailLayerVector_00477428(const CRetailLayerVector_00477428& other);   // 0x00477428
};

struct CRetailObjectDefVector_004774d6 {           // at +0x48
    unsigned char _body[0xc];
    CRetailObjectDefVector_004774d6(const CRetailObjectDefVector_004774d6& other);   // 0x004774D6
};

struct CEngineLocalDetailLayerDef : CParentDefClassBase {   // vtable 0x012362E4
    CRetailLayerVector_00477428     m_layers;      // +0x3c
    CRetailObjectDefVector_004774d6 m_objectDefs;  // +0x48
    virtual void v00();
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);
};

CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other)
    : CParentDefClassBase(other)
    , m_layers(other.m_layers)
    , m_objectDefs(other.m_objectDefs)
{
}