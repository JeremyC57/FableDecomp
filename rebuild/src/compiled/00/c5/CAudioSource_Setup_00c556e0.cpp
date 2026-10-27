// CAudioSource::Setup(CVoiceDesc_00c556e0* desc) -- slot 1 of the libpk decoded-stream voice: run the software voice's setup, point the voice at its own 128-frame ring (+0x1f0) over the shared scratch block 0x0143BCB0, then open the stream source of a streamed description; 0, -5 when the open fails, or the base's negative result
// Leaf: ?Setup@CAudioSource@@UAEJPAUCVoiceDesc_00c556e0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C556E0, 99 bytes, thiscall `ret 4`.
// Identity: slot 1 of vtable 0x012B8AE0 (constructor 0x00C555E0: the software voice base 0x00C5B1A0, the CAudioRingBuffer
//   at +0x1f0 built by 0x00C5D0A0); this class is the landed CAudioSource (Open 0x00C55680 is called on `this`).
//   Descriptive (libpk, no PDB/RTTI): Setup and the descriptor names are not proven.
// Callees: the base slot-1 setup 0x00C5A1D0 (called directly), CAudioRingBuffer::Init 0x00C5CE00 (bound name), the
//   description's slot 1 (is-streamed test), CAudioSource::Open(void*) 0x00C55680.
// Shape: `streamed ? desc : 0` is neg/sbb/and; `Open(...) ? 0 : -5` is neg/sbb/and/add; the source word read into a named
//   local keeps it in eax with `mov ecx,esi` first (passing `stream->m_pSource` directly loads it into ecx).
struct CVoiceDesc_00c556e0 {
    virtual void v00();
    virtual bool IsStreamed();                          // slot 1
};

struct CStreamDesc_00c556e0 : CVoiceDesc_00c556e0 {
    unsigned char _pad04[0x1cc - 4];
    void* m_pSource;                                    // +0x1cc
};

class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CAudioRingBuffer : public CPKObject {             // vtable 0x012B8FC4, 0x20 bytes
public:
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);   // 0x00C5CE00
};

extern unsigned char g_scratchRing_0143bcb0[];          // 0x0143BCB0

struct CSoftwareVoice_00c5a1d0 {                        // the software voice base (constructor 0x00C5B1A0)
    virtual void v00();
    virtual long Setup(CVoiceDesc_00c556e0* desc);      // slot 1 (0x00C5A1D0)
    unsigned char     _pad04[0xd8 - 4];
    CAudioRingBuffer* m_pRing;                          // +0xd8
    unsigned char     _padDC[0x1f0 - 0xdc];
};

struct CAudioSource : CSoftwareVoice_00c5a1d0 {         // vtable 0x012B8AE0
    virtual long Setup(CVoiceDesc_00c556e0* desc);
    bool Open(void* source);                            // 0x00C55680
    CAudioRingBuffer m_ring;                            // +0x1f0
};

long CAudioSource::Setup(CVoiceDesc_00c556e0* desc)
{
    long result = CSoftwareVoice_00c5a1d0::Setup(desc);
    if (result < 0)
        return result;
    m_ring.Init(g_scratchRing_0143bcb0, 0x80, 2, 1, 0, 0);
    m_pRing = &m_ring;
    CStreamDesc_00c556e0* stream = desc->IsStreamed() ? (CStreamDesc_00c556e0*)desc : 0;
    void* source = stream->m_pSource;
    return Open(source) ? 0 : -5;
}