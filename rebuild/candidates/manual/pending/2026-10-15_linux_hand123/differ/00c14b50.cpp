// CSettingsProxy_012b29f8::SetOptionCC(bool value) -- under the global profile lock, store a boolean option into the shared settings record and mark the record dirty when the value changes
// Grade: DIFFER(66v109) -- lane gap (EH frame): retail keeps the /EHsc frame (fs:[0] chain, state -1 store hoisted above the branch, so the record pointer is re-read and the byte compared from a register) although no call can throw inside the lock's scope; this compiler drops the frame entirely. /EHa restores the frame but adds a state-0 store and keeps the -1 store at the end (114v109). Tried: /EHsc, /EHa.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14B50, 108 bytes (+4 int3), __thiscall, ret 4; vtable 0x012B29F8 slot 1 (no RTTI).
// Flags: /EHsc
// Leaf: SetOptionCC
// The catalog label (CBankFileAsyncData::OnFileReadCompletion) is propagated: the row is reached only through
// slot 1 of the unnamed vtable at 0x012B29F8 (its twin, slot 2, sets the neighbouring byte); the class is named
// by its vtable.
// Callees: CProfileLock::CProfileLock 0x00C0FFC0 (on the global lock object @ 0x0143BB14) and ~CProfileLock 0x00C0FFE0
// (GOTCHAS 23f: an 8-byte lock -- section and entry tick).
// Layout (by use): the shared settings record pointer at +8; the record keeps a dirty flag at +4 and this
// option byte at +cc.
// Shape: /EHsc frame (the lock is the only object with a destructor; no call can throw between its
// constructor and the state -1 store, so no state 0 store is emitted); the compare loads the record byte
// before the argument, the record pointer is re-read for each store, and the lock dies on both paths.
struct CProfileLockSection;
extern CProfileLockSection g_settingsLock;          // 0x0143BB14

struct CProfileLock {                              // 8 bytes
    CProfileLockSection* m_pSection;
    unsigned long m_entryTick;
    CProfileLock(CProfileLockSection* section);    // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};

struct SSettingsRecord {
    unsigned long m_00;
    bool m_dirty;                                  // +0x04
    unsigned char _pad_0x05[0xc7];
    bool m_optionCC;                               // +0xCC
    bool m_optionCD;                               // +0xCD
};

class CSettingsProxy_012b29f8 {
public:
    virtual void v00();
    virtual void SetOptionCC(bool value);          // slot 1
    virtual void SetOptionCD(bool value);          // slot 2
    unsigned long m_04;                            // +0x04
    SSettingsRecord* m_pSettings;                  // +0x08
};

void CSettingsProxy_012b29f8::SetOptionCC(bool value)
{
    CProfileLock lock(&g_settingsLock);
    if (m_pSettings->m_optionCC != value) {
        m_pSettings->m_optionCC = value;
        m_pSettings->m_dirty = true;
    }
}
