// CWound_00c5eca0::LockChunk() -- vtable slot 7 of the kernel-streaming output device: the data pointer of the stream header at the write index (m_pBlocks[m_writeIndex].Data), the block the mixer fills next
// Leaf: ?LockChunk@CWound_00c5eca0@@UAEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E2B0, 14 bytes (+ int3 padding), thiscall, plain `ret`; slot 7 of the
//   vtable 0x012B914C (word at 0x012B9168).
// Name: descriptive (no PDB/RTTI); parallels the waveOut LockChunk 0x00C5D6A0 and the landed CDSoundStreamBuffer::LockChunk
//   0x00C570D0. The manifest label `_gapscan::sub_00c5e2b0` is a placeholder.
// Shape: `lea eax,[eax+eax*8]` and the `*8` scale are the 0x48-byte block stride; the header's Data field is at +0x28.
// The kernel-streaming output device of the audio module (vtable 0x012B914C, no RTTI; destructor 0x00C5ECA0): the
// landed CWound_00c5eca0 model of Kill 0x00C5EAD0 (slot 5; "CWound" is a propagated class label kept so the class has one
// spelling). Its slots parallel the waveOut output 0x012B8FCC: 5 Kill/Close, 6 the chunk-ready test 0x00C5E2A0, 7 the
// chunk pointer 0x00C5E2B0, 8 the submit 0x00C5E2C0.
struct CRITICAL_SECTION_18 { unsigned char _body[0x18]; };
struct OVERLAPPED_14 { unsigned char _body[0x14]; };
struct SKsStreamBlock {                             // 0x48 bytes: a KSSTREAM_HEADER and the OVERLAPPED of its write
    unsigned long  Size;                            // +0x00 KSSTREAM_HEADER.Size
    unsigned char  _header04[0x24];                 // +0x04 flags, presentation time, duration, frame extent, data used
    void*          Data;                            // +0x28 KSSTREAM_HEADER.Data
    unsigned long  OptionsFlags;                    // +0x2c
    OVERLAPPED_14  Overlapped;                      // +0x30
    unsigned long  _pad44;                          // +0x44
};
struct CKsObject {                                  // vtable 0x012B90D4 / 0x012B9148
    virtual void v00();
    void* m_handle;                                 // +4 (-1 when closed)
};
struct CWin32Handle {                               // vtables 0x012B90E8 / 0x012B90EC: null sentinel
    virtual void v00();
    void* m_handle;                                 // +4
};
struct CWound_00c5eca0 {                            // vtable 0x012B914C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Kill();                            // slot 5 (0x00C5EAD0)
    virtual long IsBelow();                         // slot 6 (0x00C5E2A0)
    virtual void* LockChunk();                      // slot 7 (0x00C5E2B0)
    virtual void WriteBuffer(long unused);          // slot 8 (0x00C5E2C0)
    CKsObject m_unused04;                           // +0x04
    CKsObject m_pin;                                // +0x0c
    SKsStreamBlock* m_pBlocks;                      // +0x14
    void** m_pEvents;                               // +0x18
    void* m_pSamples;                               // +0x1c
    unsigned long m_eventCount;                     // +0x20 blocks in the ring
    long m_queued;                                  // +0x24
    unsigned long m_writeIndex;                     // +0x28
    CWin32Handle m_stopEvent;                       // +0x2c
    CWin32Handle m_thread;                          // +0x34
    unsigned char _pad3c[8];
    CRITICAL_SECTION_18 m_lock;                     // +0x44
};

void* CWound_00c5eca0::LockChunk()
{
    return m_pBlocks[m_writeIndex].Data;
}