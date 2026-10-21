// Behaviour test for the binary writer at 0x00A4BEB0: the exact write sequence through a recording stream.
#include <cstdio>
#include <cstring>
// Recording output stream: every write appends "<tag><value>;" to a log and advances the position.
struct CDataOutputStream {
    char log[4096]; unsigned long pos;
    CDataOutputStream() : pos(0) { log[0] = 0; }
    void Add(const char* tag, double v, unsigned long bytes) { char b[64]; std::sprintf(b, "%s%g;", tag, v); std::strcat(log, b); pos += bytes; }
    unsigned long GetPosition() { return pos; }
    void WriteBool(bool v) { Add("b", v ? 1 : 0, 1); }
    void WriteUChar(unsigned char v) { Add("c", v, 1); }
    void WriteUShort(unsigned short v) { Add("w", v, 2); }
    void WriteShort(short v) { Add("s", v, 2); }
    void WriteULong(unsigned int v) { Add("u", v, 4); }
    void WriteLong(int v) { Add("l", v, 4); }
    void WriteFloat(float v) { Add("f", v, 4); }
    void WriteData(const void*, unsigned long size) { Add("d", (double)size, size); }
    void WriteBuffer(const void*, int size) { Add("z", size, size); }
};
static int Check(const char* addr, const char* got, const char* want)
{
    if (std::strcmp(got, want) != 0) { std::printf("%s_TEST FAIL\n%s\n%s\n", addr, got, want); return 1; }
    std::printf("%s_TEST PASS\n", addr); return 0;
}
struct CQuaternion { float X, Y, Z, W; };
struct C3DVectorWord { short X, Y, Z; };
struct CSeq {
    const char* BoneName; float SamplesPerSecond; unsigned short FrameCount;
    unsigned char BoneType : 5; unsigned char EnabledFlag : 1; unsigned char AnimatedFlag : 1; unsigned char IdentityFlag : 1;
    unsigned char RotationTrackMode : 2; unsigned char PositionTrackMode : 2; unsigned char ScalingTrackMode : 2;
    float PositionFactor, ScalingFactor; CQuaternion* RotationTrack;
    unsigned short RotationTrackSize, PositionTrackSize, PalettedRotationTrackSize, PalettedPositionTrackSize;
    C3DVectorWord* PositionTrack; unsigned char* PalettedRotationTrack; unsigned char* PalettedPositionTrack;
    void Write(CDataOutputStream& out) const
    {
        out.WriteData(BoneName, std::strlen(BoneName) + 1);
        out.WriteUChar(BoneType); out.WriteFloat(SamplesPerSecond); out.WriteULong(FrameCount); out.WriteBool(AnimatedFlag);
        out.WriteUChar(RotationTrackMode); out.WriteUChar(PositionTrackMode); out.WriteUChar(ScalingTrackMode);
        out.WriteFloat(PositionFactor); out.WriteFloat(ScalingFactor);
        out.WriteUShort(RotationTrackSize);
        for (unsigned long i = 0; i < RotationTrackSize; ++i) { const CQuaternion& q = RotationTrack[i]; out.WriteFloat(q.X); out.WriteFloat(q.Y); out.WriteFloat(q.Z); out.WriteFloat(q.W); }
        out.WriteUShort(PalettedRotationTrackSize);
        for (unsigned long j = 0; j < PalettedRotationTrackSize; ++j) out.WriteUChar(PalettedRotationTrack[j]);
        out.WriteUShort(PositionTrackSize);
        for (unsigned long k = 0; k < PositionTrackSize; ++k) { const C3DVectorWord& v = PositionTrack[k]; out.WriteShort(v.X); out.WriteShort(v.Y); out.WriteShort(v.Z); }
        out.WriteUShort(PalettedPositionTrackSize);
        for (unsigned long m = 0; m < PalettedPositionTrackSize; ++m) out.WriteUChar(PalettedPositionTrack[m]);
    }
};
int main()
{
    CQuaternion q[1] = { { 0, 0, 0, 1 } };
    C3DVectorWord v[1] = { { -2, 3, 4 } };
    unsigned char pr[2] = { 9, 8 }, pp[1] = { 7 };
    CSeq s; s.BoneName = "Bip01"; s.SamplesPerSecond = 30; s.FrameCount = 12;
    s.BoneType = 21; s.EnabledFlag = 1; s.AnimatedFlag = 1; s.IdentityFlag = 0;
    s.RotationTrackMode = 2; s.PositionTrackMode = 1; s.ScalingTrackMode = 3; s.PositionFactor = 0.5f; s.ScalingFactor = 2;
    s.RotationTrack = q; s.RotationTrackSize = 1; s.PositionTrack = v; s.PositionTrackSize = 1;
    s.PalettedRotationTrack = pr; s.PalettedRotationTrackSize = 2; s.PalettedPositionTrack = pp; s.PalettedPositionTrackSize = 1;
    CDataOutputStream out; s.Write(out);
    return Check("00a4beb0", out.log, "d6;c21;f30;u12;b1;c2;c1;c3;f0.5;f2;w1;f0;f0;f0;f1;w2;c9;c8;w1;s-2;s3;s4;w1;c7;");
}