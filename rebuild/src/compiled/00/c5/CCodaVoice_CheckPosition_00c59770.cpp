// CCodaVoice::CheckPosition(__int64 position) -- vtable slot 10: mark the voice (flag bit 0 of +0x28); a position at or past the 64-bit end mark (+0x10) clears the +0x2C bit 0x100 and returns 2, an earlier one returns 0x10
// Leaf: ?CheckPosition@CCodaVoice@@UAEJ_J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59770, 54 bytes (+ int3 padding), thiscall `ret 8`.
// Name: descriptive (the manifest label `_gapscan::sub_00c59770` is a placeholder; no caller other than the vtables).
// Shape: the 64-bit signed compare (`cmp high ; jl ; jg ; cmp low ; jb`) is a signed __int64; the `>=` arm falls through.
// GameCODA voice model (no PDB/RTTI; names descriptive): the landed CCodaVoice (ctor 0x00C59BA0, vtable 0x012B5C10, 0xB4
//   bytes; flags +0x28, the +0x2C word 0x80000000 after the ctor). Slots 8, 9, 11, 13, 14 of the base vtable are
//   _purecall (0x00BFEA08); slots 10 and 12 (0x00C59770 / 0x00C597B0) are shared unchanged by every derived voice vtable
//   (0x012B5C60 software, 0x012B5CD0, 0x012B5D70 stream, 0x012B8AE0, 0x012B8CC8, 0x012B8D48, 0x012BEE40 OpenAL).
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
    virtual bool SetPositionB(__int64 position) = 0;   // slot 11: pure here (0x00C5A210 software, 0x00C727B0 OpenAL)
    virtual void SetPositionC(__int64 position);    // slot 12: 0x00C597B0
    unsigned char  _pad08[8];                       // (VC7.1 pads the vfptr to 8 in a class holding an __int64)
    __int64        m_endPosition;                   // +0x10
    __int64        m_positionB;                     // +0x18
    __int64        m_positionC;                     // +0x20
    SVoiceFlags    m_flags;                         // +0x28
    SVoiceRequests m_requests;                      // +0x2c
};

long CCodaVoice::CheckPosition(__int64 position)
{
    m_flags.m_bit0 = 1;
    if (position >= m_endPosition) {
        m_requests.m_bit8 = 0;
        return 2;
    }
    return 0x10;
}