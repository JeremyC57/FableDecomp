#pragma optimize("s", on)
// CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other) -- copy constructor: the CParentDefClassBase base, this class's vtable, then the two vectors at +0x3c and +0x48 copy-constructed out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004773F1, length 51 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase(const&) @ 0x0046EE98, the first vector's copy constructor @ 0x00477428 (the manifest label "lua_stackspace" is
// propagated), the object-def vector's copy constructor @ 0x004774D6 (CEngineLocalDetailObjectDef_vector_CopyAndResize). Vtable 0x012362E4 =
// CEngineLocalDetailLayerDef (rtti-vtables.tsv). Shape: the other object stays in edi; the first member address is formed with `lea`, the second with
// `add edi,48h` after the first call (edi is dead afterwards).
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

struct CRetailLayerVector_00477428 {               // 12 bytes at +0x3c
    void* _body[3];                                // an STLport vector: three pointer words (4-aligned)
    CRetailLayerVector_00477428(const CRetailLayerVector_00477428& other);   // 0x00477428
};

struct CRetailObjectDefVector_004774d6 {           // at +0x48
    void* _body[3];                                // an STLport vector: three pointer words (4-aligned)
    CRetailObjectDefVector_004774d6(const CRetailObjectDefVector_004774d6& other);   // 0x004774D6
};

class CEngineLocalDetailLayerDef : public CParentDefClassBase { public:   // vtable 0x012362E4
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CEngineLocalDetailLayerDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CEngineLocalDetailLayerDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CEngineLocalDetailLayerDef@@UAEKXZ
    CRetailLayerVector_00477428     m_layers;      // +0x3c (Ego_r.pdb: std::vector<float> SpacingFromLayer)
    CRetailObjectDefVector_004774d6 m_objectDefs;  // +0x48 (Ego_r.pdb: std::vector<CEngineLocalDetailObjectDef> Objects)
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);
};

CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other)
    : CParentDefClassBase(other)
    , m_layers(other.m_layers)
    , m_objectDefs(other.m_objectDefs)
{
}