// CVideoPlayer_00ca9a90::SuspendStream() -- reset the player event at +0xd0 (ResetEvent @ IAT 0x0143FED4) and forward to the stream interface at +0x18 (COM slot 16, `__stdcall`), returning its result; 0x80040209 without one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA9B40, length 41 bytes (48 with padding).
// Leaf: SuspendStream
// The chunk label "CTCGraphicAppearance::GetAnims" is propagated and wrong: video-player code next to Pause @ 0x00CA9A90 (landed),
// reached only through the vtable cell 0x012BF7FC; its partner @ 0x00CA9B10 (cell 0x012BF7F8) signals the event and calls slot 15.
// The class is named by that sibling's address; SuspendStream is descriptive.
// Shape: the interface pointer is read again after the null test (retail `mov eax,[esi+0x18]` twice).
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

struct IMediaStream_00ca9b10 {                    // COM: __stdcall methods, `this` pushed
    virtual long __stdcall v000(); virtual long __stdcall v001(); virtual long __stdcall v002(); virtual long __stdcall v003();
    virtual long __stdcall v004(); virtual long __stdcall v005(); virtual long __stdcall v006(); virtual long __stdcall v007();
    virtual long __stdcall v008(); virtual long __stdcall v009(); virtual long __stdcall v010(); virtual long __stdcall v011();
    virtual long __stdcall v012(); virtual long __stdcall v013(); virtual long __stdcall v014();
    virtual long __stdcall Resume();               // slot 15 (+0x3c)
    virtual long __stdcall Suspend();              // slot 16 (+0x40)
};

struct CVideoPlayer_00ca9a90 {
    unsigned char            _pad00[0x18];
    IMediaStream_00ca9b10*   m_pStream;            // +0x18
    unsigned char            _pad1c[0xb4];
    void*                    m_hEvent;             // +0xd0
    long SuspendStream();
};

long CVideoPlayer_00ca9a90::SuspendStream()
{
    ResetEvent(m_hEvent);
    if (!m_pStream)
        return 0x80040209;
    return m_pStream->Suspend();
}