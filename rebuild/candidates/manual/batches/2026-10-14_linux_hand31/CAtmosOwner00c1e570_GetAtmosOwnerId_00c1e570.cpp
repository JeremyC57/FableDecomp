// CAtmosOwner_00c1e570::GetAtmosOwnerId() -- under the profile lock at 0x0143BB14, read the word at +0x18 of the object at +4 and return it after the lock is released
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E570, length 42 bytes.
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock). Shape: the value survives the lock's
// destructor in esi and is moved to eax after it (`return m_pOwner->m_id;` with the lock in scope). The chunk label
// "CEditControlCentre::SetAtmosEditingMode" is propagated; the class is named by address.
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AtmosProfileSection_0143bb14[1];   // 0x0143BB14

struct SAtmosOwnerRecord_00c1e570 {
    unsigned char _pad[0x18];
    long m_id;                                     // +0x18
};

struct CAtmosOwner_00c1e570 {
    unsigned char              _pad[4];
    SAtmosOwnerRecord_00c1e570* m_pOwner;          // +0x04
    long GetAtmosOwnerId();
};

long CAtmosOwner_00c1e570::GetAtmosOwnerId()
{
    CProfileLock lock(g_AtmosProfileSection_0143bb14);
    return m_pOwner->m_id;
}
