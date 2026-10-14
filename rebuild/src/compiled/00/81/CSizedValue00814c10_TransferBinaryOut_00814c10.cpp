// CSizedValue_00814c10::TransferBinaryOut(CDataOutputStream* stream) -- write the count at +4 as a byte (0x00993EB0) and the float at +8 (0x00993EE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00814C10, length 35 bytes.
// Callees: CDataOutputStream::WriteCBYTE @ 0x00993EB0, WriteFloat @ 0x00993EE0 (the float pushed as a word). Shape: the stream kept in edi, `ret 4`.
// The chunk label "CGraphicDataBank::ChangeScene" is propagated; named by address.
struct CDataOutputStream {
    void WriteCBYTE(long value);                   // 0x00993EB0
    void WriteFloat(float value);                  // 0x00993EE0
};

struct CSizedValue_00814c10 {
    long  m_0;
    long  m_count;                                 // +0x04
    float m_value;                                 // +0x08
    void TransferBinaryOut(CDataOutputStream* stream);
};

void CSizedValue_00814c10::TransferBinaryOut(CDataOutputStream* stream)
{
    stream->WriteCBYTE(m_count);
    stream->WriteFloat(m_value);
}