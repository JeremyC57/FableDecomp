// Behaviour test for NParticleEngine::CPSCSingleSprite::ReadBinary (0x00B068F0): a standalone model of the read -- the sprite bank
// index, three colours, the animation / size floats, the alignment and crossed-sprite longs, fade values, the position parameter,
// the four blend modes / ops, the trail bank / width / length, the stay-with-emitter factor, then fifteen one-byte EBOOLs, all in
// CPSCSingleSprite::WriteBinary 0x00B06060 order; the CPSCSplineBase part (0x00B22B40) is read last, after every sprite member.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
struct Stream {
    std::vector<unsigned char> data; unsigned long pos;
    Stream() : pos(0) {}
    void Read(void* out, long size) { if (size <= 0 || pos + size > data.size()) return; memcpy(out, &data[pos], size); pos += size; }
    unsigned long ReadULONG() { unsigned long v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadEBOOL() { unsigned char v = 0; Read(&v, 1); return v != 0; }
};
enum { L, F, C };                                          // long, float, colour (each 4 bytes)
static const int kOrder[23] = { L, C, C, C, F, F, F, L, L, F, L, L, F, F, L, L, L, L, L, L, F, L, L };
struct Sprite {
    unsigned long v[23]; bool b[15]; unsigned long splineFirst; unsigned long posWhenSplineRead;
    void SplineBaseReadBinary(Stream& in) { posWhenSplineRead = in.pos; splineFirst = in.ReadULONG(); }
    void ReadBinary(Stream& in) {
        for (int i = 0; i < 23; ++i) v[i] = in.ReadULONG();
        for (int i = 0; i < 15; ++i) b[i] = in.ReadEBOOL();
        SplineBaseReadBinary(in);
    }
};
int main()
{
    Stream s;
    for (int i = 0; i < 23; ++i) {
        unsigned long u = 0;
        if (kOrder[i] == F) { float f = 0.5f * i; memcpy(&u, &f, 4); } else u = 0x1000u * i + kOrder[i];
        s.data.insert(s.data.end(), (unsigned char*)&u, (unsigned char*)&u + 4);
    }
    for (int i = 0; i < 15; ++i) s.data.push_back((unsigned char)(i % 3 == 0 ? 0 : i));
    unsigned long spline = 0xDEADBEEF; s.data.insert(s.data.end(), (unsigned char*)&spline, (unsigned char*)&spline + 4);
    Sprite sp; memset(&sp, 0, sizeof(sp)); sp.ReadBinary(s);
    bool ok = sp.posWhenSplineRead == 23 * 4 + 15 && sp.splineFirst == 0xDEADBEEF && s.pos == s.data.size();
    for (int i = 0; i < 23; ++i) {
        if (kOrder[i] == F) { float f; memcpy(&f, &sp.v[i], 4); ok = ok && std::fabs(f - 0.5f * i) < 1e-6f; }
        else ok = ok && sp.v[i] == 0x1000u * i + kOrder[i];
    }
    for (int i = 0; i < 15; ++i) ok = ok && sp.b[i] == (i % 3 != 0);
    printf(ok ? "00b068f0_TEST PASS\n" : "00b068f0_TEST FAIL\n");
    return ok ? 0 : 1;
}