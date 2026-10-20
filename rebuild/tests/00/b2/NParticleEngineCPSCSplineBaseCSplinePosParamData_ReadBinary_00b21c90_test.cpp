#include <cstdio>
#include <cstring>
// Behaviour model of CSplinePosParamData::ReadBinary @ 0x00B21C90 (copy of the source's logic).
static int g_overflow = 0;
struct CDataInputStream {
    void* m_pVtable;
    unsigned long m_position;
    long m_08;
    const unsigned char* m_pData;
    long m_10;
    long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++g_overflow; std::memset(buffer, 0, size); }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned long)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    unsigned int ReadULong() { unsigned int value; Read(&value, 4); return value; }
    bool ReadBool() { unsigned char value; Read(&value, 1); return value != 0; }
};
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    T storage[16];
    T* begin() { return m_begin; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const T& value)
    {
        m_begin = storage; m_end = storage + n; m_capacity = storage + 16;
        for (unsigned long i = 0; i < n; ++i) storage[i] = value;
    }
};
struct CSplinePosParamData {
    void* PParentParticleSystem;
    void* Spline;
    bool  MaintainSplineShape;
    CArray<unsigned int> SplinePosParam;
    void ReadBinary(CDataInputStream& in)
    {
        MaintainSplineShape = in.ReadBool();
        unsigned long count = in.ReadULong();
        SplinePosParam.resize(count, 0);
        for (unsigned long i = 0; i < count; ++i)
            SplinePosParam[i] = (unsigned int)in.ReadULong();
    }
};
int main()
{
    unsigned char data[1 + 4 * 4];
    data[0] = 7;                                         // non-zero byte -> true
    unsigned int words[4] = { 3, 11, 22, 33 };
    std::memcpy(data + 1, words, sizeof(words));
    CDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.m_pData = data; s.m_remaining = sizeof(data);
    static CSplinePosParamData p; p.MaintainSplineShape = false;
    p.ReadBinary(s);
    int ok = p.MaintainSplineShape && p.SplinePosParam.m_end - p.SplinePosParam.m_begin == 3
          && p.SplinePosParam[0] == 11 && p.SplinePosParam[1] == 22 && p.SplinePosParam[2] == 33
          && s.m_remaining == 0 && s.m_position == 17 && g_overflow == 0;
    if (!ok) { std::printf("00b21c90_TEST FAIL\n"); return 1; }
    std::printf("00b21c90_TEST PASS\n");
    return 0;
}