// Behaviour test for 0x00612D30 GFSerialiseVectorBinaryOut<unsigned __int64>(stream, vector): write the element count as a
// signed long (WriteSLONG 0x00993EB0), then every 64-bit value through CPersistTraits (the 8-byte writer 0x00993EC0).
#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct CDataOutputStreamWriter {
    int  m_ops;
    long m_longs[8];
    u64  m_quads[8];
    int  m_nLongs, m_nQuads;
    char m_order[16];
    CDataOutputStreamWriter() : m_ops(0), m_nLongs(0), m_nQuads(0) { }
    void WriteSLONG(long value) { m_longs[m_nLongs++] = value; m_order[m_ops++] = 'L'; }
    void WriteUINT64(u64 value) { m_quads[m_nQuads++] = value; m_order[m_ops++] = 'Q'; }
};

namespace stl {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<u64> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const u64* value) const { stream.WriteUINT64(*value); }
};

template <class T>
void GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, stl::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = (long)value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

int main()
{
    int ok = 1;
    u64 ids[3];
    ids[0] = 1; ids[1] = ((u64)0x12345678 << 32) | 0x9abcdef0UL; ids[2] = ~(u64)0;
    stl::vector<u64> v;
    v._M_start = ids; v._M_finish = ids + 3; v._M_end_of_storage = ids + 3;
    CDataOutputStreamWriter w;
    GFSerialiseVectorBinaryOut(w, v);
    if (w.m_ops != 4 || w.m_order[0] != 'L' || w.m_order[1] != 'Q' || w.m_order[3] != 'Q' || w.m_longs[0] != 3 ||
        w.m_quads[0] != ids[0] || w.m_quads[1] != ids[1] || w.m_quads[2] != ids[2]) ok = 0;
    stl::vector<u64> e;
    e._M_start = e._M_finish = e._M_end_of_storage = 0;
    CDataOutputStreamWriter we;
    GFSerialiseVectorBinaryOut(we, e);
    if (we.m_ops != 1 || we.m_longs[0] != 0) ok = 0;
    if (!ok) { std::printf("00612d30_TEST FAIL\n"); return 1; }
    std::printf("00612d30_TEST PASS\n");
    return 0;
}