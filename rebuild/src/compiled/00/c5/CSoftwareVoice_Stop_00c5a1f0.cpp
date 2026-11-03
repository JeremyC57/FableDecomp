// CSoftwareVoice::Stop() -- vtable slot 2 of the GameCODA software voice (0x012B5C60): empty the source ring (+0xB4), drop the current buffer (+0x0C), then the CCodaVoice base Stop marks the voice stopped
// Leaf: ?Stop@CSoftwareVoice@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5A1F0, 29 bytes, thiscall, plain `ret` (tail `jmp` to the base slot).
// Identity: slot 2 (+0x08) of the CSoftwareVoice vtables 0x012B5C60 / 0x012B5CD0 / the stream voice 0x012B5D70 / 0x012B8AE0
//   (the four data references of 0x00C5A1F0); the base slot 2 is CCodaVoice::Stop 0x00C59710 (sets the stopped bit 31 of the
//   request word +0x2C: 0x80000000), and ~CSoftwareVoice 0x00C3F490 calls this override when the voice is not stopped.
//   GameCODA has no PDB / RTTI: class and method names are descriptive (the landed CSoftwareVoice / CCodaVoice rows, the
//   destructor's Stop). Label correction: the manifest's "CMap::Update" is propagated; the bound `CVtable_012b5c60::Stop`
//   uses a vtable placeholder for the class every other slot of 0x012B5C60 binds as CSoftwareVoice.
// Callees: CAudioRingBuffer::Clear 0x00C5CE40 (zeroes +0x04..+0x1C) on the ring at +0xB4, CCodaVoice::Stop 0x00C59710.
// Shape: qualified base call last, so VC7.1 turns it into `pop esi ; jmp`.
class CCodaObjectBase;
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
    void Clear();                                       // 0x00C5CE40
};

class CSoundBuffer;
class CCodaVoice {                                      // vtable 0x012B5C10, 0xB4 bytes (ctor 0x00C59BA0)
public:
    virtual ~CCodaVoice();                              // slot 0
    virtual long v01();                                 // slot 1
    virtual void Stop();                                // slot 2: 0x00C59710
    unsigned char _pad04[8];
    CSoundBuffer* m_pBuffer;                            // +0x0c
    unsigned char _pad10[0xb4 - 0x10];
};

class CSoftwareVoice : public CCodaVoice {              // vtable 0x012B5C60 (ctor 0x00C5B1A0)
public:
    virtual void Stop();                                // slot 2: 0x00C5A1F0
    CAudioRingBuffer m_ring;                            // +0xb4 source ring
};

void CSoftwareVoice::Stop()
{
    m_ring.Clear();
    m_pBuffer = 0;
    CCodaVoice::Stop();
}