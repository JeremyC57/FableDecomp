#include <cstdio>
#include <cstring>
// Behaviour copy of C3DGeneratorObject::LoadBinary (0x00A2E5B0): matrix, ulong, string, ulong, bool (byte != 0).
struct Str { char text[16]; };
struct Matrix { float M[12]; };
struct Stream {
    unsigned int pos; const unsigned char* data; int remaining; int overflowCalls; int stringCalls;
    Stream(const unsigned char* d, int n) : pos(0), data(d), remaining(n), overflowCalls(0), stringCalls(0) {}
    void Overflow(void* b, int size) { ++overflowCalls; std::memset(b, 0, size); pos += size; }
    void Read(void* b, int size) {
        long long np = (long long)(pos + size); if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (remaining >= size) { std::memcpy(b, data, size); data += size; remaining -= size; pos += size; } else Overflow(b, size);
    }
    Matrix ReadMatrix3x4() { Matrix m; Read(m.M, 48); return m; }
    Str ReadNullTerminatedString() { Str s; ++stringCalls; std::strcpy(s.text, "gen"); return s; }
    unsigned int ReadULong() { unsigned int v; Read(&v, 4); return v; }
    bool ReadBool() { unsigned char v; Read(&v, 1); return v != 0; }
};
struct Gen {
    Matrix M; unsigned int GeneratorID; Str ObjectName; unsigned int BankIndex; bool UseLocalOrigin;
    void LoadBinary(Stream* in) {
        M = in->ReadMatrix3x4(); GeneratorID = in->ReadULong(); ObjectName = in->ReadNullTerminatedString();
        BankIndex = in->ReadULong(); UseLocalOrigin = in->ReadBool();
    }
};
int main() {
    unsigned char b[57]; float m[12]; for (int i = 0; i < 12; ++i) m[i] = (float)i;
    std::memcpy(b, m, 48); unsigned int id = 7, bank = 3; std::memcpy(b + 48, &id, 4); std::memcpy(b + 52, &bank, 4); b[56] = 5;
    Stream s(b, 57); Gen g; g.LoadBinary(&s);
    if (g.M.M[11] != 11.0f || g.GeneratorID != 7 || g.BankIndex != 3 || !g.UseLocalOrigin || *(unsigned char*)&g.UseLocalOrigin != 1
        || std::strcmp(g.ObjectName.text, "gen") != 0 || s.stringCalls != 1 || s.pos != 57) { std::printf("00a2e5b0_TEST FAIL\n"); return 1; }
    std::printf("00a2e5b0_TEST PASS\n"); return 0;
}