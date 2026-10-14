#pragma optimize("s", on)
// CActionParamCCountedPointer::CActionParamCCountedPointer(const CCountedPointer<CBankFileAsyncEntry>&) -- constructor: type tag 9 at +4, own vtable, the counted pointer copy-constructed at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004854DF, length 34 bytes.
// Callees: CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer&) @ 0x00428928.
// Pragma: #pragma optimize("s", on) -- the argument is pushed straight from its stack slot
// (`push dword ptr [esp+8]`), the size-optimised pass-through shape.
// Vtable 0x01236794 = CActionParamCCountedPointer (rtti-vtables.tsv); the PDB CActionParam has
// EActionParamType ActionParamType at +4 (9 = the counted-pointer parameter type) and the derived
// value follows at +8. The vtable store sinks to just before the member constructor call.
struct CCountedPointer {                                         // CCountedPointer<CBankFileAsyncEntry>
    void* m_pObject;
    void* m_pInfo;
    CCountedPointer(const CCountedPointer& other);               // 0x00428928
};

struct CActionParam {
    virtual void v00();
    long ActionParamType;                                        // +0x04 EActionParamType
    CActionParam(long type) : ActionParamType(type) { }
};

struct CActionParamCCountedPointer : CActionParam {              // vtable 0x01236794
    CCountedPointer Value;                                       // +0x08
    virtual void v00();
    CActionParamCCountedPointer(const CCountedPointer& value);
};

CActionParamCCountedPointer::CActionParamCCountedPointer(const CCountedPointer& value)
    : CActionParam(9)
    , Value(value)
{
}