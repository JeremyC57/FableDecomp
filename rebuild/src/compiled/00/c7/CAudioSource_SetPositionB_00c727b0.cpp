// CAudioSource::SetPositionB(unsigned long low, unsigned long high) -- vtable slot 11 of the OpenAL voice: a zero position clears the +0x2C request bit 0x80, pauses the OpenAL source (alSourcePause on +0x58) and returns true; any other position raises 0x80, is stored at +0x18 / +0x1C and returns false
// Leaf: ?SetPositionB@CAudioSource@@UAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C727B0, 65 bytes (+ int3 padding), thiscall `ret 8`; slot 11 of the
//   OpenAL voice vtables 0x012BEE40 (ctor 0x00C72600) and 0x012B8CC8 / 0x012B8D48. The software voice's override is
//   0x00C5A210 (same shape, raising 0x100 instead of pausing).
// Callee: CAudioContext::SourcePause @ 0x00C72560 (thiscall `ret 4` on the global context 0x0143BDB0; resolves
//   "alSourcePause" 0x012BEE20 into +0x74; not landed, named from the string).
// Name: the class is the landed CAudioSource of the OpenAL voice rows (0x00C71F10, 0x00C72AF0, 0x00C72C40: SourceId at
//   +0x58, Flags +0x28); the method name is descriptive (no PDB/RTTI). The manifest label is a placeholder.
// Shape: two dword parameters tested separately; the SourceId load is scheduled with the request-word update.
// GameCODA voice base (no PDB/RTTI; names descriptive): the landed CCodaVoice (ctor 0x00C59BA0, vtable 0x012B5C10).
struct SVoiceFlags {                                // +0x28: single-bit state flags (bit fields: retail reads them with
    unsigned long m_bit0 : 1;                       //   `shr ; test al,1` and sets them through a register)
    unsigned long m_bits1to4 : 4;
    unsigned long m_bit5 : 1;
    unsigned long m_rest : 26;
};
struct SVoiceRequests {                             // +0x2c: pending-change bits (0x80000000 after the ctor)
    unsigned long m_bits0to6 : 7;
    unsigned long m_bit7 : 1;                       // 0x80
    unsigned long m_bit8 : 1;                       // 0x100
    unsigned long m_bit9 : 1;                       // 0x200
    unsigned long m_rest : 22;
};
class CCodaVoice {                                  // vtable 0x012B5C10 (19 slots; 8, 9, 11, 13, 14 pure)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool SupportsEAX();                     // slot 4 (+0x10)
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual long CheckPosition(__int64 position);   // slot 10: 0x00C59770
    virtual bool SetPositionB(unsigned long low, unsigned long high) = 0;   // slot 11: pure here (0x00C5A210 software, 0x00C727B0 OpenAL)
    virtual void SetPositionC(__int64 position);    // slot 12: 0x00C597B0
    unsigned char  _pad08[8];                       // (VC7.1 pads the vfptr to 8 in a class holding an __int64)
    __int64        m_endPosition;                   // +0x10
    unsigned long  m_positionBLow;                  // +0x18
    unsigned long  m_positionBHigh;                 // +0x1c
    __int64        m_positionC;                     // +0x20
    SVoiceFlags    m_flags;                         // +0x28
    SVoiceRequests m_requests;                      // +0x2c
};

struct CAudioContext {                              // the OpenAL wrapper (landed CAudioContext rows; global 0x0143BDB0)
    void SourcePause(unsigned long source);         // 0x00C72560
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

class CAudioSource : public CCodaVoice {            // vtable 0x012BEE40
public:
    virtual bool SetPositionB(unsigned long low, unsigned long high);   // slot 11: 0x00C727B0
    unsigned char  _pad30[0x28];
    unsigned long  SourceId;                        // +0x58 OpenAL source name
};

bool CAudioSource::SetPositionB(unsigned long low, unsigned long high)
{
    if (low == 0 && high == 0) {
        m_requests.m_bit7 = 0;
        g_AudioContext.SourcePause(SourceId);
        return true;
    }
    m_requests.m_bit7 = 1;
    m_positionBLow = low;
    m_positionBHigh = high;
    return false;
}