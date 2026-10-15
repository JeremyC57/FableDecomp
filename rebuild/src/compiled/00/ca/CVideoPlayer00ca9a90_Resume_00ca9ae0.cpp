// CVideoPlayer_00ca9a90::Resume() -- signal the player event at +0xd0 (SetEvent @ IAT 0x0143FDE0), clear the paused byte at +0x24 and forward to the media control at +0x98 (COM slot 6, `__stdcall`), returning its result; VFW_E_NOT_FOUND-style 0x8004020A without a control
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9AE0, length 45 bytes (48 with padding).
// Leaf: Resume
// The chunk label "CTCGraphicAppearance::GetAnims" is propagated and wrong: this is the video player's vtable slot right after
// Pause @ 0x00CA9A90 (landed; vtable cells 0x012BF7BC / 0x012BF7C0), which resets the same event and calls control slot 5.
// The class is named by that sibling's address; Resume is descriptive.
// Shape: `this` in esi, the control reloaded into esi for the pushed-`this` call, bare `ret`.
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0

struct IMediaControl_00ca9a90 {                   // COM: __stdcall methods, `this` pushed
    virtual long __stdcall v000(); virtual long __stdcall v001(); virtual long __stdcall v002(); virtual long __stdcall v003(); virtual long __stdcall v004();
    virtual long __stdcall Pause();                // slot 5 (+0x14)
    virtual long __stdcall Run();                  // slot 6 (+0x18)
};

struct CVideoPlayer_00ca9a90 {
    unsigned char            _pad00[0x24];
    bool                     m_paused;             // +0x24
    unsigned char            _pad25[0x73];
    IMediaControl_00ca9a90*  m_pControl;           // +0x98
    unsigned char            _pad9c[0x34];
    void*                    m_hEvent;             // +0xd0
    void*                    m_hThread;            // +0xd4
    long Resume();
};

long CVideoPlayer_00ca9a90::Resume()
{
    SetEvent(m_hEvent);
    m_paused = false;
    if (!m_pControl)
        return 0x8004020a;
    return m_pControl->Run();
}