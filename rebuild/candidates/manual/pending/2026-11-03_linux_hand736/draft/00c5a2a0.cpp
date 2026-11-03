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

class CCodaVoice {                                      // vtable 0x012B5C10, 0xB4 bytes (ctor 0x00C59BA0)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual void Reset();                               // slot 17 (+0x44): 0x00C59860
    unsigned char _pad04[0xb4 - 4];
};

class CSoftwareVoice : public CCodaVoice {              // vtable 0x012B5C60 (ctor 0x00C5B1A0)
public:
    virtual void Reset();                               // slot 17: 0x00C5A2A0
    unsigned char    m_ring[0x20];                      // +0xb4 source ring (CAudioRingBuffer)
    unsigned long    m_d4;                              // +0xd4 (cleared by 0x00C5A6B0 too)
    unsigned char    _padd8[0x1e4 - 0xd8];
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
