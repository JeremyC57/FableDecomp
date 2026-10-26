// CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CEngine*) -- particle-group primitive manager: CEnginePrimitiveManager(engine), four state bytes cleared, then the "enabled" byte (+0x21) set
// Leaf: ??0CEnginePrimitiveManagerParticleGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBF40, length 43 bytes (+5 int3 padding).
// Callee: CEnginePrimitiveManager::CEnginePrimitiveManager @ 0x00BACDB0 (one pointer argument). Vtable 0x012A5BB8 =
// CEnginePrimitiveManagerParticleGroup (rtti-vtables.tsv).
// Label correction: the manifest "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is propagated.
// Shape: speed TU; the four cleared bytes (+0x20, +0x22, +0x23, +0x24, one `xor al,al`) are initialisers scheduled
// BEFORE the vtable store; the +0x21 byte stored after the vtable store is a body assignment.
class CEngine;

class CEnginePrimitiveManager { public:
    virtual void v00();
    unsigned char _body[0x20 - 4];
    CEnginePrimitiveManager(CEngine* engine);   // 0x00BACDB0
};

class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager { public:
    bool ClipEffectToWeatherMask;                           // +0x20
    bool m_enabled;                          // +0x21
    bool WriteZBuffer;                           // +0x22
    bool DisplayBoundingBox;                           // +0x23
    bool DisplayBoundingSphere;                           // +0x24
    virtual void v00();
    CEnginePrimitiveManagerParticleGroup(CEngine* engine);
};

CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CEngine* engine)
    : CEnginePrimitiveManager(engine)
    , ClipEffectToWeatherMask(false)
    , WriteZBuffer(false)
    , DisplayBoundingBox(false)
    , DisplayBoundingSphere(false)
{
    m_enabled = true;
}