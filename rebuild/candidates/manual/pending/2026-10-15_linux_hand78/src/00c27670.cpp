// CCodaPlayback_00c27670::SetSourcePropertyFC(float value) -- under the CODA profile section (0x0143BAFC) forward the value to the playback source held at +0x0c: the helper asks the source (virtual slot 1) whether it accepts and, when it does NOT, stores the value at +0xfc with dirty bit 9 (0x00C445C0; the landed 0x00C35F20 types it as bool, the setter reads a float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27670, length 50 bytes.
// Leaf: SetSourcePropertyFC
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock),
// CodaSource_SetPropertyFCUnlessAccepted @ 0x00C35F20 (a free `__stdcall` (source, value) helper, `ret 8`).
// Shape: the 8-byte lock in the frame (`sub esp,8`), the float argument copied through eax as a dword
// (`mov eax,[esp+10h] ; push eax`, no FPU), the source pushed after it, the lock destroyed, `ret 4`. No EH
// frame (the module builds without /EHsc here). The manifest label "CSurveyDialog::GetSoundsCurrentLayer" is
// propagated over the three bodies of this shape; the playback class is unidentified (named by address).
struct CProfileLock {                              // 8 bytes: section + entry tick (GOTCHAS wave 23f)
    void* m_pSection;
    long  m_entryTick;
    CProfileLock(void* section);                   // 0x00C0FFC0
    ~CProfileLock();                               // 0x00C0FFE0
};

extern unsigned char g_CodaProfileSection[];       // 0x0143BAFC
void __stdcall CodaSource_SetPropertyFCUnlessAccepted(void* source, float value);   // 0x00C35F20

struct CCodaPlayback_00c27670 {
    long  m_reserved00;                            // +0x00
    long  m_reserved04;                            // +0x04
    long  m_reserved08;                            // +0x08
    void* m_pSource;                               // +0x0c (the CODA source)
    void SetSourcePropertyFC(float value);
};

void CCodaPlayback_00c27670::SetSourcePropertyFC(float value)
{
    CProfileLock lock(g_CodaProfileSection);
    CodaSource_SetPropertyFCUnlessAccepted(m_pSource, value);
}
