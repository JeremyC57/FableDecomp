// CEnginePrimitiveManager::CEnginePrimitiveManager(long type) -- CBase, the primitive type at +4, this class's vtable, the enabled flag at +8 set
// Leaf: ??0CEnginePrimitiveManager@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACDB0, length 29 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x012A53E8 = CEnginePrimitiveManager (rtti-vtables.tsv). Shape: the argument store precedes the vtable store
// (VC7.1 schedules the vtable between init-list stores), the byte is an immediate 1, `this` is returned.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CEnginePrimitiveManager : public CBaseClassNonCopyable { public:           // vtable 0x012A53E8
    long m_type;                                   // +0x04
    bool m_enabled;                                // +0x08
    CEnginePrimitiveManager(long type);
};

CEnginePrimitiveManager::CEnginePrimitiveManager(long type)
    : CBaseClassNonCopyable()
    , m_type(type)
    , m_enabled(true)
{
}