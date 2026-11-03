// CEventTriggerRef::GetDriverCount() -- the number of drivers the referenced event trigger holds: the trigger's driver list (slot 5) asked for its count (slot 5)
// Leaf: ?GetDriverCount@CEventTriggerRef@@QAEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EE30, 15 bytes, thiscall, two virtual calls (the second a tail `jmp`).
// Identity: bound spelling ?GetDriverCount@CEventTriggerRef@@QAEKXZ (caller CBankProcessor::AddNumberTriggers 0x00C2E4D0, which
//   passes the count as InsertDriver's position: `InsertDriver(GetDriverCount(), driver)`). libpk has no PDB / RTTI: names
//   descriptive. Label correction: the manifest's "CMap::GetWorldMapNumber" is propagated across the body shape.
// Layout: the 8-byte {registry, trigger} reference (+0 registry, +4 trigger) built by the constructor 0x00C1EE60; the trigger's
//   slot 5 (+0x14) returns the driver list (the same slot InsertDriver 0x00C1F4E0 reaches before its slot-3 insert).
class CDriverList {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual unsigned long GetCount();                   // slot 5 (+0x14)
};

class CEventTrigger {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CDriverList* GetDrivers();                  // slot 5 (+0x14)
};

struct CNumberTriggerRegistry;
struct CEventTriggerRef {                               // 8 bytes: the registry and the trigger it found or created
    CNumberTriggerRegistry* m_pRegistry;                // +0x00
    CEventTrigger*          m_pTrigger;                 // +0x04
    unsigned long GetDriverCount();
};

unsigned long CEventTriggerRef::GetDriverCount()
{
    return m_pTrigger->GetDrivers()->GetCount();
}
