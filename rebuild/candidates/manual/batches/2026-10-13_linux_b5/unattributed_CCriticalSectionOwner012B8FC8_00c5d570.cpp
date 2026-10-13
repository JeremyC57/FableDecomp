// <unattributed> destructor: vtable 0x012B8FC8, DeleteCriticalSection(&this->+0x04)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D570, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov dword ptr [ecx],offset vftable ; add ecx,4 ; push ecx ; call [KERNEL32!DeleteCriticalSection] ; ret
// A polymorphic lock holder: the destructor re-points the vtable and deletes the Win32 critical
// section embedded at +0x04 (import resolved from the IAT slot 0x0143FDE4).
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);

struct CCriticalSectionOwner_012B8FC8 {
    virtual void v00();                     // polymorphic (the destructor stores the vtable)
    unsigned char CriticalSection[0x18];    // +0x04 RTL_CRITICAL_SECTION
    ~CCriticalSectionOwner_012B8FC8();     // non-virtual here so only the plain destructor body is emitted
};

CCriticalSectionOwner_012B8FC8::~CCriticalSectionOwner_012B8FC8()
{
    DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
}
