// PKCCriticalSection -- libpk (Lionhead audio library linked into Fable.exe; no PDB or RTTI covers it): the library's lock, a bare
// Win32 CRITICAL_SECTION (0x18 bytes) behind four KERNEL32 wrappers.
// Identity (descriptive under the owner's vendor rule, 2026-11-04): the class name follows libpk's file naming (PKCxxx classes
//   live in PKCxxx.cpp; the scoped guard's trace string is ".\PKCProtectionInstance.cpp"); the four members are the landed
//   wrappers 0x00C1C810 Initialize (InitializeCriticalSection) / 0x00C1C820 Delete / 0x00C1C830 Enter / 0x00C1C840 Leave
//   (sweep1 2026-10-25; landed PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0 call Enter / Leave).
// Class key: `class` (the landed definers and 217 of the 249 landed declarations; the 32 `struct` declarations bound
//   PAU / 3U spellings of the same addresses -- vendor naming V2 moves them onto this header).
// Vendor naming V2 (plan phase 1, 2026-11-04): one shared model instead of ~250 local copies.
#ifndef FABLE_LIBPK_PKCCRITICALSECTION_H
#define FABLE_LIBPK_PKCCRITICALSECTION_H

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

#endif
