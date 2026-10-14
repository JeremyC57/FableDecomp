// CEnginePrimitiveManager::CEnginePrimitiveManager(long type) -- CBase, the primitive type at +4, this class's vtable, the enabled flag at +8 set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACDB0, length 29 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x012A53E8 = CEnginePrimitiveManager (rtti-vtables.tsv). Shape: the argument store precedes the vtable store
// (VC7.1 schedules the vtable between init-list stores), the byte is an immediate 1, `this` is returned.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct CEnginePrimitiveManager : CBase {           // vtable 0x012A53E8
    long m_type;                                   // +0x04
    bool m_enabled;                                // +0x08
    virtual void v00();
    CEnginePrimitiveManager(long type);
};

CEnginePrimitiveManager::CEnginePrimitiveManager(long type)
    : CBase()
    , m_type(type)
    , m_enabled(true)
{
}
