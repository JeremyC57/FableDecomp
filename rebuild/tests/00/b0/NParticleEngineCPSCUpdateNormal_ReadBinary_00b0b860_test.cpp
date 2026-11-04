// Behaviour test for NParticleEngine::CPSCUpdateNormal::ReadBinary (0x00B0B860): a standalone model of the stream layout written
// by WriteBinary 0x00B0B5C0 -- two ULONG fade integers, eighteen one-byte EBOOL flags (non-zero = true), the null-terminated decal
// emitter name, fifteen floats, five C3DVectors and the two parameter keys, read back in the same order; a short stream leaves the
// remaining members untouched (the bounds-checked inline read).
#include <cstdio>
#include <cstring>
#include <string>
struct Stream {
    const unsigned char* p; long left;
    bool Read(void* d, long n) { if (n > left) return false; memcpy(d, p, n); p += n; left -= n; return true; }
    unsigned int ReadULONG(unsigned int keep) { unsigned int v = keep; Read(&v, 4); return v; }
    bool ReadEBOOL(bool keep) { unsigned char v = keep; if (!Read(&v, 1)) return keep; return v != 0; }
    float ReadFloat(float keep) { float v = keep; Read(&v, 4); return v; }
    std::string ReadString() { std::string s; while (left > 0) { char c = (char)*p++; --left; if (!c) break; s += c; } return s; }
};
int main()
{
    unsigned char buf[256]; long n = 0;
    unsigned int fadeIn = 12, fadeOut = 900; memcpy(buf + n, &fadeIn, 4); n += 4; memcpy(buf + n, &fadeOut, 4); n += 4;
    for (int i = 0; i < 18; ++i) buf[n++] = (unsigned char)(i % 3 == 0 ? 2 : 0);
    memcpy(buf + n, "Smoke", 6); n += 6;
    for (int i = 0; i < 15; ++i) { float f = 0.5f * i; memcpy(buf + n, &f, 4); n += 4; }
    Stream s = { buf, n };
    unsigned int a = s.ReadULONG(0), b = s.ReadULONG(0);
    bool flags[18]; for (int i = 0; i < 18; ++i) flags[i] = s.ReadEBOOL(false);
    std::string name = s.ReadString();
    float f[15]; for (int i = 0; i < 15; ++i) f[i] = s.ReadFloat(-1.0f);
    float vx = s.ReadFloat(-7.0f);                              // stream exhausted: member left alone
    bool ok = a == 12 && b == 900 && flags[0] && !flags[1] && flags[3] && name == "Smoke" && f[0] == 0.0f && f[14] == 7.0f && vx == -7.0f;
    printf(ok ? "00b0b860_TEST PASS\n" : "00b0b860_TEST FAIL\n");
    return ok ? 0 : 1;
}