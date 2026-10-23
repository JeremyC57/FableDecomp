// Behaviour test for C3DMeshMaterial::LoadBinary (0x00ABF100): scalar fields, flags, optional filename block resolved through the bank.
#include <cstdio>
#include <cstring>
struct Stream {
    const unsigned char* p;
    unsigned int ReadU32() { unsigned int v; std::memcpy(&v, p, 4); p += 4; return v; }
    bool ReadBool() { unsigned char v = *p++; return v != 0; }
    const char* ReadString() { const char* s = (const char*)p; p += std::strlen(s) + 1; return s; }
};
struct Bank {
    int FindIndexByFilename(const char* name, int kind) const { return (int)std::strlen(name) * 10 + kind; }
};
struct Material {
    unsigned int MaterialIndex; char Name[32]; unsigned int DecalID;
    int Diffuse, Bump, Reflection, Illumination; unsigned int MapFlags, SelfIllumination;
    bool TwoSided, Transparent, BooleanAlpha, Degenerate;
    void LoadBinary(Stream& in, const Bank* bank)
    {
        MaterialIndex = in.ReadU32();
        std::strcpy(Name, in.ReadString());
        DecalID = in.ReadU32();
        Diffuse = (int)in.ReadU32(); Bump = (int)in.ReadU32(); Reflection = (int)in.ReadU32(); Illumination = (int)in.ReadU32();
        MapFlags = in.ReadU32(); SelfIllumination = in.ReadU32();
        TwoSided = in.ReadBool(); Transparent = in.ReadBool(); BooleanAlpha = in.ReadBool(); Degenerate = in.ReadBool();
        if (in.ReadBool()) {
            const char* d = in.ReadString(); const char* b = in.ReadString();
            const char* r = in.ReadString(); const char* i = in.ReadString();
            if (bank) {
                Diffuse = bank->FindIndexByFilename(d, 0); Bump = bank->FindIndexByFilename(b, 2);
                Reflection = bank->FindIndexByFilename(r, 0); Illumination = bank->FindIndexByFilename(i, 0);
            }
        }
    }
};
static unsigned char* put32(unsigned char* q, unsigned int v) { std::memcpy(q, &v, 4); return q + 4; }
static unsigned char* puts_(unsigned char* q, const char* s) { std::strcpy((char*)q, s); return q + std::strlen(s) + 1; }
static int build(unsigned char* buf, bool names)
{
    unsigned char* q = buf;
    q = put32(q, 7); q = puts_(q, "mat"); q = put32(q, 9);
    q = put32(q, 1); q = put32(q, 2); q = put32(q, 3); q = put32(q, 4); q = put32(q, 0x55); q = put32(q, 66);
    *q++ = 1; *q++ = 0; *q++ = 1; *q++ = 0; *q++ = names ? 1 : 0;
    if (names) { q = puts_(q, "ab"); q = puts_(q, "abc"); q = puts_(q, "a"); q = puts_(q, "abcd"); }
    return (int)(q - buf);
}
int main()
{
    unsigned char buf[256]; Bank bank; Material m;
    int n = build(buf, false); Stream s = { buf }; m.LoadBinary(s, &bank);
    if (s.p != buf + n || m.MaterialIndex != 7 || std::strcmp(m.Name, "mat") || m.Diffuse != 1 || m.Illumination != 4
        || m.MapFlags != 0x55 || !m.TwoSided || m.Transparent || !m.BooleanAlpha) { std::printf("00abf100_TEST FAIL\n"); return 1; }
    n = build(buf, true); Stream t = { buf }; m.LoadBinary(t, &bank);
    if (t.p != buf + n || m.Diffuse != 20 || m.Bump != 32 || m.Reflection != 10 || m.Illumination != 40) { std::printf("00abf100_TEST FAIL\n"); return 1; }
    Stream u = { buf }; m.LoadBinary(u, 0);
    if (u.p != buf + n || m.Diffuse != 1 || m.Bump != 2) { std::printf("00abf100_TEST FAIL\n"); return 1; }
    std::printf("00abf100_TEST PASS\n");
    return 0;
}