// NUISystem::CMesh::CMesh(void* def) -- the CDraggableInto base (0x0052CC50 with the def), the three vtables of this class (0x01249F9C at +0, 0x01249F74 at +4, 0x01249F6C at +0x18), the CBase-derived record at +0x164 constructed (CBaseClass @ 0x0099A2D0, its vtable 0x0122F598, its +4 cleared), the byte +0x1a0 cleared, then GetDataFromDef (0x005500B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00550190, length 78 bytes.
// Callees: CDraggableInto::CDraggableInto @ 0x0052CC50, CBaseClass::CBaseClass @ 0x0099A2D0, 0x005500B0 (labelled CScrollingViewport::GetDataFromDef). Shape: `this` in esi, the record in edi, one zero register, `ret 4`.
struct IViewportPrimary_00550190 {                    // the primary base (vptr at +0)
    virtual void v00();
};
struct IViewportSecondaryA_00550190 {                 // the secondary base at +4
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct IViewportSecondaryB_00550190 {                 // the secondary base at +0x18
    virtual void b00();
};
struct CDraggableInto : IViewportPrimary_00550190, IViewportSecondaryA_00550190, IViewportSecondaryB_00550190 {   // 0x164 bytes
    unsigned char _pad1c[0x164 - 0x1c];
    CDraggableInto(void* def);                     // 0x0052CC50
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct SRecordBody_00550190 { long m_4; SRecordBody_00550190() : m_4(0) { } };
struct CBatchRecord_0122f598 : CBaseClass {             // vtable 0x0122F598
    virtual void v00();
    SRecordBody_00550190 m_body;                   // +0x04: an embedded object with an inline constructor (its store follows the vtable store)
    CBatchRecord_0122f598() : CBaseClass(), m_body() { }
};

namespace NUISystem {
class CMesh : public CDraggableInto { public:                    // vtables 0x01249F9C / 0x01249F74 / 0x01249F6C
    virtual void v00();
    virtual void a00();
    virtual void b00();
    CBatchRecord_0122f598 m_record;                // +0x164
    unsigned char         _pad16c[0x34];
    bool                  m_1a0;                   // +0x1a0
    CMesh(void* def);
    void GetDataFromDef();                         // 0x005500B0
};
}

NUISystem::CMesh::CMesh(void* def)
    : CDraggableInto(def), m_record(), m_1a0(false)
{
    GetDataFromDef();
}