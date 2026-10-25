// PKCCriticalSection::PKCCriticalSection() -- initialise the embedded Win32 critical section (InitializeCriticalSection(this)) and return this
// Leaf: ??0PKCCriticalSection@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C810, 14 bytes (`push esi ; mov esi,ecx ; push esi ; call [InitializeCriticalSection] ; mov eax,esi ; pop esi ; ret`), thiscall.
// Identity: `this` is a bare 0x18-byte CRITICAL_SECTION handed to the KERNEL32 InitializeCriticalSection import (IAT 0x0143FEE4); callers:
//   the constructors of every lock-holding libpk object, e.g. PKCDefaultFileManager 0x00C1AC40 (its +4 member, call 0x00C1AC71).
// Naming (sweep1, 2026-10-25): the libpk lock family -- PKCCriticalSection (descriptive: a bare CRITICAL_SECTION with
//   the four KERNEL32 wrappers 0x00C1C810 Initialize / 0x00C1C820 Delete / 0x00C1C830 Enter / 0x00C1C840 Leave) and its
//   scoped guard PKCProtectionInstance (0x00C0FFC0 / 0x00C0FFE0; the class name is the guard's warning file string
//   ".\PKCProtectionInstance.cpp"). No PDB covers libpk.
// Label correction: the landed row was the free `void* __fastcall op_and(void*)` over a function-pointer global (catalog "NScript::operator&&<...CIsThingAlive...>", propagated).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // KERNEL32 import 0x0143FEE4

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

PKCCriticalSection::PKCCriticalSection()
{
    InitializeCriticalSection(this);
}
