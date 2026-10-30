// CCodaVoice::Play(CAudioSound* sound) -- start a sound's single buffer on the voice: clear the start time and the buffer, hand the voice the sound's buffer (+0x78) with its loop bit, its format record (+0xF0), reset the play state and report 2
// Leaf: ?Play@CCodaVoice@@UAEJPAVCAudioObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C59720, 74 bytes, thiscall, `ret 4`.
// Identity: slot 7 (+0x1C) of the GameCODA voice base vtable 0x012B5C10 (agent441's CCodaVoice, ctor 0x00C59BA0) and of
//   the derived voice vtables (the eight `abs .rdata` refs: 0x012B5C2C ... 0x012BEE5C). Descriptive method name
//   (GameCODA has no PDB or RTTI). Label correction: the manifest row "CDataOutputStream::Write3DVector" is propagated.
// Callees (virtual): slot 18 (+0x48) 0x00C59880 SetBuffer (stores +0x0C, the buffer the landed CSoftwareVoice::StartSound
//   clears), slot 14 (+0x38, pure in the base) SetFormat, slot 17 (+0x44) 0x00C59860 Reset (the landed StartSound name).
// Layout: +0x0C buffer, +0x10 64-bit start time (the landed CAudioSource / CSoftwareVoice models, 4-byte packed);
//   CAudioSound flag word +0x10 (bit 8: loop), buffer +0x78, format record +0xF0.
// Shape: the loop bit is an `unsigned char` argument (`shr ecx,8 ; and ecx,0FFFFFF01h` = a byte `& 1` on the dword).
class CAudioObject { public:
    unsigned char  _pad00[0x10];
    unsigned long  m_flags;                             // +0x10 (bit 8: loop)
    unsigned char  _pad14[0x78 - 0x14];
    void*          m_pBuffer;                           // +0x78
    unsigned char  _pad7c[0xf0 - 0x7c];
    void*          m_pFormat;                           // +0xF0
};
#pragma pack(push, 4)
class CCodaVoice {                                      // vtable 0x012B5C10 (19 slots)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual long Play(CAudioObject* sound);              // slot 7 (+0x1C)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13();
    virtual void SetFormat(void* format);               // slot 14 (+0x38)
    virtual void v15(); virtual void v16();
    virtual void Reset();                               // slot 17 (+0x44): 0x00C59860
    virtual void SetBuffer(void* buffer, unsigned char loop);   // slot 18 (+0x48): 0x00C59880
    unsigned char _pad04[8];
    void*         m_pBuffer;                            // +0x0C
    __int64       m_startTime;                          // +0x10
};
#pragma pack(pop)

long CCodaVoice::Play(CAudioObject* sound)
{
    m_startTime = 0;
    m_pBuffer = 0;
    SetBuffer(sound->m_pBuffer, (sound->m_flags >> 8) & 1);
    SetFormat(sound->m_pFormat);
    Reset();
    return 2;
}