// CCodaVoice::Reset() -- vtable slot 17 of the GameCODA voice base (0x012B5C10): zero the position words +0x18..+0x24 and the request word +0x2C, set +0x28 to 1
// Leaf: ?Reset@CCodaVoice@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C59860, thiscall, plain `ret`.
// Identity (landing 2026-11-03g, chunk 736 NAME_FIXES): slot 17 (+0x44) of the CCodaVoice vtable 0x012B5C10 (data reference
//   0x012B5C54; also slot 17 of the derived tables 0x012B8CC8 / 0x012B8D48 / 0x012BEE40); CCodaVoice::Play 0x00C59720 calls
//   the slot as Reset, and CSoftwareVoice::Reset 0x00C5A2A0 calls this body first as its qualified base Reset. GameCODA has
//   no PDB / RTTI: names descriptive. Was `CDataInputStream::Close` (a real engine class name on a GameCODA address).
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
    unsigned char _pad04[0x18 - 4];
    unsigned long m_positionB[2];                       // +0x18 64-bit position as two words (landed 0x00C597B0 model: __int64;
    unsigned long m_positionC[2];                       // +0x20  word stores keep retail's +0x18, +0x1C, +0x20, +0x24 order)
    unsigned long m_flags;                              // +0x28 state flags
    unsigned long m_requests;                           // +0x2c pending-change bits (bit 31: stopped)
};

void CCodaVoice::Reset()
{
    m_positionB[0] = 0;
    m_positionB[1] = 0;
    m_positionC[0] = 0;
    m_positionC[1] = 0;
    m_requests = 0;
    m_flags = 1;
}
