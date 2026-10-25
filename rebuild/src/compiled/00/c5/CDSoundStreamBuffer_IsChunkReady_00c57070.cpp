// CDSoundStreamBuffer::IsChunkReady() -- ask the DirectSound buffer for its play cursor and report whether at least one chunk (+0x14) has been played since the stream's cursor (+0xc), wrapping round the buffer size (+0x10)
// Leaf: ?IsChunkReady@CDSoundStreamBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57070, length 86 bytes (+ int3 padding); slot 8 of the vtable 0x012B8D90
// (also slot 8 of the derived table 0x012B8DF0 at 0x012B8E10).
// Callees: IDirectSoundBuffer::GetCurrentPosition (COM slot 4, `call [ecx+10h]`, `this` pushed). Any failure HRESULT -> false.
// Model: the DirectSound streaming buffer of the libpk audio driver (buffer +4, as in the landed ReleaseBuffer
// 0x00C57040, which clears it and +0x10). No PDB/RTTI: the names are descriptive; the catalog label
// "Stream_CheckBytesAvailable" is descriptive too.
// Shape: `return avail >= chunk;` gives `sbb eax,eax ; inc eax`; retail's byte `sbb al,al ; inc al` comes from the
// if/return-false form (a named bool or `?: true : false` give the same bytes). VC tail-duplicates the compare per arm.
struct IDirectSoundBuffer {                        // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetCaps(void* caps);                              // slot 3
    virtual long __stdcall GetCurrentPosition(unsigned long* play, unsigned long* write);   // slot 4 (+0x10)
};

struct CDSoundStreamBuffer {
    void*               __vftable;                 // +0x00 (vtable 0x012B8D90)
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    long                m_8;                       // +0x08
    unsigned long       m_cursor;                  // +0x0c (byte offset consumed so far)
    unsigned long       m_bufferBytes;             // +0x10
    unsigned long       m_chunkBytes;              // +0x14
    bool IsChunkReady();                           // vtable slot 8
};

bool CDSoundStreamBuffer::IsChunkReady()
{
    unsigned long play, write;
    if (m_pBuffer->GetCurrentPosition(&play, &write) != 0)
        return false;
    unsigned long played;
    if (m_cursor <= play)
        played = play - m_cursor;
    else
        played = m_bufferBytes - m_cursor + play;
    if (played < m_chunkBytes)
        return false;
    return true;
}