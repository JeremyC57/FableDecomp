// CSoftwareVoice::Reset() -- vtable slot 17 of the GameCODA software voice (0x012B5C60): the CCodaVoice base Reset (positions
//   and request word), then clear the resampler phase (+0x1EC), the word at +0x1E4 and the dword at +0xD4, and empty the two
//   shared scratch rings (mono 0x0143BF88, six-channel mix 0x0143BF68)
// Leaf: ?Reset@CSoftwareVoice@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5A2A0, 49 bytes, thiscall, plain `ret` (tail `jmp` into the second ring call).
// Identity: slot 17 (+0x44) of the CSoftwareVoice vtables 0x012B5C60 / 0x012B5CD0 / 0x012B8AE0 (the three data references); the
//   stream voice 0x012B5D70 overrides slot 17 with 0x00C5C290, whose first call is this function (a qualified base call).
//   The base slot 17 0x00C59860 is CCodaVoice::Reset, the name CCodaVoice::Play 0x00C59720 gives the slot it calls last.
//   GameCODA has no PDB / RTTI: names descriptive. Label corrections: the manifest's "CMap::Update" is propagated, the bound
//   `CAudioDevice_00c5c290::Prepare` (QAE) misses that it is the virtual slot of the CSoftwareVoice vtable.
// Callees: CCodaVoice::Reset 0x00C59860, CAudioRingBuffer::Flush 0x00C5CEA0 (read / write cursors and the queued count to 0;
//   descriptive) on g_mixRing / g_monoRing.
// Shape: one zero register for the three stores; the mono ring first, the mix ring as the tail call.
class CAudioRingBuffer {                                // vtable 0x012B8FC4, 0x20 bytes
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14
    unsigned long  m_writePos;                          // +0x18
    unsigned long  m_queued;                            // +0x1c
    void Flush();                                       // 0x00C5CEA0
};

extern CAudioRingBuffer g_monoRing;                     // 0x0143BF88 mono scratch ring
extern CAudioRingBuffer g_mixRing;                      // 0x0143BF68 planar six-channel mix ring

class CAudioObject;
class CCodaVoice {                                      // vtable 0x012B5C10 (19 slots), 0xB4 bytes (ctor 0x00C59BA0)
public:
    virtual ~CCodaVoice();                              // slot 0
    virtual long Initialise(CAudioObject* sound);       // slot 1: 0x00C596E0
    virtual void Stop();                                // slot 2: 0x00C59710
    virtual void ClearField0C();                        // slot 3: 0x00C3F460 (descriptive, landed catalog name)
    virtual bool SupportsEAX();                         // slot 4 (descriptive, landed 0x00C5A210 model)
    virtual void v05();                                 // slot 5 (no name evidence)
    virtual void v06();                                 // slot 6 (no name evidence)
    virtual long Play(CAudioObject* sound);             // slot 7: 0x00C59720
    virtual void v08() = 0;                             // slot 8 (pure; CSoftwareVoice::StartSound 0x00C5B250)
    virtual void v09() = 0;                             // slot 9 (pure; 0x00C5B7C0)
    virtual long CheckPosition(__int64 position);       // slot 10: 0x00C59770
    virtual bool SetPositionB(unsigned long low, unsigned long high) = 0;  // slot 11
    virtual void SetPositionC(__int64 position);        // slot 12: 0x00C597B0
    virtual void v13() = 0;                             // slot 13 (pure; GetDataSize 0x00C3F510 in the derived table)
    virtual void SeekBytes(unsigned long bytes) = 0;    // slot 14 (pure; 0x00C5A250)
    virtual void SetEAXProperty();                      // slot 15: 0x00C597E0
    virtual void GetEAXProperty();                      // slot 16: 0x00C59820
    virtual void Reset();                               // slot 17 (+0x44): 0x00C59860
    unsigned char _pad04[0xb4 - 4];
};

class CSoftwareVoice : public CCodaVoice {              // vtable 0x012B5C60 (ctor 0x00C5B1A0)
public:
    virtual void Reset();                               // slot 17: 0x00C5A2A0
    CAudioRingBuffer m_ring;                            // +0xb4 source ring
    unsigned long    m_d4;                              // +0xd4 (cleared by 0x00C5A6B0 too)
    CAudioRingBuffer* m_pSource;                        // +0xd8 (the ring the mix reads; landed 0x00C5B0F0 model)
    unsigned char    _paddc[0x1e4 - 0xdc];
    unsigned long    m_1e4;                             // +0x1e4
    unsigned long    m_step;                            // +0x1e8 resampling step, 20.12 fixed point
    unsigned long    m_phase;                           // +0x1ec resampling phase (fraction)
};

void CSoftwareVoice::Reset()
{
    CCodaVoice::Reset();
    m_phase = 0;
    m_1e4 = 0;
    m_d4 = 0;
    g_monoRing.Flush();
    g_mixRing.Flush();
}