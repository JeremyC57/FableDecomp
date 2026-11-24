// CDefClassBase::InternalSaveBinary(CDataOutputStream&) -- write the three flag bits (Setup, Template, DefaultValsApplied) as bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACAB0, length 74 bytes.
// Name fix (NF380): 0x00993E50 is CDataOutputStream::WriteUBYTE (retail has the UBYTE / SBYTE / CBYTE writers at 0x00993E50 / 60 / 70 in Ego_r and FableWin object order; the FableWin caller calls WriteUBYTE). Was WriteCBYTE(char).
// Callees: CDataOutputStream::WriteUBYTE(unsigned char) @ 0x00993E50 (three times). Virtual (vtable 0x01230BCC).
// Shape: each 1-bit member is widened with `xor eax,eax ; mov al,[esi+24h]`, shifted to bit 0 and masked with
// `and eax,0FFFFFF01h` (the char conversion of a bitfield), then pushed; the stream stays in edi.
class CDataOutputStream { public:
    void WriteUBYTE(unsigned char value);                   // 0x00993E50
};

class CDefClassBase { public:
    virtual void v00();
    unsigned char       _body[0x20];               // +0x04 .. +0x24: CResource links, PDefManager, GlobalIndex
    unsigned char       Setup : 1;                 // +0x24 bit 0
    unsigned char       Template : 1;              // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;    // +0x24 bit 2
    virtual void InternalSaveBinary(CDataOutputStream& stream);
};

void CDefClassBase::InternalSaveBinary(CDataOutputStream& stream)
{
    stream.WriteUBYTE(Setup);
    stream.WriteUBYTE(Template);
    stream.WriteUBYTE(DefaultValsApplied);
}