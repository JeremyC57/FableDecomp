// CVtable_012beda4::~CVtable_012beda4() -- destroy the critical-section member, then the base
// Leaf: ??1CVtable_012beda4@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E630, length 34 bytes (+ int3 padding).
// Fix (sweep1, 2026-10-25; FINDINGS chunks 443/444): the +0x20 member is a float volume, not `long m_count`: the mixer
//   effects over this base read it with `fld dword ptr [ecx+20h]` and scale it by 32767.0f (0x00C6CEF0, 0x00C6DE60,
//   0x00C6EBA0, 0x00C6EBE0) and 0x00C70840 compares it with fucompp; the +0x24 byte is the flag 0x00C6EBE0 stores.
// Callee: KERNEL32!DeleteCriticalSection through the IAT slot 0x0143FDE4 (on +0x08).
// Vtables (script module, no RTTI, named by address): this class's 0x012BEDA4 is stored first; the
// member at +4 restores its own 0x012B8FC8 (the same store the landed out-of-line destructor @
// 0x00C5D570 makes) before DeleteCriticalSection, and the inlined empty base destructor stores
// 0x012B5868 last. The constructor is @ 0x00C6E6A0 (same layout: lock +0x04, float volume +0x20, byte
// +0x24). The destructor is virtual: slot 0 of 0x012BEDA4 is the scalar deleting destructor 0x00C6E6D0 around this
// body (freeing through the root's CPKObject::operator delete 0x00C3C370), and 0x012B5868 is the libpk CPKObject root
// (its own deleting destructor 0x00C3C840), so the root is modelled with its inline virtual destructor (sweep1 fix).
// The manifest label "CConsoleCommandParameters" is propagated and rejected.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);

struct CPKObject {                             // vtable 0x012B5868: the libpk root
    virtual ~CPKObject() { }
};

struct CCriticalSectionOwner_012B8FC8 {        // vtable 0x012B8FC8
    virtual void v00();
    unsigned char CriticalSection[0x18];       // +0x04 RTL_CRITICAL_SECTION
    ~CCriticalSectionOwner_012B8FC8()
    {
        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};

struct CVtable_012beda4 : CPKObject {             // vtable 0x012BEDA4
    CCriticalSectionOwner_012B8FC8 m_lock;     // +0x04
    float         m_volume;                    // +0x20 (read as a float by the mixer effects)
    unsigned char m_flag;                      // +0x24
    virtual void v00();
    ~CVtable_012beda4();
};

CVtable_012beda4::~CVtable_012beda4()
{
}