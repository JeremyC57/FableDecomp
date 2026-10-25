// CCodaPlayback_00c49420::SetSourceVolume(float volume) -- under the CODA profile section (0x0143BAFC) forward the linear volume to the playback source held at +0x04 through CODA_SetSourceVolume (0x00C35AF0, which calls CAudioObject::SetGain 0x00C42F50: the gain at the source's +0x18 and, when it changed, its millibel volume at +0x1c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49420, length 50 bytes.
// Leaf: ?SetSourceVolume@CCodaPlayback_00c49420@@QAEXM@Z
// Callees: CProfileLock::CProfileLock(void*) @ 0x00C0FFC0 / ~CProfileLock @ 0x00C0FFE0 (the 8-byte scoped lock),
// CODA_SetSourceVolume @ 0x00C35AF0 (a free `__stdcall` (source, value) helper, `ret 8`).
// Naming (sweep1, 2026-10-25): was landed as SetSourceDecibels calling `CodaSource_SetDecibels`; FINDINGS chunk 435 asks to
//   reconcile the helper's two names: it forwards a LINEAR gain to the landed CAudioObject::SetGain 0x00C42F50, which
//   converts it to millibels itself, and three landed CCodaPlayback rows (0x00C27550 / 0x00C27620 / 0x00C276B0) call it
//   CODA_SetSourceVolume, the name this pass gives the helper (renamed). The method follows (descriptive).
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
class CAudioObject;                                 // the CODA source object (landed CAudioObject::SetGain 0x00C42F50)
extern "C" void __stdcall CODA_SetSourceVolume(CAudioObject* source, float volume);   // 0x00C35AF0 (the CODA C API)

struct CCodaPlayback_00c49420 {
    long  m_reserved00;                            // +0x00
    CAudioObject* m_pSource;                       // +0x04 (the CODA source)
    void SetSourceVolume(float volume);
};

void CCodaPlayback_00c49420::SetSourceVolume(float volume)
{
    CProfileLock lock(g_CodaProfileSection);
    CODA_SetSourceVolume(m_pSource, volume);
}