// Behaviour test for 00f28610 CPersistContext::Transfer(name, vector<CWaveRecord38>&) ("Waves"): binary save writes the count then
// transfers each element, binary load reads the count (inline chunked ReadLong, overflow helper when the chunk is short),
// resizes and transfers each element, mode 4 transfers each element only, text load hands the vector to the text helper.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __assume(x)
#endif
struct CPersistContext;
struct CMemoryDataInputStream {
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    long overflowReads;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++overflowReads; int v = 2; std::memcpy(buffer, &v, size); m_position += size; }
    void Read(void* buffer, long size)
    {
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { int value; Read(&value, 4); return value; }
};
struct CDataOutputStream { long written; void WriteLong(long v) { written = v; } };
struct CWaveRecord38 { int transfers; unsigned char body[0x38 - 8]; CWaveRecord38() : transfers(0) {} void Transfer(CPersistContext&) { ++transfers; } };
static CWaveRecord38 g_store[16];
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count) { _M_finish = _M_start + count; }
};
struct CPersistContext_TransferableVectorGenericType { virtual long GetCount() { return -1; } virtual ~CPersistContext_TransferableVectorGenericType() {} };
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount() { return TransVector->size(); }
};
struct CPersistContext {
    long Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream; int crc; long textCount;
    void CheckCRC(const char* name) { if (std::strcmp(name, "Waves") == 0) ++crc; }
    void TransferSubComponentVectorLoadText(const char*, CPersistContext_TransferableVectorGenericType& item) { textCount = item.GetCount(); }
    template <class T> void Transfer(const char* name, vector<T>& value);
};
template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4: { long count = value.size(); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
    case 3: { long count = value.size(); PSaveStream->WriteLong(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
    case 2: { long count = PLoadStream->ReadLong(); value.resize(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
    case 0: { CPersistContext_TransferableVector<T> item(value); TransferSubComponentVectorLoadText(name, item); break; }
    default: __assume(0);
    }
}
int main()
{
    vector<CWaveRecord38> v; v._M_start = g_store; v._M_finish = g_store + 3; v._M_end_of_storage = g_store + 16;
    int five = 5; unsigned char buf[4]; std::memcpy(buf, &five, 4);
    CMemoryDataInputStream in; in.m_position = 0; in.m_pData = buf; in.m_remaining = 4; in.overflowReads = 0;
    CDataOutputStream out; out.written = -1;
    CPersistContext c; c.Mode = 3; c.PLoadStream = &in; c.PSaveStream = &out; c.crc = 0; c.textCount = -1;
    c.Transfer("Waves", v);
    int ok = out.written == 3 && g_store[0].transfers == 1 && g_store[2].transfers == 1 && g_store[3].transfers == 0;
    c.Mode = 2; c.Transfer("Waves", v);
    if (v.size() != 5 || g_store[4].transfers != 1 || g_store[0].transfers != 2 || in.m_position != 4 || in.m_remaining != 0) ok = 0;
    c.Transfer("Waves", v);   /* chunk exhausted: the overflow helper supplies 2 */
    if (in.overflowReads != 1 || v.size() != 2 || g_store[0].transfers != 3 || g_store[2].transfers != 2) ok = 0;
    c.Mode = 4; c.Transfer("Waves", v);
    if (g_store[1].transfers != 4 || g_store[4].transfers != 1) ok = 0;
    c.Mode = 0; c.Transfer("Waves", v);
    if (c.textCount != 2 || c.crc != 5) ok = 0;
    if (!ok) { std::printf("00f28610_TEST FAIL\n"); return 1; }
    std::printf("00f28610_TEST PASS\n");
    return 0;
}