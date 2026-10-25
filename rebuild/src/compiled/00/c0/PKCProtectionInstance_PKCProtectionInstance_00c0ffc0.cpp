// PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection* section) -- scoped lock guard: keep the section, enter it, stamp the entry tick (GetTickCount) at +4
// Leaf: ??0PKCProtectionInstance@@QAE@PAVPKCCriticalSection@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0FFC0, 29 bytes (`ret 4`), thiscall.
// Identity: the scoped guard of the libpk locks: it stores the section, enters it (0x00C1C830) and records
//   GetTickCount (IAT 0x0143FDFC) at +4; its destructor 0x00C0FFE0 leaves the section (0x00C1C840) after warning
//   ".\PKCProtectionInstance.cpp(19) : Event time (N ms) over maximum (M ms)" when the hold exceeded the global at
//   0x0139C904. Callers: every libpk method that locks a module section (the call list starts 0x00C088F4, 0x00C08965,
//   ...; e.g. the CSoundEventManager walks over the section @ 0x0143BB14).
// Naming (sweep1, 2026-10-25): the libpk lock family -- PKCCriticalSection (descriptive: a bare CRITICAL_SECTION with
//   the four KERNEL32 wrappers 0x00C1C810 Initialize / 0x00C1C820 Delete / 0x00C1C830 Enter / 0x00C1C840 Leave) and its
//   scoped guard PKCProtectionInstance (0x00C0FFC0 / 0x00C0FFE0; the class name is the guard's warning file string
//   ".\PKCProtectionInstance.cpp"). No PDB covers libpk.
// Label correction: the landed row was CTCHeroExperience::AddSuccessfulBlockParry (catalog and manifest label propagated
//   from a byte-identical game body), calling GetTickCount through a modelled function-pointer global.
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();   // KERNEL32 import 0x0143FDFC

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

class PKCProtectionInstance {                   // 8 bytes: the guarded section and the entry tick
public:
    PKCCriticalSection* m_pSection;             // +0
    unsigned long m_tick;                       // +4
    PKCProtectionInstance(PKCCriticalSection* section);
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection* section)
    : m_pSection(section)
{
    section->Enter();
    m_tick = GetTickCount();
}
