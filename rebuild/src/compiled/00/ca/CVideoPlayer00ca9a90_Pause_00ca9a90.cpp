// CVideoPlayer_00ca9a90::Pause() -- reset the event at +0xd0 (ResetEvent @ IAT 0x0143FED4) when both handles (+0xd0, +0xd4) exist and forward to the media control at +0x98 (COM slot 5, `__stdcall`), returning its result; VFW_E_NOT_FOUND (0x8004020A) without a control, E_FAIL (0x80004005) without the handles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9A90, length 80 bytes.
// Leaf: Pause
// The chunk label (CMultiStatBar::GetCurrentChange) is propagated; named by address. Shape: `this` in esi, the control reloaded into esi for the pushed-`this` call, `ret`.
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

struct IMediaControl_00ca9a90 {                   // COM: __stdcall methods, `this` pushed
    virtual long __stdcall v000(); virtual long __stdcall v001(); virtual long __stdcall v002(); virtual long __stdcall v003(); virtual long __stdcall v004();
    virtual long __stdcall Pause();                // slot 5 (+0x14)
};

struct CVideoPlayer_00ca9a90 {
    unsigned char            _pad00[0x98];
    IMediaControl_00ca9a90*  m_pControl;           // +0x98
    unsigned char            _pad9c[0x34];
    void*                    m_hEvent;             // +0xd0
    void*                    m_hThread;            // +0xd4
    long Pause();
};

long CVideoPlayer_00ca9a90::Pause()
{
    if (!m_hEvent || !m_hThread)
        return 0x80004005;
    ResetEvent(m_hEvent);
    if (!m_pControl)
        return 0x8004020a;
    return m_pControl->Pause();
}