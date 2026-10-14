// CEngineSubPrimitiveText::CEngineSubPrimitiveText(void* primitive) -- the inlined CEngineSubPrimitiveBase (CBase, the primitive at +0xc, the cleared link at +0x10), this class's vtable, then the text string at +0x2c default-constructed out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7A50, length 40 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString() @ 0x0099AED0 (on +0x2c). Vtable 0x012A6858 = CEngineSubPrimitiveText (rtti-vtables.tsv). Shape:
// the base's stores precede the vtable (its own vtable store is absent: `__declspec(novtable)` on the base here), the string's `lea ecx` is scheduled early.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {   // vtable 0x012A3E7C (stored only by its own constructor @ 0x00B98A50)
    unsigned char _pad04[8];                       // +0x04 .. +0x0c
    void*         m_pPrimitive;                    // +0x0c
    long          m_pNext;                         // +0x10
    CEngineSubPrimitiveBase(void* primitive) : CBase(), m_pPrimitive(primitive), m_pNext(0) { }
};

struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099AED0
};

struct CEngineSubPrimitiveText : CEngineSubPrimitiveBase {   // vtable 0x012A6858
    unsigned char _pad14[0x18];                    // +0x14 .. +0x2c
    CCharString   m_text;                          // +0x2c
    virtual void v00();
    CEngineSubPrimitiveText(void* primitive);
};

CEngineSubPrimitiveText::CEngineSubPrimitiveText(void* primitive)
    : CEngineSubPrimitiveBase(primitive)
    , m_text()
{
}