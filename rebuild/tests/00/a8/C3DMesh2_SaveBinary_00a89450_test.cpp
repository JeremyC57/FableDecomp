// Behaviour test for the binary writer at 0x00A89450: the exact write sequence through a recording stream.
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
struct CMaterial { int id; void SaveBinary(CDataOutputStream& o) const { o.WriteLong(100 + id); } };
struct CPrimitive { unsigned int StaticBlockCount, AnimatedBlockCount; int id; void SaveBinary(CDataOutputStream& o) const { o.WriteLong(200 + id); } };
struct CMesh {
    unsigned int MaterialCount; CMaterial* Materials; unsigned int PrimitiveCount; CPrimitive* Primitives;
    unsigned int BoneCount; unsigned int BoneNameDataSize; bool ClothFlag;
    void SaveStats(CDataOutputStream& o) const { o.WriteLong(-7); }
    unsigned long SaveBinary(CDataOutputStream& out) const
    {
        unsigned long start = out.GetPosition();
        SaveStats(out);
        out.WriteULong(MaterialCount); out.WriteULong(PrimitiveCount); out.WriteULong(BoneCount); out.WriteULong(BoneNameDataSize);
        out.WriteBool(ClothFlag);
        unsigned long staticBlocks = 0, animatedBlocks = 0;
        for (unsigned long p = 0; p < PrimitiveCount; ++p) { staticBlocks += Primitives[p].StaticBlockCount; animatedBlocks += Primitives[p].AnimatedBlockCount; }
        out.WriteUShort((unsigned short)staticBlocks); out.WriteUShort((unsigned short)animatedBlocks);
        out.WriteData(0, BoneCount * 2);
        out.WriteBuffer(0, BoneNameDataSize); out.WriteBuffer(0, BoneCount * 60); out.WriteBuffer(0, BoneCount * 48); out.WriteBuffer(0, BoneCount * 64);
        out.WriteData(0, 48);
        for (unsigned long i = 0; i < MaterialCount; ++i) Materials[i].SaveBinary(out);
        for (unsigned long j = 0; j < PrimitiveCount; ++j) Primitives[j].SaveBinary(out);
        return out.GetPosition() - start;
    }
};
int main()
{
    CMaterial m[1] = { { 1 } };
    CPrimitive p[2] = { { 3, 1, 1 }, { 4, 2, 2 } };
    CMesh mesh = { 1, m, 2, p, 2, 10, true };
    CDataOutputStream out; out.pos = 50;
    unsigned long n = mesh.SaveBinary(out);
    if (n != out.pos - 50) { std::printf("00a89450_TEST FAIL\n"); return 1; }
    return Check("00a89450", out.log, "l-7;u1;u2;u2;u10;b1;w7;w3;d4;z10;z120;z96;z128;d48;l101;l201;l202;");
}