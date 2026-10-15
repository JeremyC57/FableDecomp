// CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CEngine*) -- particle-group primitive manager: CEnginePrimitiveManager(engine), four state bytes cleared, then the "enabled" byte (+0x21) set
// Leaf: ??0CEnginePrimitiveManagerParticleGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBF40, length 43 bytes (+5 int3 padding).
// Callee: CEnginePrimitiveManager::CEnginePrimitiveManager @ 0x00BACDB0 (one pointer argument). Vtable 0x012A5BB8 =
// CEnginePrimitiveManagerParticleGroup (rtti-vtables.tsv).
// Label correction: the manifest "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is propagated.
// Shape: speed TU; the four cleared bytes (+0x20, +0x22, +0x23, +0x24, one `xor al,al`) are initialisers scheduled
// BEFORE the vtable store; the +0x21 byte stored after the vtable store is a body assignment.
struct CEngine;

struct CEnginePrimitiveManager {
    virtual void v00();
    unsigned char _body[0x20 - 4];
    CEnginePrimitiveManager(CEngine* engine);   // 0x00BACDB0
};

struct CEnginePrimitiveManagerParticleGroup : CEnginePrimitiveManager {
    bool m_flag20;                           // +0x20
    bool m_enabled;                          // +0x21
    bool m_flag22;                           // +0x22
    bool m_flag23;                           // +0x23
    bool m_flag24;                           // +0x24
    virtual void v00();
    CEnginePrimitiveManagerParticleGroup(CEngine* engine);
};

CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CEngine* engine)
    : CEnginePrimitiveManager(engine)
    , m_flag20(false)
    , m_flag22(false)
    , m_flag23(false)
    , m_flag24(false)
{
    m_enabled = true;
}