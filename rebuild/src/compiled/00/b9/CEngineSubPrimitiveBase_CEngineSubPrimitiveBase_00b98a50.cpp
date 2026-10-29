// CEngineSubPrimitiveBase::CEngineSubPrimitiveBase(void* primitive) -- CBase, the primitive at +0xc, this class's vtable, the cleared link at +0x10
// Leaf: ??0CEngineSubPrimitiveBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B98A50, length 34 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x012A3E7C = CEngineSubPrimitiveBase (rtti-vtables.tsv). Shape: the primitive store precedes the vtable, the cleared
// word follows it (an immediate); `this` is returned.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable { public:           // vtable 0x012A3E7C
    unsigned char _pad04[8];                       // +0x04 .. +0x0c
    void*         m_pPrimitive;                    // +0x0c
    long          m_pNext;                         // +0x10
    CEngineSubPrimitiveBase(void* primitive);
};

CEngineSubPrimitiveBase::CEngineSubPrimitiveBase(void* primitive)
    : CBaseClassNonCopyable()
    , m_pPrimitive(primitive)
    , m_pNext(0)
{
}