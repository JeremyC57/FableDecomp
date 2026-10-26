// CEngineInternalAttachedPrimitiveInfo::CEngineInternalAttachedPrimitiveInfo() -- base, this vtable, an empty primitive handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3AAE0, length 43 bytes (+ int3 padding).
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0 twice (on this, then on the handle member at +0x4).
// Vtables: 0x012A1AB8 CEngineInternalAttachedPrimitiveInfo, 0x0122F598 CEnginePrimitiveHandle
// (rtti-vtables.tsv). The manifest's "CAIStateGroup_SummonerCharge" label is propagated.
// Shape: the member CEnginePrimitiveHandle (vptr + a primitive pointer zeroed by its inline
// constructor) sits at +0x4; the member's address is formed into edi before this class's vtable
// store, the handle's base call, vtable and null store follow in init order.
class CBaseClass { public:
    virtual ~CBaseClass();
    CBaseClass();                               // 0x0099A2D0
};

class CEnginePrimitiveHandle : public CBaseClass { public:
    virtual void Release();                     // own vtable 0x0122F598
    void* m_pPrimitive;                         // +0x04
    CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) {}
};

class CEngineInternalAttachedPrimitiveInfo : public CBaseClass { public:
    virtual void Release();                     // own vtable 0x012A1AB8
    CEnginePrimitiveHandle Handle;              // +0x04
    CEngineInternalAttachedPrimitiveInfo();
};

CEngineInternalAttachedPrimitiveInfo::CEngineInternalAttachedPrimitiveInfo()
    : CBaseClass(), Handle()
{
}