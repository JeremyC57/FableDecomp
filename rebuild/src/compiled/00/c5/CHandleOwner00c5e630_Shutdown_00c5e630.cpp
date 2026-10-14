// CHandleOwner_00c5e630::Shutdown() -- run virtual slot 5, then close the Win32 handle at +8 (CloseHandle @ IAT 0x0143FE30) unless it is -1 and set it to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E630, length 32 bytes.
// The chunk label "ClearTextures" is propagated; named by its effect. Shape: `cmp eax,-1`, `ret`.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30
struct CHandleOwner_00c5e630 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void Stop();                           // slot 5 (+0x14)
    long  m_4;
    void* m_hHandle;                               // +0x08
    void Shutdown();
};

void CHandleOwner_00c5e630::Shutdown()
{
    Stop();
    if (m_hHandle != (void*)-1) {
        CloseHandle(m_hHandle);
        m_hHandle = (void*)-1;
    }
}