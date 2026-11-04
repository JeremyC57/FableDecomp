// Behaviour test for NParticleEngine::CPSCEmitterGeneric::ReadBinary (0x00B149D0): a standalone model of the stream layout written
// by WriteBinary 0x00B12EF0 -- two ULONG param keys, the bit-field integers (masked to their widths) and one-byte EBOOL flags in
// member order, the spline flag, then (only when set) the tension, the point count and the points, which replace the control points
// of a freshly made spline (default tension 0.5 is overwritten). A short stream leaves later members untouched.
#include <cstdio>
#include <cstring>
#include <vector>
struct Stream {
    const unsigned char* p; long left;
    bool Read(void* d, long n) { if (n > left) return false; memcpy(d, p, n); p += n; left -= n; return true; }
    unsigned int ReadULONG(unsigned int keep) { unsigned int v = keep; Read(&v, 4); return v; }
    bool ReadEBOOL(bool keep) { unsigned char v = keep; if (!Read(&v, 1)) return keep; return v != 0; }
    float ReadFloat(float keep) { float v = keep; Read(&v, 4); return v; }
};
struct Vec { float x, y, z; };
struct Emitter {
    unsigned int posParam, dirParam;
    unsigned int noStart : 10, noStartRand : 10, type : 2, solid : 1;
    unsigned short scaleZ : 11;
    bool hasSpline; float tension; std::vector<Vec> points;
};
static void put(unsigned char* b, long& n, unsigned int v) { memcpy(b + n, &v, 4); n += 4; }
int main()
{
    unsigned char buf[256]; long n = 0;
    put(buf, n, 0x1234); put(buf, n, 0x5678); put(buf, n, 1025); put(buf, n, 7); put(buf, n, 6); buf[n++] = 3;
    put(buf, n, 0xFFFF);                                   // NonUniformScalingZ: 11 bits keep 0x7FF
    buf[n++] = 1;                                          // spline present
    float t = 0.25f; memcpy(buf + n, &t, 4); n += 4; put(buf, n, 2);
    for (int i = 0; i < 6; ++i) { float f = (float)i; memcpy(buf + n, &f, 4); n += 4; }
    Stream s = { buf, n };
    Emitter e; memset(&e, 0, sizeof(e) - sizeof(e.points));
    e.posParam = s.ReadULONG(0); e.dirParam = s.ReadULONG(0);
    e.noStart = s.ReadULONG(0); e.noStartRand = s.ReadULONG(0); e.type = s.ReadULONG(0); e.solid = s.ReadEBOOL(false);
    e.scaleZ = s.ReadULONG(0);
    e.hasSpline = s.ReadEBOOL(false);
    if (e.hasSpline) {
        e.tension = 0.5f; e.tension = s.ReadFloat(e.tension);
        unsigned int count = s.ReadULONG(0);
        std::vector<Vec> pts(count);
        for (unsigned int i = 0; i < count; ++i) { pts[i].x = s.ReadFloat(0); pts[i].y = s.ReadFloat(0); pts[i].z = s.ReadFloat(0); }
        e.points = pts;
    }
    float extra = s.ReadFloat(-3.0f);
    bool ok = e.posParam == 0x1234 && e.dirParam == 0x5678 && e.noStart == 1 && e.noStartRand == 7 && e.type == 2 && e.solid
        && e.scaleZ == 0x7FF && e.hasSpline && e.tension == 0.25f && e.points.size() == 2 && e.points[1].z == 5.0f && extra == -3.0f;
    printf(ok ? "00b149d0_TEST PASS\n" : "00b149d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
