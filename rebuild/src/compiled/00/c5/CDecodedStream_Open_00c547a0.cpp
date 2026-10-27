// CDecodedStream::Open(const SStreamDesc_00c547a0* desc) -- slot 2: clear the flag byte at +4, take the descriptor's word at +0x58 into +0x220 and initialise the stream; 0 on success, -9 on failure
// Leaf: ?Open@CDecodedStream@@UAEJPBUSStreamDesc_00c547a0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C547A0, 35 bytes, thiscall `ret 4`.
// Identity: slot 2 of the CDecodedStream vtable 0x012B89C4 (the landed PumpBlock 0x00C546E0 is its slot 7, the scalar
//   deleting destructor 0x00C53AE0 slot 0). The initialiser 0x00C54510 (EH-framed) is unlanded. Descriptive (libpk,
//   no PDB/RTTI): method, descriptor and field names are not proven.
// Shape: `Initialise() ? 0 : -9` is `neg al ; sbb eax,eax ; and eax,9 ; add eax,-9`.
struct SStreamDesc_00c547a0 {
    unsigned char _pad[0x58];
    long m_58;                                  // +0x58
};

class CDecodedStream {                          // vtable 0x012B89C4
public:
    virtual void v00();
    virtual void v01();
    virtual long Open(const SStreamDesc_00c547a0* desc);   // slot 2
    bool Initialise();                          // 0x00C54510
    bool m_04;                                  // +0x04
    unsigned char _pad05[0x220 - 5];
    long m_220;                                 // +0x220
};

long CDecodedStream::Open(const SStreamDesc_00c547a0* desc)
{
    m_04 = false;
    m_220 = desc->m_58;
    return Initialise() ? 0 : -9;
}