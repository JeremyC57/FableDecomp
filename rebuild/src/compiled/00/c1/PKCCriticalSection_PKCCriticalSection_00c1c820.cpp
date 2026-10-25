// PKCCriticalSection::~PKCCriticalSection() -- delete the embedded Win32 critical section (DeleteCriticalSection(this))
// Leaf: ??1PKCCriticalSection@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C820, 8 bytes (`push ecx ; call [DeleteCriticalSection] ; ret`), thiscall.
// Identity: `this` is a bare 0x18-byte CRITICAL_SECTION handed to the KERNEL32 DeleteCriticalSection import (IAT 0x0143FDE4); callers:
//   the destructors of the same objects, e.g. ~PKCDefaultFileManager 0x00C1A230 (call 0x00C1A349).
// Naming (sweep1, 2026-10-25): the libpk lock family -- PKCCriticalSection (descriptive: a bare CRITICAL_SECTION with
//   the four KERNEL32 wrappers 0x00C1C810 Initialize / 0x00C1C820 Delete / 0x00C1C830 Enter / 0x00C1C840 Leave) and its
//   scoped guard PKCProtectionInstance (0x00C0FFC0 / 0x00C0FFE0; the class name is the guard's warning file string
//   ".\PKCProtectionInstance.cpp"). No PDB covers libpk.
// Label correction: the landed row was the free `__fastcall operator_new_00c1c820(void*)` over an address-named import (catalog "operator_new", propagated).
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // KERNEL32 import 0x0143FDE4

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

PKCCriticalSection::~PKCCriticalSection()
{
    DeleteCriticalSection(this);
}
