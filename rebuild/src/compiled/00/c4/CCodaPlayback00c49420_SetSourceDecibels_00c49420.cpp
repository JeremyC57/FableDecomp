// CCodaPlayback_00c49420::SetSourceDecibels(float value) -- under the CODA profile section (0x0143BAFC) forward the value to the playback source held at +0x04: the helper stores the decibel level at the source's +0x18 when it changed (0x00C42F50, manifest "Audio_SetDecibels")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49420, length 50 bytes.
// Leaf: SetSourceDecibels
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock),
// CodaSource_SetDecibels @ 0x00C35AF0 (a free `__stdcall` (source, value) helper, `ret 8`).
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
void __stdcall CodaSource_SetDecibels(void* source, float value);   // 0x00C35AF0

struct CCodaPlayback_00c49420 {
    long  m_reserved00;                            // +0x00
    void* m_pSource;                               // +0x04 (the CODA source)
    void SetSourceDecibels(float value);
};

void CCodaPlayback_00c49420::SetSourceDecibels(float value)
{
    CProfileLock lock(g_CodaProfileSection);
    CodaSource_SetDecibels(m_pSource, value);
}