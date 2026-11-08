// CAudioDevice_00c3e760::RenderFrame() -- finish an audio frame: let the listener apply its changes (slot 11); with a primary output, hand the mixer's 32-bit channel buffers to the pre-output hook, lock the primary output, interleave the mixer channels into it in the output's speaker order (mono, stereo, quad, 5.1), hand the 16-bit block to the post-output hook, unlock it, and render the scratch block into the secondary output when there is one; returns 0
// Leaf: ?RenderFrame@CAudioDevice_00c3e760@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3E890, 438 bytes of code + a 6-entry jump table (0x00C3EA48), thiscall, plain `ret`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the device object of the landed BeginFrame 0x00C3E760 and
//   UpdateMeters 0x00C3E800 (+0x58 mixer, +0x17c scratch block, +0x180 / +0x184 primary / secondary outputs, +0x1bc channel
//   count, +0x1d4 listener); single caller, the audio system's frame update 0x00C3D5D9, which calls BeginFrame and
//   UpdateMeters first. The manifest's "Audio_SubmitAndProcess" is a descriptive guess.
// Callees (all virtual): listener slot 11 (+0x2c), output slot 7 Lock (+0x1c, returns the 16-bit sample block) and slot 8
//   Unlock (+0x20), mixer slot 10 (+0x28) WriteChannel(dest, channel) (interleaved, 16-bit), scratch-block slot 7 (+0x1c)
//   Render(dest); the two __stdcall hooks at +0x1a8 (pre-output, stage 0) and +0x1a4 (post-output, stage 1).
// Block descriptor (frame 0x1c): sample format (32 for the mixer's long buffers, 1 for the 16-bit output), channels, the
//   44100 Hz rate, channels << 8, two zero words, the data pointer (the mixer's channel-buffer table at +4, or the locked block).
// Speaker order: the mixer keeps front L/R in channels 0/1, rear L/R in 2/3, centre / LFE in 4/5; 5.1 output wants FL FR C
//   LFE RL RR, so the six slots take channels 0, 1, 4, 5, 2, 3 (3 and 5 channels write nothing).
struct SAudioBlockDesc {
    long  m_format;                             // +0x00 32: the mixer's 32-bit buffers, 1: 16-bit samples
    long  m_channels;                           // +0x04
    long  m_sampleRate;                         // +0x08 44100
    long  m_blockSize;                          // +0x0c channels << 8
    long  m_reserved0;                          // +0x10
    long  m_reserved1;                          // +0x14
    void* m_pData;                              // +0x18
};
typedef void (__stdcall* AudioBlockHook)(SAudioBlockDesc* desc, long stage);

struct COutput_00c3e760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual bool IsReady();                     // slot 6 (+0x18)
    virtual short* Lock();                      // slot 7 (+0x1c)
    virtual void Unlock(short* block);          // slot 8 (+0x20)
};

struct CMixer_00c3e760 {
    virtual void v00(); virtual void v01();
    virtual void Lock();                        // slot 2 (+8)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09();
    virtual void WriteChannel(short* dest, long channel);   // slot 10 (+0x28)
    long* m_channelBuffers[6];                  // +0x04
};

struct CListener_00c3e760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Commit();                      // slot 10 (+0x28)
    virtual void Apply();                       // slot 11 (+0x2c)
};

struct CScratchBlock_00c3e760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual void Render(short* dest);           // slot 7 (+0x1c)
};

struct CAudioDevice_00c3e760 {
    unsigned char _pad_0x000[0x58];
    CMixer_00c3e760* m_pMixer;                  // +0x058
    unsigned char _pad_0x05c[0x120];
    CScratchBlock_00c3e760* m_pScratch;         // +0x17c
    COutput_00c3e760* m_pPrimary;               // +0x180
    COutput_00c3e760* m_pSecondary;             // +0x184
    unsigned char _pad_0x188[0x1c];
    AudioBlockHook m_pPostOutputHook;           // +0x1a4
    AudioBlockHook m_pPreOutputHook;            // +0x1a8
    unsigned char _pad_0x1ac[0x10];
    unsigned long m_channels;                   // +0x1bc
    unsigned char _pad_0x1c0[0x14];
    CListener_00c3e760* m_pListener;            // +0x1d4
    long RenderFrame();
};

long CAudioDevice_00c3e760::RenderFrame()
{
    SAudioBlockDesc desc;
    if (m_pListener)
        m_pListener->Apply();
    if (!m_pPrimary)
        return 0;
    if (m_pPreOutputHook) {
        desc.m_format = 32;
        desc.m_channels = m_channels;
        desc.m_sampleRate = 44100;
        desc.m_blockSize = m_channels << 8;
        desc.m_reserved0 = 0;
        desc.m_reserved1 = 0;
        desc.m_pData = m_pMixer->m_channelBuffers;
        m_pPreOutputHook(&desc, 0);
    }
    short* block = m_pPrimary->Lock();
    switch (m_channels) {
    case 6:
        m_pMixer->WriteChannel(block, 0);
        m_pMixer->WriteChannel(block + 1, 1);
        m_pMixer->WriteChannel(block + 2, 4);
        m_pMixer->WriteChannel(block + 3, 5);
        m_pMixer->WriteChannel(block + 4, 2);
        m_pMixer->WriteChannel(block + 5, 3);
        break;
    case 4:
        m_pMixer->WriteChannel(block, 0);
        m_pMixer->WriteChannel(block + 1, 1);
        m_pMixer->WriteChannel(block + 2, 2);
        m_pMixer->WriteChannel(block + 3, 3);
        break;
    case 2:
        m_pMixer->WriteChannel(block, 0);
        m_pMixer->WriteChannel(block + 1, 1);
        break;
    case 1:
        m_pMixer->WriteChannel(block, 0);
        break;
    }
    if (m_pPostOutputHook) {
        desc.m_format = 1;
        desc.m_channels = m_channels;
        desc.m_sampleRate = 44100;
        desc.m_blockSize = m_channels << 8;
        desc.m_reserved0 = 0;
        desc.m_reserved1 = 0;
        desc.m_pData = block;
        m_pPostOutputHook(&desc, 1);
    }
    m_pPrimary->Unlock(block);
    if (m_pSecondary && m_pScratch) {
        short* scratch = m_pSecondary->Lock();
        m_pScratch->Render(scratch);
        m_pSecondary->Unlock(scratch);
    }
    return 0;
}