#pragma optimize("s", on)
// CExplosionRing::TransferBinaryOut(CDataOutputStream* stream) -- write the ring's count byte (+4) then its three floats (+0, +8, +0xc) to the stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB1D9, length 61 bytes.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the word pushed straight from memory), WriteFloat @ 0x00993EE0 three times (each float loaded through the x87
// and stored into the reserved slot: the values come from inline float getters). Shape: the size-optimised module (`push dword ptr [esi+4]`), the stream in edi,
// `ret 4`.
class CDataOutputStream { public:
    void WriteFloat(float value);                  // 0x00993EE0
public:
    void WriteSLONG(long);   // 0x00993EB0
};

class CExplosionRing { public:
    float m_radius;                                // +0x00
    long  m_count;                                 // +0x04
    float m_height;                                // +0x08
    float m_speed;                                 // +0x0c
    float GetRadius() const { return m_radius; }   // inline getters: the values travel through the x87 into the pushed slots
    float GetHeight() const { return m_height; }
    float GetSpeed() const { return m_speed; }
    void TransferBinaryOut(CDataOutputStream* stream);
};

void CExplosionRing::TransferBinaryOut(CDataOutputStream* stream)
{
    stream->WriteSLONG(m_count);
    stream->WriteFloat(GetRadius());
    stream->WriteFloat(GetHeight());
    stream->WriteFloat(GetSpeed());
}