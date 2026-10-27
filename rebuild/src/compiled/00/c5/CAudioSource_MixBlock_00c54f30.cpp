// CAudioSource::MixBlock(CAudioRingBuffer* ring, void* context, bool effects) -- mix one 0x100-byte block of the voice: copy the block at the ring's read cursor to an aligned frame buffer; with effects, run the +0x230 processor into a second buffer and apply the voice volume to it (always when the master's scaler exists and is enabled, else only under EAX version 1); then run the +0x224 processor on the block, write it to the decoded stream with the master's mix format, and commit 0x80 frames read
// Leaf: ?MixBlock@CAudioSource@@QAEXPAVCAudioRingBuffer@@PAX_N@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54F30, 245 bytes, thiscall `ret 0Ch`, ebp frame realigned to 16 with the /GS
//   cookie (__security_cookie 0x0139C8A8, check 0x00BFE9F9).
// Identity (descriptive; GameCODA/libpk have no PDB or RTTI): slot 12 of the software mixer voice vtable (0x012B8B40),
//   the "voice mixer" the landed CAudioSource::ApplyVolume 0x00C54EA0 names as its only caller (call 0x00C54FD2). The
//   catalog's `AudioBuffer_ApplyGainFilter` is a propagated description. The second argument is never read.
// Callees: CAudioSource::ApplyVolume 0x00C54EA0, the +0x230 / +0x224 block processors 0x00C6C530 / 0x00C6C0F0
//   (unlanded, descriptive), the stream's slot 2, CAudioRingBuffer::CommitRead 0x00C5CF20; globals g_pAudioMaster
//   0x0143BC2C (scaler +0x17C, mix format +0x58) and g_pEAXManager 0x0143BB70 (EAX version +0xE8), as the landed rows spell them.
// Shape: two `__declspec(align(16))` 0x100-byte blocks give the realigned ebp frame, and their char arrays the /GS
//   cookie (64-float blocks compile the same body with no cookie, 221v245); the block copy is the implicit
//   struct assignment (`rep movsd`, 0x40 dwords); the two effect arms tail-merge into one ApplyVolume call.
struct SAudioBlock {                               // one mix block: 0x100 bytes of PCM (0x80 16-bit frames)
    unsigned char m_bytes[0x100];
};

class CAudioRingBuffer {                           // vtable 0x012B8FC4, 0x20 bytes (landed ctor 0x00C5D0A0)
public:
    virtual void v00();
    unsigned char* m_pData;                        // +0x04 sample memory
    unsigned long  m_capacity;                     // +0x08 frames
    unsigned long  m_frameBytes;                   // +0x0c bytes per frame
    unsigned long  m_channels;                     // +0x10
    unsigned long  m_readPos;                      // +0x14 read cursor (frames)
    void CommitRead(unsigned long frames);         // 0x00C5CF20
};

struct CVolumeScaler {                             // the master object's +0x17c member
    unsigned char _pad[4];
    bool          m_bEnabled;                      // +0x04
};

struct SMixFormat {
    long m_0;
    long m_format;                                 // +0x04
};

struct CAudioMaster {                              // the global master object 0x0143BC2C
    unsigned char  _pad[0x58];
    SMixFormat*    m_pMixFormat;                   // +0x58
    unsigned char  _pad5c[0x17c - 0x5c];
    CVolumeScaler* m_pScaler;                      // +0x17c
};
extern CAudioMaster* g_pAudioMaster;               // 0x0143BC2C

class CEAXManager {
public:
    unsigned char _pad[0xe8];
    long          m_version;                       // +0xe8
};
extern CEAXManager* g_pEAXManager;                 // 0x0143BB70

struct CDecodedStream {
    virtual void v00();
    virtual void Rewind();                         // slot 1 (+0x04)
    virtual void Write(SAudioBlock* block, long* format);   // slot 2 (+0x08)
};

struct CBlockEffect_00c6c530 {                     // +0x230
    void Process(SAudioBlock* out, SAudioBlock* in);        // 0x00C6C530
};
struct CBlockFilter_00c6c0f0 {                     // +0x224
    void Process(SAudioBlock* block);              // 0x00C6C0F0
};

struct CAudioSource {
    void*                 __vftable;               // +0x00
    unsigned char         _pad4[0x21c];
    CDecodedStream*       m_pStream;               // +0x220
    CBlockFilter_00c6c0f0 m_filter;                // +0x224
    unsigned char         _pad225[0x230 - 0x225];
    CBlockEffect_00c6c530 m_effect;                // +0x230
    void ApplyVolume(void* dest);                  // 0x00C54EA0
    void MixBlock(CAudioRingBuffer* ring, void* context, bool effects);
};

void CAudioSource::MixBlock(CAudioRingBuffer* ring, void* context, bool effects)
{
    __declspec(align(16)) SAudioBlock block;
    __declspec(align(16)) SAudioBlock processed;
    block = *(SAudioBlock*)(ring->m_readPos * ring->m_frameBytes + ring->m_pData);
    if (effects) {
        CVolumeScaler* scaler = g_pAudioMaster->m_pScaler;
        if (scaler) {
            m_effect.Process(&processed, &block);
            if (scaler->m_bEnabled)
                ApplyVolume(&processed);
        } else if (g_pEAXManager && g_pEAXManager->m_version == 1) {
            m_effect.Process(&processed, &block);
            ApplyVolume(&processed);
        }
    }
    m_filter.Process(&block);
    m_pStream->Write(&block, &g_pAudioMaster->m_pMixFormat->m_format);
    ring->CommitRead(0x80);
}