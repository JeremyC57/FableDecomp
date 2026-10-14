// CIEngine::CIEngine() -- init-base class, own vtable, the flag byte at +8 cleared.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23750, length 22 bytes (+ int3 padding).
// Callees: CInitBaseClass::CInitBaseClass() @ 0x0099A310 (vtable 0x0129A7CC, clears its byte at +4, bumps the
// instance counter). Vtable 0x012A0D24 is CIEngine's (rtti-vtables.tsv). The manifest label
// "CAIStateGroup_MoveGoodsBase ctor" is propagated from a byte-identical shell and rejected.
// Shape: base constructor, the derived vtable store, then the body assignment `m_initialised = false` (+8).
struct CInitBaseClass {
    virtual void v00();                         // (a non-virtual destructor keeps the deleting destructor out of the TU)
    bool m_flag04;                              // +0x04 (cleared by the base constructor)
    CInitBaseClass();                           // 0x0099A310
};

struct CIEngine : CInitBaseClass {              // vtable 0x012A0D24
    bool m_initialised;                         // +0x08
    CIEngine();
};

CIEngine::CIEngine()
    : CInitBaseClass()
{
    m_initialised = false;
}
