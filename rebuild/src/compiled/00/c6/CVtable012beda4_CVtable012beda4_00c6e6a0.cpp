// CVtable_012beda4::CVtable_012beda4() -- a polymorphic object owning a critical section: the
// Leaf: ??0CVtable_012beda4@@QAE@XZ
// lock member at +4 initialises its Win32 critical section, the float volume and a byte are cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E6A0, length 38 bytes (+ int3 padding).
// Fix (sweep1, 2026-10-25; FINDINGS chunks 443/444): the +0x20 member is a float volume, not `long m_count`: the mixer
//   effects over this base read it with `fld dword ptr [ecx+20h]` and scale it by 32767.0f (0x00C6CEF0, 0x00C6DE60,
//   0x00C6EBA0, 0x00C6EBE0) and 0x00C70840 compares it with fucompp; the +0x24 byte is the flag 0x00C6EBE0 stores.
// Callee: KERNEL32!InitializeCriticalSection through the IAT slot 0x0143FEE4 (on +0x08).
// Vtables (script module, no RTTI, named by address): 0x012BEDA4 is this class; 0x012B8FC8 at +4 is
// the lock holder's OWN vtable (the landed destructor @ 0x00C5D570 stores the same one), so the
// lock holder is a member with an inline constructor, not a base (a base subobject would receive
// this class's vtable-for-base). The base at +0 (vtable 0x012B5868, stored by the destructor @
// 0x00C6E630) has an empty constructor whose vtable store is dead here. This class's vtable store
// precedes the member constructor and the zero stores (init-list order).
// Layout: +0x04 CCriticalSectionOwner_012B8FC8 (vptr + 0x18-byte RTL_CRITICAL_SECTION), float volume +0x20,
// unsigned char +0x24. The manifest label "CConsoleCommandParameters" is propagated and rejected.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* section);

struct CPKObject {                             // vtable 0x012B5868: the libpk root
    CPKObject() { }
    virtual ~CPKObject() { }
};

struct CCriticalSectionOwner_012B8FC8 {        // vtable 0x012B8FC8
    virtual void v00();
    unsigned char CriticalSection[0x18];       // +0x04 RTL_CRITICAL_SECTION
    CCriticalSectionOwner_012B8FC8()
    {
        InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};

struct CVtable_012beda4 : CPKObject {             // vtable 0x012BEDA4
    CCriticalSectionOwner_012B8FC8 m_lock;     // +0x04
    float         m_volume;                    // +0x20 (read as a float by the mixer effects)
    unsigned char m_flag;                      // +0x24
    virtual void v00();
    CVtable_012beda4();
};

CVtable_012beda4::CVtable_012beda4()
    : CPKObject(), m_lock(), m_volume(0.0f), m_flag(0)
{
}