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

struct C2DCoordI { int X, Y; };
struct CEngineLandscapeMap { int id; };
struct CMovableResource { int allocs; };
static unsigned char g_block[64];
struct CWaterPatchMesh {
    CEngineLandscapeMap* Map; int OffsetX, OffsetY; CIn* stream;
    CWaterPatchMesh(CEngineLandscapeMap* map, CIn* s, const C2DCoordI* offset) : Map(map), OffsetX(offset->X), OffsetY(offset->Y), stream(s) {}
    static void* operator new(size_t size, CMovableResource* owner) { if (!owner) return 0; ++owner->allocs; return g_block; }
    static void operator delete(void*, CMovableResource*) {}
    static CWaterPatchMesh* CreateFromStream(CMovableResource* owner, CEngineLandscapeMap* map, CIn* in)
    {
        int x = (int)in->ReadLong();
        int y = (int)in->ReadLong();
        C2DCoordI offset; offset.X = x; offset.Y = y;
        return new (owner) CWaterPatchMesh(map, in, &offset);
    }
};
int main()
{
    unsigned char buf[8]; put32(buf, 3); put32(buf + 4, 4);
    CIn in; in.Init(buf, 8, 8);
    CMovableResource owner; owner.allocs = 0; CEngineLandscapeMap map; map.id = 1;
    CWaterPatchMesh* m = CWaterPatchMesh::CreateFromStream(&owner, &map, &in);
    CHECK(m != 0 && m->Map == &map && m->OffsetX == 3 && m->OffsetY == 4 && m->stream == &in && owner.allocs == 1);
    if (g_fail) { std::printf("00bf5a30_TEST FAIL\n"); return 1; }
    std::printf("00bf5a30_TEST PASS\n");
    return 0;
}