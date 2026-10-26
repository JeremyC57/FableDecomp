// CDataInputStream::ReadMatrix3x4() -- read twelve floats (X/Y/Z axes then position) and return the 3x4 matrix by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995340, length 1072 bytes (thiscall, hidden result pointer, ret 4).
// Label correction: the manifest's COpinionPersonalityTraits::TransferBinaryIn is propagated; the landed caller
// C3DMeshFileDummyObjectChunk::ReadFromFile 0x00AA9E60 names it CDataInputStream::ReadMatrix3x4 (also called from
// C3DGeneratorObject::LoadBinary 0x00A2E5B0).
// Shape: twelve inlined CMemoryDataInputStream::Read (64-bit position range check, chunk fast path, overflow reader
// 0x00993CA0) into one float temp, each stored into the local matrix; the local is copied out with rep movsd.
// Leaf: ReadMatrix3x4@CDataInputStream
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

class CDataInputStream { public:
    virtual void v0();
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    CMatrix3x4 ReadMatrix3x4();
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

CMatrix3x4 CDataInputStream::ReadMatrix3x4()
{
    CMatrix3x4 m;
    m.XAxis.X = ReadFloat();
    m.XAxis.Y = ReadFloat();
    m.XAxis.Z = ReadFloat();
    m.YAxis.X = ReadFloat();
    m.YAxis.Y = ReadFloat();
    m.YAxis.Z = ReadFloat();
    m.ZAxis.X = ReadFloat();
    m.ZAxis.Y = ReadFloat();
    m.ZAxis.Z = ReadFloat();
    m.Position.X = ReadFloat();
    m.Position.Y = ReadFloat();
    m.Position.Z = ReadFloat();
    return m;
}