// Behaviour test for the binary writer at 0x00A2EAA0: the exact write sequence through a recording stream.
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
struct C3DVector { float X, Y, Z; };
static void Write3DVector(CDataOutputStream& o, const C3DVector& v) { o.WriteFloat(v.X); o.WriteFloat(v.Y); o.WriteFloat(v.Z); }
struct CContainmentVolume { int id; void SaveBinary(CDataOutputStream& o) const { o.WriteLong(1000 + id); } };
struct C3DMeshVolume { const char* Name; CContainmentVolume* ConvexVolume; };
struct CPair { unsigned int first; C3DMeshVolume second; };
struct CGen { int BoneIndex; const char* ObjectName; unsigned int BankIndex; bool UseLocalOrigin; };
struct CStats {
    const char* Name; bool AnimatedFlag; C3DVector Center; float Radius; C3DVector Min, Max;
    unsigned int helperCount, dummyCount; unsigned short PackedNamesSize;
    CPair volumes[2]; unsigned int volumeCount; CGen gens[1]; unsigned int genCount;
    void SaveBinary(CDataOutputStream& out) const
    {
        out.WriteData(Name, 0);
        out.WriteBool(AnimatedFlag);
        Write3DVector(out, Center); out.WriteFloat(Radius); Write3DVector(out, Min); Write3DVector(out, Max);
        out.WriteUShort((unsigned short)helperCount); out.WriteUShort((unsigned short)dummyCount);
        out.WriteUShort(PackedNamesSize); out.WriteUShort((unsigned short)volumeCount); out.WriteUShort((unsigned short)genCount);
        out.WriteBuffer(0, helperCount * 20); out.WriteBuffer(0, dummyCount * 56); out.WriteBuffer(0, PackedNamesSize);
        for (unsigned int i = 0; i < volumeCount; ++i) {
            out.WriteULong(volumes[i].first);
            const C3DMeshVolume& v = volumes[i].second;
            out.WriteData(v.Name, 0);
            v.ConvexVolume->SaveBinary(out);
        }
        for (unsigned int j = 0; j < genCount; ++j) {
            const CGen& g = gens[j];
            out.WriteData(0, 48); out.WriteLong(g.BoneIndex); out.WriteData(g.ObjectName, 0); out.WriteULong(g.BankIndex); out.WriteBool(g.UseLocalOrigin);
        }
    }
};
int main()
{
    CContainmentVolume cv1 = { 1 }, cv2 = { 2 };
    CStats s; s.Name = "m"; s.AnimatedFlag = true; s.Center.X = 1; s.Center.Y = 2; s.Center.Z = 3; s.Radius = 4;
    s.Min.X = s.Min.Y = s.Min.Z = -1; s.Max.X = s.Max.Y = s.Max.Z = 1; s.helperCount = 2; s.dummyCount = 1; s.PackedNamesSize = 9;
    s.volumes[0].first = 7; s.volumes[0].second.Name = "a"; s.volumes[0].second.ConvexVolume = &cv1;
    s.volumes[1].first = 8; s.volumes[1].second.Name = "b"; s.volumes[1].second.ConvexVolume = &cv2; s.volumeCount = 2;
    s.gens[0].BoneIndex = -1; s.gens[0].ObjectName = "g"; s.gens[0].BankIndex = 5; s.gens[0].UseLocalOrigin = false; s.genCount = 1;
    CDataOutputStream out; s.SaveBinary(out);
    return Check("00a2eaa0", out.log, "d0;b1;f1;f2;f3;f4;f-1;f-1;f-1;f1;f1;f1;w2;w1;w9;w2;w1;z40;z56;z9;u7;d0;l1001;u8;d0;l1002;d48;l-1;d0;u5;b0;");
}