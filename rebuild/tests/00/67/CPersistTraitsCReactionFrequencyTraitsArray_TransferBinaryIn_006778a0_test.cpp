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

struct CTraits { int type; int payload; virtual ~CTraits() {} virtual void TransferBinaryIn(CIn* in) { payload = (int)in->ReadLong(); } };
static CTraits g_made[4]; static int g_nmade = 0;
static CTraits* CreateFromType(int type) { if (type == 9) return 0; g_made[g_nmade].type = type; return &g_made[g_nmade++]; }
struct CArr { CTraits* LowHigh[2][79]; };
struct Traits { void TransferBinaryIn(CIn* in, CArr* value)
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 79; ++j) {
            CTraits* traits = 0;
            long type = in->ReadLong();
            if (type != 0) {
                traits = CreateFromType((int)type);
                if (traits != 0) traits->TransferBinaryIn(in);
            }
            value->LowHigh[i][j] = traits;
        }
} };
int main()
{
    static unsigned char buf[4 * 170]; memset(buf, 0, sizeof(buf));
    // slot [0][1] = type 4 with payload 44; slot [1][0] = type 9 (no traits); everything else 0
    put32(buf + 4, 4); put32(buf + 8, 44); put32(buf + 4 * (79 + 1), 9);
    CIn in; in.Init(buf, sizeof(buf), sizeof(buf));
    static CArr arr; memset(&arr, 0xff, sizeof(arr));
    Traits t; t.TransferBinaryIn(&in, &arr);
    CHECK(arr.LowHigh[0][0] == 0 && arr.LowHigh[0][1] == &g_made[0] && g_made[0].type == 4 && g_made[0].payload == 44);
    CHECK(arr.LowHigh[1][0] == 0 && arr.LowHigh[1][78] == 0 && g_nmade == 1 && in.StreamPos == 4 * 159);
    if (g_fail) { std::printf("006778a0_TEST FAIL\n"); return 1; }
    std::printf("006778a0_TEST PASS\n");
    return 0;
}