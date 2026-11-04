// PKCDeferredBank::SetOptionCD(bool value) -- under the global profile lock, store a boolean option into the shared settings record and mark the record dirty when the value changes
// Grade: RELOCATION_MATCH -- early return out of the guarded scope keeps retail's /EHsc frame (was DIFFER 66v109 with a
//   nested if)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14BC0, 108 bytes (+4 int3), __fastcall, ret 4; vtable 0x012B29F8 slot 2 (no RTTI).
// Flags: /EHsc
// Leaf: SetOptionCD
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy). Spellings unified: 0143bb14: ?g_settingsLock@@3UPKCCriticalSection@@A -> ?g_settingsLock@@3VPKCCriticalSection@@A; 00c0ffc0: ??0PKCProtectionInstance@@QAE@PAUPKCCriticalSection@@@Z -> ??0PKCProtectionInstance@@QAE@PAVPKCCriticalSection@@@Z
// The catalog label (CBankFileAsyncData::OnFileReadCompletion) is propagated: the row is reached only through
// slot 2 of the unnamed vtable at 0x012B29F8 (its twin, slot 1, sets the neighbouring byte); the class is named
// by its vtable.
// Class (naming wave 13, owner decision 2026-11-01): PKCDeferredBank, the libpk deferred-bank object whose third vtable is
//   0x012B29F8 (built by 0x00C14E40, destroyed by 0x00C14F90); descriptive, libpk has no RTTI and no class strings.
// Callees: PKCProtectionInstance::PKCProtectionInstance 0x00C0FFC0 (on the global lock object @ 0x0143BB14) and ~PKCProtectionInstance 0x00C0FFE0
// (GOTCHAS 23f: an 8-byte guard -- critical section and entry tick; trace string .\PKCProtectionInstance.cpp).
// Layout (by use): the shared settings record pointer at +8; the record keeps a dirty flag at +4 and this
// option byte at +cd.
// Shape: early return out of the guarded scope -- the second exit keeps the /EHsc frame (fs:[0] chain, one state -1
// store hoisted above the branch and a single destructor call); the compare loads the record byte
// before the argument, the record pointer is re-read for each store, and the lock dies on both paths.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_settingsLock;          // 0x0143BB14


struct SSettingsRecord {
    unsigned long m_00;
    bool m_dirty;                                  // +0x04
    unsigned char _pad_0x05[0xc7];
    bool m_optionCC;                               // +0xCC
    bool m_optionCD;                               // +0xCD
};

class PKCDeferredBank {
public:
    virtual void v00();
    virtual void SetOptionCC(bool value);          // slot 1
    virtual void SetOptionCD(bool value);          // slot 2
    unsigned long m_04;                            // +0x04
    SSettingsRecord* m_pSettings;                  // +0x08
};

void PKCDeferredBank::SetOptionCD(bool value)
{
    PKCProtectionInstance lock(&g_settingsLock);
    if (m_pSettings->m_optionCD == value)
        return;
    m_pSettings->m_optionCD = value;
    m_pSettings->m_dirty = true;
}