// CDSoundStreamBuffer::CreateAndPlay(unsigned long a, unsigned long b) -- create the DirectSound buffer (0x00C57140, third argument false) and start it looping: IDirectSoundBuffer::Play(0, 0, DSBPLAY_LOOPING); true only when both succeed
// Leaf: ?CreateAndPlay@CDSoundStreamBuffer@@UAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57380, 55 bytes (+ int3 padding), thiscall `ret 8`; vtable slot at
//   0x012B8DA4 (slot 3 of the table the ctor 0x00C56D30 stores, 0x012B8D98; slot 5 in the landed numbering from 0x012B8D90)
//   and at 0x012B8E04 (the same slot of 0x012B8DF8, whose abstract base 0x012B8DD0 leaves it pure).
// Callees: 0x00C57140 (thiscall `ret 0Ch`, sets up the DirectSound buffer with a WAVEFORMATEXTENSIBLE; catalog
//   "CSound_AllocateBuffers", descriptive name CreateBuffer here; its other caller, slot 4 0x00C56D70, passes (1, x, true)),
//   IDirectSoundBuffer::Play (COM slot 12, `call [edx+30h]`, `this` pushed).
// Name: descriptive (libpk DirectSound driver, no PDB/RTTI); class name from the landed slot members of the same vtable
//   (IsChunkReady 0x00C57070, LockChunk 0x00C570D0). The manifest label `_gapscan::sub_00c57380` is a placeholder.
// Shape: retail's byte `neg eax ; sbb al,al ; inc al` comes from the if/return-false form; `return hr == 0;` gives the
//   dword `sbb eax,eax ; inc eax` (as for the landed IsChunkReady 0x00C57070).
struct IDirectSoundBuffer {                        // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetCaps(void* caps);                              // slot 3
    virtual long __stdcall GetCurrentPosition(unsigned long* play, unsigned long* write);   // slot 4
    virtual long __stdcall GetFormat(void* format, unsigned long size, unsigned long* written);   // slot 5
    virtual long __stdcall GetVolume(long* volume);                          // slot 6
    virtual long __stdcall GetPan(long* pan);                                // slot 7
    virtual long __stdcall GetFrequency(unsigned long* frequency);           // slot 8
    virtual long __stdcall GetStatus(unsigned long* status);                 // slot 9
    virtual long __stdcall Initialize(void* sound, const void* desc);        // slot 10
    virtual long __stdcall Lock(unsigned long offset, unsigned long bytes, void** ptr1, unsigned long* bytes1,
                                void** ptr2, unsigned long* bytes2, unsigned long flags);   // slot 11
    virtual long __stdcall Play(unsigned long reserved, unsigned long priority, unsigned long flags);   // slot 12 (+0x30)
};
class CDSoundStreamBuffer {                        // vtable 0x012B8D98 (ctor 0x00C56D30)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool CreateAndPlay(unsigned long a, unsigned long b);   // 0x00C57380
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    long                m_8;                       // +0x08
    unsigned long       m_cursor;                  // +0x0c
    unsigned long       m_bufferBytes;             // +0x10
    unsigned long       m_chunkBytes;              // +0x14
    bool CreateBuffer(unsigned long a, unsigned long b, bool c);   // 0x00C57140
};

bool CDSoundStreamBuffer::CreateAndPlay(unsigned long a, unsigned long b)
{
    if (!CreateBuffer(a, b, false))
        return false;
    if (m_pBuffer->Play(0, 0, 1) != 0)
        return false;
    return true;
}