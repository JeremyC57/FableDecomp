// Behaviour test for CReactionMatch_Sphere::TransferBinaryOut @ 0x007003A0: type, count, raw opinion floats, squared radius.
#include <cstdio>
#include <cstring>

struct CDataOutputStream {
    long longs[8]; int nl; float floats[8]; int nf; unsigned char bytes[64]; unsigned long nb; const void* lastData;
    CDataOutputStream() : nl(0), nf(0), nb(0), lastData(0) {}
    virtual void WriteData(const void* data, unsigned long size) { lastData = data; std::memcpy(bytes + nb, data, size); nb += size; }
    void WriteLong(long v) { longs[nl++] = v; }
    void WriteFloat(float v) { floats[nf++] = v; }
};

template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T* GetList() const { return Size() > 0 ? m_begin : (const T*)this; }
};

struct COpinionSet {
    CArray<float> OpinionValues;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(OpinionValues.Size());
        stream.WriteData(OpinionValues.GetList(), OpinionValues.Size() * sizeof(float));
    }
};

struct CReactionMatch_Sphere {
    long ReactionType; COpinionSet Centre; float SqRadius;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(ReactionType);
        Centre.TransferBinaryOut(stream);
        stream.WriteFloat(SqRadius);
    }
};

int main()
{
    float opinions[3] = { 0.5f, -1.0f, 2.0f };
    CReactionMatch_Sphere m;
    m.ReactionType = 4; m.SqRadius = 6.25f;
    m.Centre.OpinionValues.m_begin = opinions; m.Centre.OpinionValues.m_end = opinions + 3; m.Centre.OpinionValues.m_capacity = opinions + 3;
    CDataOutputStream s;
    m.TransferBinaryOut(s);
    float back[3];
    std::memcpy(back, s.bytes, sizeof(back));
    if (s.nl != 2 || s.longs[0] != 4 || s.longs[1] != 3 || s.nb != 12 || back[1] != -1.0f || back[2] != 2.0f || s.nf != 1 || s.floats[0] != 6.25f) {
        std::printf("007003a0_TEST FAIL\n"); return 1;
    }
    // empty set: zero count, zero bytes, the data pointer is the array object itself
    m.Centre.OpinionValues.m_end = opinions;
    CDataOutputStream e;
    m.TransferBinaryOut(e);
    if (e.longs[1] != 0 || e.nb != 0 || e.lastData != (const void*)&m.Centre.OpinionValues) { std::printf("007003a0_TEST FAIL\n"); return 1; }
    std::printf("007003a0_TEST PASS\n");
    return 0;
}
