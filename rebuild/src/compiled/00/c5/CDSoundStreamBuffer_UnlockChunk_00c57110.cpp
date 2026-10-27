// CDSoundStreamBuffer::UnlockChunk(void* ptr) -- unlock the chunk returned by LockChunk (+0x14 bytes) and advance the stream cursor (+0xc) by one chunk, wrapping at the buffer size (+0x10)
// Leaf: ?UnlockChunk@CDSoundStreamBuffer@@UAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57110, 43 bytes, thiscall `ret 4`.
// Identity: slot 10 of the stream-buffer vtables 0x012B8D90 / 0x012B8DF0 (slot 9 LockChunk 0x00C570D0, slot 8 the landed
//   IsChunkReady 0x00C57070); IDirectSoundBuffer::Unlock is COM slot 19 (+0x4c). Descriptive (libpk, no PDB/RTTI).
// Shape: the Unlock result is dropped; `(cursor + chunk) % size` is `xor edx,edx ; div`.
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
    virtual long __stdcall Play(unsigned long reserved, unsigned long priority, unsigned long flags);   // slot 12
    virtual long __stdcall SetCurrentPosition(unsigned long position);       // slot 13
    virtual long __stdcall SetFormat(const void* format);                    // slot 14
    virtual long __stdcall SetVolume(long volume);                           // slot 15
    virtual long __stdcall SetPan(long pan);                                 // slot 16
    virtual long __stdcall SetFrequency(unsigned long frequency);            // slot 17
    virtual long __stdcall Stop();                                           // slot 18
    virtual long __stdcall Unlock(void* ptr1, unsigned long bytes1, void* ptr2, unsigned long bytes2);   // slot 19 (+0x4c)
};

class CDSoundStreamBuffer {                        // vtable 0x012B8D90
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool IsChunkReady();                   // slot 8 (0x00C57070)
    virtual void* LockChunk();                     // slot 9 (0x00C570D0)
    virtual void UnlockChunk(void* ptr);           // slot 10
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    long                m_8;                       // +0x08
    unsigned long       m_cursor;                  // +0x0c
    unsigned long       m_bufferBytes;             // +0x10
    unsigned long       m_chunkBytes;              // +0x14
};

void CDSoundStreamBuffer::UnlockChunk(void* ptr)
{
    m_pBuffer->Unlock(ptr, m_chunkBytes, 0, 0);
    m_cursor = (m_cursor + m_chunkBytes) % m_bufferBytes;
}