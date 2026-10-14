// CAtmosOwner_00c14930::SetAtmosEditingMode(unsigned char) -- under the profile lock at 0x0143BB14, store the byte argument into the atmosphere record's flag byte at +0xce of the record at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14930, length 46 bytes.
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock, landed shape). Shape: the record pointer
// at +8 is read after the lock, the argument is read from its slot (`mov [eax+0CEh],cl`), the lock is destroyed and `ret 4`. The chunk label
// "CEditControlCentre::SetAtmosEditingMode" is propagated (the 0x00C1xxxx module); the class is named by address.
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};
extern unsigned char g_AtmosProfileSection_0143bb14[1];   // 0x0143BB14

struct SAtmosRecord_00c14930 {
    unsigned char _pad[0xce];
    unsigned char m_atmosFlag;                                      // +0xce
};

struct CAtmosOwner_00c14930 {
    unsigned char       _pad[8];
    SAtmosRecord_00c14930*    m_pAtmos;                  // +0x08
    void SetAtmosEditingMode(unsigned char mode);
};

void CAtmosOwner_00c14930::SetAtmosEditingMode(unsigned char mode)
{
    CProfileLock lock(g_AtmosProfileSection_0143bb14);
    m_pAtmos->m_atmosFlag = mode;
}