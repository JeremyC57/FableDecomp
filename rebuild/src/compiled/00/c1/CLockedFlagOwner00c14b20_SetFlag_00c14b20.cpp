// CLockedFlagOwner_00c14b20::SetFlag() -- under the scoped lock over the global @ 0x0143BB14, set the byte at +4 of the object at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14B20, length 48 bytes.
// Callees: CScopedLock::CScopedLock @ 0x00C0FFC0 / ~CScopedLock @ 0x00C0FFE0 (the landed 8-byte lock of 0x00C38410). Shape: an 8-byte frame for the lock, `this` in esi, no EH frame
// (this TU is built without exceptions). The chunk label "CHTG_Executioner::OnPredicateFail" is propagated (0x00C1xxxx module); the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;         // 0x0143BB14

struct CScopedLock {                               // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);        // 0x00C0FFC0
    ~CScopedLock();                                // 0x00C0FFE0
};

struct CFlagHolder_00c14b20 {
    unsigned char _pad[4];
    bool          m_flag;                          // +0x04
};

struct CLockedFlagOwner_00c14b20 {
    unsigned char          _pad[8];
    CFlagHolder_00c14b20*  m_pHolder;              // +0x08
    void SetFlag();
};

void CLockedFlagOwner_00c14b20::SetFlag()
{
    CScopedLock lock(&g_lock_0143bb14);
    m_pHolder->m_flag = true;
}