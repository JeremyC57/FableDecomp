// CVtable_012beda4::~CVtable_012beda4() -- destroy the critical-section member, then the base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E630, length 34 bytes (+ int3 padding).
// Callee: KERNEL32!DeleteCriticalSection through the IAT slot 0x0143FDE4 (on +0x08).
// Vtables (script module, no RTTI, named by address): this class's 0x012BEDA4 is stored first; the
// member at +4 restores its own 0x012B8FC8 (the same store the landed out-of-line destructor @
// 0x00C5D570 makes) before DeleteCriticalSection, and the inlined empty base destructor stores
// 0x012B5868 last. The constructor is @ 0x00C6E6A0 (same layout: lock +0x04, long +0x20, byte
// +0x24). The destructor is non-virtual in this model so only the plain `??1` body is emitted.
// The manifest label "CConsoleCommandParameters" is propagated and rejected.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);

struct CBase_012b5868 {                        // vtable 0x012B5868
    virtual void v00();
    ~CBase_012b5868() { }
};

struct CCriticalSectionOwner_012B8FC8 {        // vtable 0x012B8FC8
    virtual void v00();
    unsigned char CriticalSection[0x18];       // +0x04 RTL_CRITICAL_SECTION
    ~CCriticalSectionOwner_012B8FC8()
    {
        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};

struct CVtable_012beda4 : CBase_012b5868 {     // vtable 0x012BEDA4
    CCriticalSectionOwner_012B8FC8 m_lock;     // +0x04
    long          m_count;                     // +0x20
    unsigned char m_flag;                      // +0x24
    virtual void v00();
    ~CVtable_012beda4();
};

CVtable_012beda4::~CVtable_012beda4()
{
}