// PKCCriticalSection::Leave() -- leave the embedded Win32 critical section (LeaveCriticalSection(this))
// Leaf: ?Leave@PKCCriticalSection@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C840, 8 bytes (`push ecx ; call [LeaveCriticalSection] ; ret`), thiscall.
// Identity: `this` is a bare 0x18-byte CRITICAL_SECTION handed to the KERNEL32 LeaveCriticalSection import (IAT 0x0143FE10); callers:
//   its only caller, ~PKCProtectionInstance 0x00C0FFE0 (call 0x00C100C7).
// Naming (sweep1, 2026-10-25): the libpk lock family -- PKCCriticalSection (descriptive: a bare CRITICAL_SECTION with
//   the four KERNEL32 wrappers 0x00C1C810 Initialize / 0x00C1C820 Delete / 0x00C1C830 Enter / 0x00C1C840 Leave) and its
//   scoped guard PKCProtectionInstance (0x00C0FFC0 / 0x00C0FFE0; the class name is the guard's warning file string
//   ".\PKCProtectionInstance.cpp"). No PDB covers libpk.
// Label correction: the landed row was the free `__fastcall operator_new_00c1c840(void*)` over an address-named import (catalog "operator_new", propagated).
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // KERNEL32 import 0x0143FE10

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

void PKCCriticalSection::Leave()
{
    LeaveCriticalSection(this);
}
