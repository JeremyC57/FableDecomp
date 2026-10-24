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

struct C3DVector { float X, Y, Z; };
static int g_frame = 777;
static long GFGetCurrentFrame() { return g_frame; }
struct CThing { u64 UniqueID; };
struct CIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct COpinionDeed {
    int DeedType; int DeedHandle; C3DVector Position; int FrameHappened; CIntelligentPointer PTarget;
    bool WasSearchingForWitnesses, IsStillHappening, IsCrimeReportedToVillage, IsCrime; int ExtraData;
    long GetFrameHappened() const { return IsStillHappening ? GFGetCurrentFrame() : FrameHappened; }
};
struct COut3 : COut { void Write3DVector(const C3DVector& v) { add('v', (u64)(s64)(v.X + v.Y * 10 + v.Z * 100)); } };
struct TraitsPtr { void TransferBinaryOut(COut3* out, const CIntelligentPointer* value)
{
    u64 uid = 0;
    if (value->GetPItem() != 0) uid = value->GetPItem()->UniqueID;
    out->WriteUInt64(uid);
} };
struct Traits { void TransferBinaryOut(COut3* out, COpinionDeed* const* value)
{
    out->WriteLong((*value)->DeedType);
    out->WriteLong((*value)->DeedHandle);
    out->Write3DVector((*value)->Position);
    out->WriteLong((*value)->GetFrameHappened());
    out->WriteEBOOL((*value)->IsCrimeReportedToVillage);
    out->WriteEBOOL((*value)->IsCrime);
    out->WriteLong((*value)->ExtraData);
    TraitsPtr traits;
    traits.TransferBinaryOut(out, &(*value)->PTarget);
} };
int main()
{
    CThing target; target.UniqueID = 42;
    COpinionDeed d; d.DeedType = 5; d.DeedHandle = 6; d.Position.X = 1; d.Position.Y = 2; d.Position.Z = 3;
    d.FrameHappened = 100; d.PTarget.item = &target; d.WasSearchingForWitnesses = true; d.IsStillHappening = true;
    d.IsCrimeReportedToVillage = false; d.IsCrime = true; d.ExtraData = 9;
    COpinionDeed* pd = &d;
    COut3 out; Traits t; t.TransferBinaryOut(&out, &pd);
    CHECK(out.n == 8 && out.is(0, 'l', 5) && out.is(1, 'l', 6) && out.is(2, 'v', 321) && out.is(3, 'l', 777)
          && out.is(4, 'b', 0) && out.is(5, 'b', 1) && out.is(6, 'l', 9) && out.is(7, 'q', 42));
    d.IsStillHappening = false; d.PTarget.item = 0;
    COut3 out2; t.TransferBinaryOut(&out2, &pd);
    CHECK(out2.is(3, 'l', 100) && out2.is(7, 'q', 0));
    if (g_fail) { std::printf("006c5310_TEST FAIL\n"); return 1; }
    std::printf("006c5310_TEST PASS\n");
    return 0;
}