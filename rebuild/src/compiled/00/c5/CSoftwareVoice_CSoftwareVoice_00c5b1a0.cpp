// CSoftwareVoice::CSoftwareVoice() -- EH-framed constructor of the GameCODA software voice: the CCodaVoice base (0x00C59BA0, EH state 0), the software-voice vtable 0x012B5C60, the source ring buffer at +0xb4 (0x00C5D0A0, state 1) and the two state-variable filters at +0xdc (0x00C6B9A0, an inline counted loop: trivially destructible), then (re)initialise the two shared scratch rings: the mono ring 0x0143BF88 over 0x0143CBC0 and the six-channel mix ring 0x0143BF68 over 0x0143C5C0 (0x80 frames of 2 bytes each)
// Leaf: ??0CSoftwareVoice@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5B1A0, 169 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC5B1A0 "CLandscapeBackgroundTree" = CSoftwareVoice ctor ("CSoundInit"
//   +0xB4 = ring buffer, "CElement"s = voice filters ctor 0xC6B9A0, g_matcherA/B 0x143BF88/0x143BF68 = mono scratch and
//   6-channel mix rings, "Register" 0xC5CE00 = CAudioRingBuffer::Init)"):
//   - the vtable 0x012B5C60 is agent441's CSoftwareVoice table (28 slots; slot 22 SetParameters 0x00C5BD00); the base
//     constructor is CCodaVoice::CCodaVoice 0x00C59BA0;
//   - +0xb4 is built by the ring-buffer constructor 0x00C5D0A0 (vtable 0x012B8FC4: data, capacity, frame bytes, channels,
//     read/write cursors, queued count), the same class whose Init 0x00C5CE00 is called on the two globals; agent441's
//     landed voice rows read +0xb4 as that ring (SizeRing 0x00C5A3C0, the SetCapacity/CommitWrite/CommitRead calls);
//   - +0xdc / +0x100 are the two 0x24-byte state-variable filters of the landed CStateVariableFilter rows (constructor
//     0x00C6B9A0, Filter 0x00C6BAE0, SetParameters 0x00C6BC80) the software mix path runs;
//   - the global rings: 0x0143BF88 gets 1 channel over 0x0143CBC0 (0x100 bytes), 0x0143BF68 gets 6 channels over
//     0x0143C5C0 (0x600 bytes, ending at 0x0143CBC0) -- agent441's g_mixRing is the planar 0x0143BF68.
//   The CLandscapeBackgroundTree name is a propagated manifest label; descriptive names (GameCODA: no PDB/RTTI).
class CPKObject {
public:
    virtual ~CPKObject();
};

class CAudioRingBuffer : public CPKObject {             // vtable 0x012B8FC4, 0x20 bytes
public:
    CAudioRingBuffer();                                 // 0x00C5D0A0
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);   // 0x00C5CE00
};

class CStateVariableFilter {                            // 0x24 bytes, trivially destructible
public:
    CStateVariableFilter();                             // 0x00C6B9A0
    float m_current[3];                                 // +0x00 current coefficients
    float m_target[3];                                  // +0x0c target coefficients
    float m_band;                                       // +0x18
    float m_low;                                        // +0x1c
    long  m_rampPos;                                    // +0x20
};

class CCodaVoice {                                      // vtable 0x012B5C10, 0xB4 bytes
public:
    CCodaVoice();                                       // 0x00C59BA0
    virtual ~CCodaVoice();
    unsigned char _pad04[0xb4 - 4];
};

class CSoftwareVoice : public CCodaVoice {              // vtable 0x012B5C60
public:
    CSoftwareVoice();
    CAudioRingBuffer m_ring;                            // +0xb4 source ring
    unsigned char    _padd4[8];                         // +0xd4
    CStateVariableFilter m_filters[2];                  // +0xdc
};

extern CAudioRingBuffer g_monoRing;                     // 0x0143BF88 mono scratch ring
extern CAudioRingBuffer g_mixRing;                      // 0x0143BF68 planar six-channel mix ring
extern unsigned char    g_monoRingData[0x100];          // 0x0143CBC0
extern unsigned char    g_mixRingData[0x600];           // 0x0143C5C0

CSoftwareVoice::CSoftwareVoice()
{
    g_monoRing.Init(g_monoRingData, 0x80, 2, 1, 0, 0);
    g_mixRing.Init(g_mixRingData, 0x80, 2, 6, 0, 0);
}
