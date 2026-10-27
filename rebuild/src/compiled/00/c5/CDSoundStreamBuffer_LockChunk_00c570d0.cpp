// CDSoundStreamBuffer::LockChunk() -- lock the next chunk (+0x14 bytes at the stream cursor +0xc) of the DirectSound buffer and return its first region, or 0 when Lock fails
// Leaf: ?LockChunk@CDSoundStreamBuffer@@UAEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C570D0, 62 bytes, thiscall, plain `ret`.
// Identity: slot 9 of the stream-buffer vtables 0x012B8D90 / 0x012B8DF0 (slot 8 the landed IsChunkReady 0x00C57070, slot 10
//   UnlockChunk 0x00C57110); IDirectSoundBuffer::Lock is COM slot 11 (+0x2c). Descriptive (libpk, no PDB/RTTI).
// Shape: four 4-byte frame locals (16 bytes) passed by address, flags 0; `hr < 0 ? 0 : p1` is `setl ; dec ; and`.
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
                                void** ptr2, unsigned long* bytes2, unsigned long flags);   // slot 11 (+0x2c)
};

class CDSoundStreamBuffer {                        // vtable 0x012B8D90
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool IsChunkReady();                   // slot 8 (0x00C57070)
    virtual void* LockChunk();                     // slot 9
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    long                m_8;                       // +0x08
    unsigned long       m_cursor;                  // +0x0c
    unsigned long       m_bufferBytes;             // +0x10
    unsigned long       m_chunkBytes;              // +0x14
};

void* CDSoundStreamBuffer::LockChunk()
{
    void* ptr1;
    unsigned long bytes1;
    void* ptr2;
    unsigned long bytes2;
    long hr = m_pBuffer->Lock(m_cursor, m_chunkBytes, &ptr1, &bytes1, &ptr2, &bytes2, 0);
    if (hr < 0)
        return 0;
    return ptr1;
}