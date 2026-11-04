// PKCProtectionInstance -- libpk's scoped lock guard (8 bytes): the constructor 0x00C0FFC0 keeps the section, enters it and stamps
// GetTickCount at +4; the destructor 0x00C0FFE0 traces ".\PKCProtectionInstance.cpp(19) : Event time (N ms) over maximum (M ms)"
// when the hold exceeded the global maximum 0x0139C904, then leaves the section.
// Identity: the class name is the guard's own trace file string ".\PKCProtectionInstance.cpp" (libpk names its classes after
//   their files); the layout is the landed definers' (0x00C0FFC0 / 0x00C0FFE0). Callers: every libpk method that locks a module
//   section (scoped `PKCProtectionInstance lock(&g_lock_...)`).
// Vendor naming V2 (plan phase 1, 2026-11-04): one shared model instead of ~200 local copies.
#ifndef FABLE_LIBPK_PKCPROTECTIONINSTANCE_H
#define FABLE_LIBPK_PKCPROTECTIONINSTANCE_H
#include "libpk/PKCCriticalSection.h"

class PKCProtectionInstance {                   // 8 bytes: the guarded section and the entry tick
public:
    PKCCriticalSection* m_pSection;             // +0
    unsigned long m_tick;                       // +4 GetTickCount at entry
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

#endif
