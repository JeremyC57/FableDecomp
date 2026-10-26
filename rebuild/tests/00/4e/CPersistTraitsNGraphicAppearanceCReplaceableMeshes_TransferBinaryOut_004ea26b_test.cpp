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

#pragma pack(push, 1)
class CEngineGraphic { public: unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T* data() const { return size() > 0 ? _M_start : (const T*)this; }
};
struct CReplaceableMeshes { CArray<CEngineGraphic> Vector; };
struct Traits { void TransferBinaryOut(COut* out, const CReplaceableMeshes* value)
{
    out->WriteLong((long)value->Vector.size());
    out->Write(value->Vector.data(), value->Vector.size() * sizeof(CEngineGraphic));
} };
int main()
{
    CHECK(sizeof(CEngineGraphic) == 14);
    CEngineGraphic g[2]; memset(g, 0, sizeof(g)); g[0].BankIndex = 5; g[1].Type = 9;
    CReplaceableMeshes m; m.Vector._M_start = g; m.Vector._M_finish = g + 2; m.Vector._M_end_of_storage = g + 2;
    COut out; Traits t; t.TransferBinaryOut(&out, &m);
    CHECK(out.n == 2 && out.is(0, 'l', 2) && out.is(1, 'w', 28) && out.raw[0] == 5 && out.raw[27] == 9);
    CReplaceableMeshes e; e.Vector._M_start = 0; e.Vector._M_finish = 0; e.Vector._M_end_of_storage = 0;
    COut out2; t.TransferBinaryOut(&out2, &e);
    CHECK(out2.n == 2 && out2.is(0, 'l', 0) && out2.is(1, 'w', 0));
    if (g_fail) { std::printf("004ea26b_TEST FAIL\n"); return 1; }
    std::printf("004ea26b_TEST PASS\n");
    return 0;
}