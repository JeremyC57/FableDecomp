#include <cstdio>
#include <cstring>
struct CDataOutputStream {
    unsigned char m_buf[256];
    unsigned long m_size;
    CDataOutputStream() : m_size(0) {}
    virtual ~CDataOutputStream() {}
    void WriteULONG(unsigned long value) { unsigned int v = (unsigned int)value; std::memcpy(m_buf + m_size, &v, 4); m_size += 4; }
    void WriteData(const void* data, unsigned long size) { std::memcpy(m_buf + m_size, data, size); m_size += size; }
};
struct CMemoryDataOutputStream : CDataOutputStream {
    unsigned long GetSize() { return m_size; }
    void* PeekData() { return m_buf; }
};
struct CParticleProgram {
    void WriteToFile(CDataOutputStream& out) { out.WriteData("prog", 4); out.WriteULONG(9); }
};
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; T* operator->() const { return m_pObject; } };
struct C3DClothPrimitive {
    CCountedPointer<CParticleProgram> ParticleProgram;
    unsigned long PrimitiveIndex;
    unsigned long MaterialIndex;
    void SaveBinary(CDataOutputStream& out) const
    {
        out.WriteULONG(PrimitiveIndex);
        out.WriteULONG(MaterialIndex);
        CMemoryDataOutputStream stream;
        ParticleProgram->WriteToFile(stream);
        out.WriteULONG(stream.GetSize());
        out.WriteData(stream.PeekData(), stream.GetSize());
    }
};
int main()
{
    CParticleProgram prog;
    C3DClothPrimitive c; c.ParticleProgram.m_pObject = &prog; c.ParticleProgram.m_pCount = 0; c.PrimitiveIndex = 3; c.MaterialIndex = 5;
    CDataOutputStream out;
    c.SaveBinary(out);
    unsigned int w[4]; std::memcpy(w, out.m_buf, 12);
    int ok = out.m_size == 20 && w[0] == 3 && w[1] == 5 && w[2] == 8 && std::memcmp(out.m_buf + 12, "prog", 4) == 0;
    std::memcpy(w, out.m_buf + 16, 4);
    ok = ok && w[0] == 9;
    if (!ok) { std::printf("00ac3b00_TEST FAIL\n"); return 1; }
    std::printf("00ac3b00_TEST PASS\n");
    return 0;
}