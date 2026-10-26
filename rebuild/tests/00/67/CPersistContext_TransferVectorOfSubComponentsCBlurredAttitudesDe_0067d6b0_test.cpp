#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

#ifndef _MSC_VER
typedef long long i64;
#else
typedef __int64 i64;
#endif
class CPersistContext;
class CMemoryDataInputStream { public:
    void* m_pVtable; unsigned long m_position; long StreamSize; const unsigned char* m_pData; long SourceChunkStreamPos; long m_remaining;
    long overflow;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { LOG("overflow"); long v = 3; std::memcpy(buffer, &v, 4); overflow++; }
    static bool IsValidPosition(i64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        i64 newPosition = (i64)(m_position + size);
        if (!IsValidPosition(newPosition)) return;
        if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { long value = 0; Read(&value, 4); return value; }   /* 4-byte stream long (host long may be wider) */
};
class CDataOutputStream { public: long written; void WriteLong(long v) { LOG("write"); written = v; } };
static int g_transfers;
class CBlurredAttitudesDef { public: long id; virtual void Transfer(CPersistContext&) { g_transfers += id; } virtual ~CBlurredAttitudesDef() {} };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count) { LOG("resize"); _M_finish = _M_start + count; }
};
class CPersistContext { public:
    long Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { LOG(name[0] == 0 ? "crc" : "crc?"); }
    template <class T> void TransferVectorOfSubComponents(vector<T>& value);
};
template <class T>
void CPersistContext::TransferVectorOfSubComponents(vector<T>& value)
{
    CheckCRC("");
    switch (Mode) {
    case 2: { long count = PLoadStream->ReadLong(); value.resize(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
    default: { long count = value.size(); PSaveStream->WriteLong(count); for (long i = 0; i < count; ++i) value[i].Transfer(*this); break; }
    }
}
int main()
{
    CBlurredAttitudesDef e[4]; for (int i = 0; i < 4; ++i) e[i].id = 1 << i;
    vector<CBlurredAttitudesDef> v; v._M_start = e; v._M_finish = e + 3; v._M_end_of_storage = e + 4;
    CDataOutputStream out = { 0 };
    long two = 2; CMemoryDataInputStream in; std::memset(&in, 0, sizeof(in)); in.m_pData = (const unsigned char*)&two; in.m_remaining = 4;
    CPersistContext c; c.PLoadStream = &in; c.PSaveStream = &out;
    c.Mode = 3; g_log[0] = 0; g_transfers = 0; c.TransferVectorOfSubComponents(v);
    if (std::strcmp(g_log, "crc;write;") || out.written != 3 || g_transfers != 7) { std::printf("0067d6b0_TEST FAIL\n"); return 1; }
    c.Mode = 2; g_log[0] = 0; g_transfers = 0; c.TransferVectorOfSubComponents(v);
    if (std::strcmp(g_log, "crc;resize;") || v.size() != 2 || g_transfers != 3 || in.m_position != 4 || in.m_remaining != 0) { std::printf("0067d6b0_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_transfers = 0; c.TransferVectorOfSubComponents(v);   /* chunk exhausted: overflow read gives 3 */
    if (std::strcmp(g_log, "crc;overflow;resize;") || v.size() != 3 || g_transfers != 7) { std::printf("0067d6b0_TEST FAIL\n"); return 1; }
    std::printf("0067d6b0_TEST PASS\n");
    return 0;
}