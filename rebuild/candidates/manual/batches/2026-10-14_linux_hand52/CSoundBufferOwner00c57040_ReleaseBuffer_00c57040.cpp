// CSoundBufferOwner_00c57040::ReleaseBuffer() -- stop (IDirectSoundBuffer slot 18) and release (slot 2) the DirectSound buffer at +4 when present and clear it, then clear the word at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57040, length 48 bytes.
// The chunk label "CJoystickDX::~CJoystickDX" is propagated (the real one is inlined into 0x00AB7950); the COM slot 18 after a slot 2 release is IDirectSoundBuffer::Stop. Named by effect. Shape: `this` in esi, the buffer re-read for the release, `ret`.
struct IDirectSoundBuffer_Retail {                // COM: __stdcall methods, `this` pushed
    virtual void __stdcall v000();
    virtual void __stdcall v001();
    virtual unsigned long __stdcall Release();      // slot 2 (+0x08)
    virtual void __stdcall v003();
    virtual void __stdcall v004();
    virtual void __stdcall v005();
    virtual void __stdcall v006();
    virtual void __stdcall v007();
    virtual void __stdcall v008();
    virtual void __stdcall v009();
    virtual void __stdcall v010();
    virtual void __stdcall v011();
    virtual void __stdcall v012();
    virtual void __stdcall v013();
    virtual void __stdcall v014();
    virtual void __stdcall v015();
    virtual void __stdcall v016();
    virtual void __stdcall v017();
    virtual long __stdcall Stop();                  // slot 18 (+0x48)
};
struct CSoundBufferOwner_00c57040 {
    unsigned char               _pad[4];
    IDirectSoundBuffer_Retail*  m_pBuffer;         // +0x04
    unsigned char               _pad08[8];
    long                        m_playing;         // +0x10
    void ReleaseBuffer();
};

void CSoundBufferOwner_00c57040::ReleaseBuffer()
{
    if (m_pBuffer) {
        m_pBuffer->Stop();
        m_pBuffer->Release();
        m_pBuffer = 0;
    }
    m_playing = 0;
}
