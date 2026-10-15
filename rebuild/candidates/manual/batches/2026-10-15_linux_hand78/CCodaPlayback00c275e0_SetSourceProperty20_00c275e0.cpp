// CCodaPlayback_00c275e0::SetSourceProperty20(float value) -- under the CODA profile section (0x0143BAFC) forward the value to the playback source held at +0x0c: the helper stores it at the source's +0x20 (and sets dirty bit 4) when it changed (0x00C42FE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C275E0, length 50 bytes.
// Leaf: SetSourceProperty20
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock),
// CodaSource_SetProperty20 @ 0x00C35B10 (a free `__stdcall` (source, value) helper, `ret 8`).
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
void __stdcall CodaSource_SetProperty20(void* source, float value);   // 0x00C35B10

struct CCodaPlayback_00c275e0 {
    long  m_reserved00;                            // +0x00
    long  m_reserved04;                            // +0x04
    long  m_reserved08;                            // +0x08
    void* m_pSource;                               // +0x0c (the CODA source)
    void SetSourceProperty20(float value);
};

void CCodaPlayback_00c275e0::SetSourceProperty20(float value)
{
    CProfileLock lock(g_CodaProfileSection);
    CodaSource_SetProperty20(m_pSource, value);
}
