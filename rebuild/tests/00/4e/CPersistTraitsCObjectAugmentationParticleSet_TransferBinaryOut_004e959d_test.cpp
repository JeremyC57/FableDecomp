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

struct CArrayL { int* first; int* last; int* end; };
static void GFSerialiseVectorBinaryOut(COut* out, const CArrayL* v)
{
    long n = (long)(v->last - v->first);
    out->WriteLong(n);
    for (long i = 0; i < n; ++i) out->WriteLong(v->first[i]);
}
struct CObjectAugmentationParticleSet { float PeriodSecs; int ParticleEffect, ParticleEffectToBlendTo, AttackParticleEffect, AttackParticleEffectToBlendTo; CArrayL StartPoint, EndPoint; };
struct Traits { void TransferBinaryOut(COut* out, const CObjectAugmentationParticleSet* value)
{
    out->WriteFloat(value->PeriodSecs);
    out->WriteLong(value->ParticleEffect);
    out->WriteLong(value->ParticleEffectToBlendTo);
    out->WriteLong(value->AttackParticleEffect);
    out->WriteLong(value->AttackParticleEffectToBlendTo);
    GFSerialiseVectorBinaryOut(out, &value->StartPoint);
    GFSerialiseVectorBinaryOut(out, &value->EndPoint);
} };
int main()
{
    int s[2] = { 5, 6 };
    CObjectAugmentationParticleSet p; p.PeriodSecs = 1.5f; p.ParticleEffect = 1; p.ParticleEffectToBlendTo = 2;
    p.AttackParticleEffect = 3; p.AttackParticleEffectToBlendTo = 4;
    p.StartPoint.first = s; p.StartPoint.last = s + 2; p.StartPoint.end = s + 2;
    p.EndPoint.first = 0; p.EndPoint.last = 0; p.EndPoint.end = 0;
    COut out; Traits t; t.TransferBinaryOut(&out, &p);
    CHECK(out.n == 9 && out.is(0, 'f', 1500) && out.is(4, 'l', 4) && out.is(5, 'l', 2) && out.is(6, 'l', 5) && out.is(7, 'l', 6) && out.is(8, 'l', 0));
    if (g_fail) { std::printf("004e959d_TEST FAIL\n"); return 1; }
    std::printf("004e959d_TEST PASS\n");
    return 0;
}