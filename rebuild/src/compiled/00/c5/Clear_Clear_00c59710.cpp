// CCodaVoice::Stop() -- vtable slot 2 of the GameCODA voice base (0x012B5C10): set the request word at +0x2C to the stopped bit (bit 31) alone
// Leaf: ?Stop@CCodaVoice@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C59710, 8 bytes, thiscall, `mov dword ptr [ecx+2Ch],80000000h ; ret`.
// Identity (landing 2026-11-03g, chunk 736 NAME_FIXES): slot 2 (+0x08) of the CCodaVoice vtable 0x012B5C10 (its only data
//   reference, 0x012B5C18; ctor 0x00C59BA0), the qualified base call that CSoftwareVoice::Stop 0x00C5A1F0 ends with (`jmp`),
//   and the call the base destructor 0x00C3F3D0 makes while the stopped bit is clear. GameCODA has no PDB / RTTI: class and
//   method names are descriptive (the landed CCodaVoice / CSoftwareVoice family). Was `C_00c59710::M` (Leaf `Clear`).
class CCodaVoice {                                      // vtable 0x012B5C10, 0xB4 bytes (ctor 0x00C59BA0)
public:
    virtual ~CCodaVoice();                              // slot 0
    virtual long v01();                                 // slot 1
    virtual void Stop();                                // slot 2: 0x00C59710
    unsigned char _pad04[0x2c - 4];
    unsigned long m_requests;                           // +0x2c pending-change bits (bit 31: stopped; landed 0x00C597B0 model)
};

void CCodaVoice::Stop()
{
    m_requests = 0x80000000;
}
