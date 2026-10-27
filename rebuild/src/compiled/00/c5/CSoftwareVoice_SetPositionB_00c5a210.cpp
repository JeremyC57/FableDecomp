// CSoftwareVoice::SetPositionB(unsigned long low, unsigned long high) -- vtable slot 11 of the GameCODA software voice: a zero position clears the +0x2C request bit 0x80, raises 0x100 and returns true; any other position raises 0x80, is stored at +0x18 / +0x1C and returns false
// Leaf: ?SetPositionB@CSoftwareVoice@@UAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5A210, 55 bytes (+ int3 padding), thiscall `ret 8`; slot 11 of the
//   CSoftwareVoice vtable 0x012B5C60 (also of 0x012B5CD0 and the stream voice 0x012B5D70). The base CCodaVoice slot is
//   _purecall; the OpenAL source's override is 0x00C727B0.
// Name: descriptive (GameCODA, no PDB/RTTI; the manifest label `_gapscan::sub_00c5a210` is a placeholder).
// Shape: two dword parameters tested separately (`test eax ; jne ; test edx ; jne`); an `__int64 == 0` test compiles to
//   `mov esi,eax ; or esi,edx` with an esi save. The request word is a bit field (the two bits of the zero path merge
//   into one load/and/or/store, the single bit of the other path is `or dword ptr [ecx+2Ch],80h`).
// GameCODA voice model (no PDB/RTTI; names descriptive): the landed CCodaVoice (ctor 0x00C59BA0, vtable 0x012B5C10, 0xB4
//   bytes) and CSoftwareVoice (ctor 0x00C5B1A0, vtable 0x012B5C60).
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

class CSoftwareVoice : public CCodaVoice {          // vtable 0x012B5C60
public:
    virtual bool SetPositionB(unsigned long low, unsigned long high);   // slot 11: 0x00C5A210
};

bool CSoftwareVoice::SetPositionB(unsigned long low, unsigned long high)
{
    if (low == 0 && high == 0) {
        m_requests.m_bit7 = 0;
        m_requests.m_bit8 = 1;
        return true;
    }
    m_requests.m_bit7 = 1;
    m_positionBLow = low;
    m_positionBHigh = high;
    return false;
}