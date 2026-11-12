// Behaviour test for 0x00A4C5E0 C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream&, CSimpleMemoryPool&): a host
// model of the reader over an in-memory stream (bounds-checked inline Read with a chunk-overflow fallback) and a bump pool
// (AllocateType: 0 for a zero count). Checks the bone name copy, the scalar fields, the bit fields, the four tracks, the stream
// position and the identity flag for an animated and an identity sequence.
#include <cstdio>
#include <cstring>

struct Stream {
    const unsigned char* Data; long Size; long Pos; int Overflows;
    void ReadOverflow(unsigned char* out, long size) { ++Overflows; std::memcpy(out, Data + Pos, size); Pos += size; }
    void Read(void* out, long size) {
        if (size <= 0) return;
        if (Pos + size < 0 || Pos + size > 0x7fffffff) return;
        if (size <= Size - Pos && size <= 8) { std::memcpy(out, Data + Pos, size); Pos += size; }   // small reads: fast path
        else ReadOverflow((unsigned char*)out, size);                                               // large reads: chunk path
    }
    unsigned char ReadUBYTE() { unsigned char v; Read(&v, 1); return v; }
    unsigned short ReadUWORD() { unsigned short v; Read(&v, 2); return v; }
    unsigned int ReadULONG() { unsigned int v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
    bool ReadEBOOL() { return ReadUBYTE() != 0; }
    const char* ReadNullTerminatedString() { const char* s = (const char*)Data + Pos; Pos += (long)std::strlen(s) + 1; return s; }
};
struct Pool {
    unsigned char Buffer[1024]; unsigned int Used;
    Pool() : Used(0) {}
    template<class T> T* AllocateType(unsigned int count) {
        if (count == 0) return 0;
        T* p = (T*)(Buffer + Used); Used += count * sizeof(T); return p;
    }
};
struct Quat { float X, Y, Z, W; };
struct VecWord { short X, Y, Z; };
struct SeqData {
    char* BoneName; float SamplesPerSecond; unsigned short FrameCount;
    unsigned char BoneType, Enabled, Animated, Identity, RotationMode, PositionMode, ScalingMode;
    float PositionFactor, ScalingFactor; Quat* RotationTrack;
    unsigned short RotationTrackSize, PositionTrackSize, PalettedRotationTrackSize, PalettedPositionTrackSize;
    VecWord* PositionTrack; unsigned char* PalettedRotationTrack; unsigned char* PalettedPositionTrack;
    void Read(Stream& in, Pool& pool) {
        const char* name = in.ReadNullTerminatedString();
        BoneName = pool.AllocateType<char>((unsigned int)std::strlen(name) + 1);
        BoneType = in.ReadUBYTE() & 0x1f;
        std::strcpy(BoneName, name);
        SamplesPerSecond = in.ReadFloat();
        FrameCount = (unsigned short)in.ReadULONG();
        Animated = in.ReadEBOOL();
        RotationMode = in.ReadUBYTE() & 3; PositionMode = in.ReadUBYTE() & 3; ScalingMode = in.ReadUBYTE() & 3;
        PositionFactor = in.ReadFloat(); ScalingFactor = in.ReadFloat();
        RotationTrackSize = in.ReadUWORD(); RotationTrack = pool.AllocateType<Quat>(RotationTrackSize);
        in.Read(RotationTrack, RotationTrackSize * (long)sizeof(Quat));
        PalettedRotationTrackSize = in.ReadUWORD(); PalettedRotationTrack = pool.AllocateType<unsigned char>(PalettedRotationTrackSize);
        in.Read(PalettedRotationTrack, PalettedRotationTrackSize);
        PositionTrackSize = in.ReadUWORD(); PositionTrack = pool.AllocateType<VecWord>(PositionTrackSize);
        in.Read(PositionTrack, PositionTrackSize * (long)sizeof(VecWord));
        PalettedPositionTrackSize = in.ReadUWORD(); PalettedPositionTrack = pool.AllocateType<unsigned char>(PalettedPositionTrackSize);
        in.Read(PalettedPositionTrack, PalettedPositionTrackSize);
        Identity = (RotationMode == 0 && PositionMode == 0 && ScalingMode == 0) ? 1 : 0;
    }
};

static unsigned char g_file[512];
static long g_len;
static void Put(const void* p, long n) { std::memcpy(g_file + g_len, p, n); g_len += n; }
static void PutB(unsigned char v) { Put(&v, 1); }
static void PutW(unsigned short v) { Put(&v, 2); }
static void PutL(unsigned int v) { Put(&v, 4); }
static void PutF(float v) { Put(&v, 4); }
static void Build(const char* name, unsigned char rot, unsigned char pos, unsigned char scl, unsigned short nq, unsigned short npr,
                  unsigned short nv, unsigned short npp)
{
    g_len = 0; Put(name, (long)std::strlen(name) + 1);
    PutB(0x27); PutF(30.0f); PutL(0x10045); PutB(5); PutB(rot); PutB(pos); PutB(scl); PutF(0.5f); PutF(2.0f);
    PutW(nq); for (int i = 0; i < nq; ++i) { Quat q = { 0.0f, 0.0f, (float)i, 1.0f }; Put(&q, sizeof(q)); }
    PutW(npr); for (int i = 0; i < npr; ++i) PutB((unsigned char)(0x10 + i));
    PutW(nv); for (int i = 0; i < nv; ++i) { VecWord v = { (short)i, (short)-i, 7 }; Put(&v, sizeof(v)); }
    PutW(npp); for (int i = 0; i < npp; ++i) PutB((unsigned char)(0x80 + i));
}

int main()
{
    bool ok = true;
    {
        Build("Bip01 Spine", 1, 2, 0, 3, 2, 2, 1);
        Stream in = { g_file, g_len, 0, 0 }; Pool pool; SeqData d;
        d.Read(in, pool);
        ok = std::strcmp(d.BoneName, "Bip01 Spine") == 0 && d.BoneType == 7 && d.SamplesPerSecond == 30.0f && d.FrameCount == 0x45 &&
             d.Animated == 1 && d.RotationMode == 1 && d.PositionMode == 2 && d.ScalingMode == 0 && d.PositionFactor == 0.5f &&
             d.ScalingFactor == 2.0f && d.RotationTrackSize == 3 && d.RotationTrack[2].Z == 2.0f && d.PalettedRotationTrackSize == 2 &&
             d.PalettedRotationTrack[1] == 0x11 && d.PositionTrackSize == 2 && d.PositionTrack[1].Y == -1 &&
             d.PalettedPositionTrackSize == 1 && d.PalettedPositionTrack[0] == 0x80 && d.Identity == 0 && in.Pos == g_len &&
             in.Overflows == 2;
    }
    if (ok) {
        Build("Root", 0, 0, 0, 0, 0, 0, 0);
        Stream in = { g_file, g_len, 0, 0 }; Pool pool; SeqData d;
        d.Read(in, pool);
        ok = std::strcmp(d.BoneName, "Root") == 0 && d.RotationTrack == 0 && d.PositionTrack == 0 && d.PalettedRotationTrack == 0 &&
             d.PalettedPositionTrack == 0 && d.Identity == 1 && in.Pos == g_len && in.Overflows == 0 && pool.Used == 5;
    }
    if (!ok) { std::printf("00a4c5e0_TEST FAIL\n"); return 1; }
    std::printf("00a4c5e0_TEST PASS\n");
    return 0;
}
