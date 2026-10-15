// Behaviour test for 0x0081ace0 (vector TransferBinaryOut, 24-byte elements)
#include <cstdio>
struct CDataOutputStream {
    long m_longs[4]; int m_nLongs; const void* m_data; long m_bytes; int m_nWrites;
    CDataOutputStream() : m_nLongs(0), m_data(0), m_bytes(-1), m_nWrites(0) {}
    virtual void Write(const void* data, long bytes) { m_data = data; m_bytes = bytes; ++m_nWrites; }
    void WriteLong(long value) { m_longs[m_nLongs++] = value; }
};
struct SRecord { unsigned int m_words[6]; };
template <class T> struct CRecordVector {
    T* m_first; T* m_last; T* m_end;
    long size() const { return m_last - m_first; }
    const void* GetData(long count) const { return count ? (const void*)m_first : (const void*)this; }
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(size());
        long count = size();
        stream.Write(GetData(count), count * sizeof(T));
        stream.WriteLong(0x01AB6CBB);
    }
};
int main()
{
    SRecord recs[3];
    CRecordVector<SRecord> v; v.m_first = recs; v.m_last = recs + 3; v.m_end = recs + 3;
    CDataOutputStream s; v.TransferBinaryOut(s);
    bool ok = s.m_nLongs == 2 && s.m_longs[0] == 3 && s.m_longs[1] == 0x01AB6CBB && s.m_data == recs && s.m_bytes == 3 * 24;
    CRecordVector<SRecord> e; e.m_first = e.m_last = e.m_end = 0;
    CDataOutputStream t; e.TransferBinaryOut(t);
    ok = ok && t.m_longs[0] == 0 && t.m_data == &e && t.m_bytes == 0 && t.m_nWrites == 1;
    if (!ok) { std::printf("0081ace0_TEST FAIL\n"); return 1; }
    std::printf("0081ace0_TEST PASS\n");
    return 0;
}