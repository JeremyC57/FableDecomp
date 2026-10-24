#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 s64;
typedef unsigned __int64 u64;
#else
typedef long long s64;
typedef unsigned long long u64;
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)

// Recording output stream: every Write* call appends (tag, value).
struct LogEntry { char tag; u64 v; };
struct COut {
    LogEntry e[400];
    int n;
    unsigned char raw[512];
    int rawn;
    COut() : n(0), rawn(0) {}
    void add(char t, u64 v) { e[n].tag = t; e[n].v = v; ++n; }
    void WriteEBOOL(bool v) { add('b', v ? 1 : 0); }
    void WriteLong(long v) { add('l', (u64)(s64)v); }
    void WriteULong(unsigned long v) { add('u', (u64)v); }
    void WriteFloat(float f) { add('f', (u64)(s64)(f * 1000.0f)); }
    void WriteUInt64(u64 v) { add('q', v); }
    void Write(const void* data, unsigned long size) { add('w', size); memcpy(raw + rawn, data, size); rawn += (int)size; }
    bool is(int i, char t, u64 v) const { return i < n && e[i].tag == t && e[i].v == v; }
};

// Memory input stream with the retail inline Read (64-bit range check, chunk fast path, overflow slow path).
struct CIn {
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
    const unsigned char* base;
    int overflowCalls;
    void Init(const unsigned char* data, long size, long chunk)
    {
        base = data; StreamPos = 0; StreamSize = size; CurrentSourcePtr = data; SourceChunkStreamPos = 0;
        SourceChunkBytesRemaining = chunk; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        memcpy(buffer, base + StreamPos, size);
        StreamPos += size;
        CurrentSourcePtr = base + StreamPos;
        SourceChunkBytesRemaining = StreamSize - (long)StreamPos;
    }
    static bool IsValidPosition(s64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        s64 newPosition = (s64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { int v = 0; Read(&v, 4); return v; }
    unsigned long ReadULong() { unsigned int v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    unsigned char ReadByte() { unsigned char v = 0; Read(&v, 1); return v; }
};
static void put32(unsigned char* p, int v) { memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { memcpy(p, &v, 4); }

struct SPOD { int _words[6]; };
static SPOD g_storage[8];
static int g_fillZero = 0;
struct CPODVector {
    SPOD* _M_start; SPOD* _M_finish; SPOD* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    SPOD* data() { return size() > 0 ? _M_start : (SPOD*)this; }
    void resize(unsigned long count, const SPOD& value)
    {
        g_fillZero = 1; for (int i = 0; i < 6; ++i) if (value._words[i] != 0) g_fillZero = 0;
        for (unsigned long i = 0; i < count; ++i) g_storage[i] = value;
        _M_start = g_storage; _M_finish = g_storage + count; _M_end_of_storage = _M_finish;
    }
    void TransferBinaryIn(CIn* in)
    {
        unsigned int count;
        in->Read(&count, sizeof(count));
        resize(count, SPOD());
        SPOD* elements = data();
        long bytes = (long)(size() * sizeof(SPOD));
        if (bytes > 0)
            in->Read(elements, bytes);
    }
};
int main()
{
    unsigned char buf[4 + 48]; memset(buf, 0, sizeof(buf)); put32(buf, 2); put32(buf + 4, 21); put32(buf + 48, 22);
    CIn in; in.Init(buf, sizeof(buf), 10);
    CPODVector v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.TransferBinaryIn(&in);
    CHECK(v.size() == 2 && g_fillZero == 1 && g_storage[0]._words[0] == 21 && g_storage[1]._words[5] == 22 && in.overflowCalls == 1 && in.StreamPos == 52);
    if (g_fail) { std::printf("0046fee9_TEST FAIL\n"); return 1; }
    std::printf("0046fee9_TEST PASS\n");
    return 0;
}