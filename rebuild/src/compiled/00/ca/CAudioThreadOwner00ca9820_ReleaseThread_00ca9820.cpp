// CAudioThreadOwner_00ca9820::ReleaseThread() -- resets the +0xBC event, signals and closes the +0xC0 handle, marks the owner detached (+0xC4 = 2) and clears the handle
// Leaf: ReleaseThread
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9820, 64 bytes.
// Callees: kernel32 imports ResetEvent (IAT 0x0143FED4), SetEvent (0x0143FDE0), CloseHandle (0x0143FE30).
//
// The manifest label `CPhysicalPrimitiveCylinder::TransferBinaryOut` is propagated and wrong: this
// is the fallback the landed CAudioThreadOwner_00cab870::Attach calls when the owner is not ready
// (it is called from 0x00CA9A25 and 0x00CAB968). Attach duplicates the caller's thread handle into
// +0xC0 and sets +0xC4; this undoes it: ResetEvent(+0xBC), SetEvent(+0xC0), CloseHandle(+0xC0), then
// the state word +0xC4 becomes 2 and the handle is cleared. Each member is reloaded into a scratch
// register and pushed (speed TU); each import is called once, so a plain `__declspec(dllimport)`
// declaration gives `call dword ptr [__imp_..]`.
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);

struct CAudioThreadOwner_00ca9820 {
    unsigned char _pad[0xBC];
    void*         m_hEvent;                // +0xBC
    void*         m_hThread;               // +0xC0
    long          m_state;                 // +0xC4
    void ReleaseThread();
};

void CAudioThreadOwner_00ca9820::ReleaseThread()
{
    ResetEvent(m_hEvent);
    SetEvent(m_hThread);
    CloseHandle(m_hThread);
    m_state = 2;
    m_hThread = 0;
}