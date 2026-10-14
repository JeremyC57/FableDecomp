// CHandleOwner_00c5e130::void CloseHandle_00c5e130() -- close the Win32 handle at +4 (CloseHandle @ IAT 0x0143FE30) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E130, length 26 bytes.
// The chunk label is propagated; named by its effect (one of two byte-identical bodies). Shape: `ret`.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30
struct CHandleOwner_00c5e130 {
    long  m_0;
    void* m_hHandle;                               // +0x04
    void CloseHandle_00c5e130();
};

void CHandleOwner_00c5e130::CloseHandle_00c5e130()
{
    if (m_hHandle) {
        CloseHandle(m_hHandle);
        m_hHandle = 0;
    }
}
