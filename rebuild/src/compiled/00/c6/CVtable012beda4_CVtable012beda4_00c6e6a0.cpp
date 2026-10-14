// CVtable_012beda4::CVtable_012beda4() -- a polymorphic object owning a critical section: the
// lock member at +4 initialises its Win32 critical section, a dword and a byte are cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E6A0, length 38 bytes (+ int3 padding).
// Callee: KERNEL32!InitializeCriticalSection through the IAT slot 0x0143FEE4 (on +0x08).
// Vtables (script module, no RTTI, named by address): 0x012BEDA4 is this class; 0x012B8FC8 at +4 is
// the lock holder's OWN vtable (the landed destructor @ 0x00C5D570 stores the same one), so the
// lock holder is a member with an inline constructor, not a base (a base subobject would receive
// this class's vtable-for-base). The base at +0 (vtable 0x012B5868, stored by the destructor @
// 0x00C6E630) has an empty constructor whose vtable store is dead here. This class's vtable store
// precedes the member constructor and the zero stores (init-list order).
// Layout: +0x04 CCriticalSectionOwner_012B8FC8 (vptr + 0x18-byte RTL_CRITICAL_SECTION), long +0x20,
// unsigned char +0x24. The manifest label "CConsoleCommandParameters" is propagated and rejected.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* section);

struct CBase_012b5868 {                        // vtable 0x012B5868
    virtual void v00();
    CBase_012b5868() { }
};

struct CCriticalSectionOwner_012B8FC8 {        // vtable 0x012B8FC8
    virtual void v00();
    unsigned char CriticalSection[0x18];       // +0x04 RTL_CRITICAL_SECTION
    CCriticalSectionOwner_012B8FC8()
    {
        InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};

struct CVtable_012beda4 : CBase_012b5868 {     // vtable 0x012BEDA4
    CCriticalSectionOwner_012B8FC8 m_lock;     // +0x04
    long          m_count;                     // +0x20
    unsigned char m_flag;                      // +0x24
    virtual void v00();
    CVtable_012beda4();
};

CVtable_012beda4::CVtable_012beda4()
    : CBase_012b5868(), m_lock(), m_count(0), m_flag(0)
{
}