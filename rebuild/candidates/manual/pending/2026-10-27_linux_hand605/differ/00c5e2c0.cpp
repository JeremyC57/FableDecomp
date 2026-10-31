// CWound_00c5eca0::WriteBuffer(long unused) -- vtable slot 8 of the kernel-streaming output device: submit the stream header at the write index to the pin (DeviceIoControl IOCTL_KS_WRITE_STREAM, overlapped), count it as queued under the critical section and advance the write index round the ring
// Grade: DIFFER(102v102) 13 lines incl. 3 IAT relocs (unchanged in chunk 605) -- register colouring only: retail builds the OVERLAPPED pointer and the header size in ecx, &returned in edx and loads the pin handle into edx before the size push; ours uses edx/ecx and loads the handle into eax after the block push. Tried in chunk 605: an inline CKsObject::WriteData(header, overlapped) wrapper with the returned local inside (same), `returned` declared first + pointer-arithmetic block (same), /Oa /G5 /Ow (same), /G7 (95), optimize("s") (88). Name is descriptive (no PDB/RTTI; the class label is a propagated placeholder) -- name_review if it ever matches.
// Leaf: ?WriteBuffer@CWound_00c5eca0@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E2C0, 102 bytes (+ int3 padding), thiscall `ret 4`; slot 8 of the
//   vtable 0x012B914C (word at 0x012B916C).
// Callees: DeviceIoControl (KERNEL32 IAT 0x0143FEC0; code 0x002F8013 = IOCTL_KS_WRITE_STREAM, no input, the header as the
//   output buffer of header.Size bytes, the block's OVERLAPPED), EnterCriticalSection / LeaveCriticalSection (IAT
//   0x0143FE0C / 0x0143FE10) on +0x44.
// Name: descriptive (no PDB/RTTI); parallels the landed CAudioWaveOut::WriteWaveBuffer(long) 0x00C5D6B0 (same unused
//   argument, same queued count / ring index logic). The manifest label `_gapscan::sub_00c5e2c0` is a placeholder.
// Shape: the bytes-returned count is a frame local (`push ecx`); the block pointer is a named local; the critical
//   section address stays in edi for both calls; `if (++index == count) index = 0;`.
extern "C" __declspec(dllimport) int __stdcall DeviceIoControl(void* device, unsigned long code, void* in, unsigned long inSize,
    void* out, unsigned long outSize, unsigned long* returned, struct OVERLAPPED_14* overlapped);   // IAT 0x0143FEC0
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(struct CRITICAL_SECTION_18* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(struct CRITICAL_SECTION_18* cs);   // IAT 0x0143FE10
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

void CWound_00c5eca0::WriteBuffer(long unused)
{
    SKsStreamBlock* block = &m_pBlocks[m_writeIndex];
    unsigned long returned;
    DeviceIoControl(m_pin.m_handle, 0x2f8013, 0, 0, block, block->Size, &returned, &block->Overlapped);
    EnterCriticalSection(&m_lock);
    ++m_queued;
    LeaveCriticalSection(&m_lock);
    if (++m_writeIndex == m_eventCount)
        m_writeIndex = 0;
}
