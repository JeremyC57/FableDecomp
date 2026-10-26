#include <cstdio>
// Behaviour copy of GFSerialiseVectorBinaryIn<CExplosionRing>: read the count, resize with CExplosionRing() defaults
// {1.0, 1, 0.0, 1.0}, then each ring reads itself in place.
struct SStream {
    const long* data; int pos; int size;
    SStream(const long* d, int n) : data(d), pos(0), size(n) { }
    void Read(long* out) { if (pos < size) *out = data[pos++]; }
};
class CExplosionRing { public:
    float RingRadius; long NumExplosions; float AngleOffset; float SecondsBeforeSpawnNewRing;
    CExplosionRing() : RingRadius(1.0f), NumExplosions(1), AngleOffset(0.0f), SecondsBeforeSpawnNewRing(1.0f) { }
    void TransferBinaryIn(SStream& s) { s.Read(&NumExplosions); }
};
struct SVec {
    CExplosionRing items[16]; int count;
    SVec() : count(0) { }
    void resize(int n, const CExplosionRing& x) { for (int i = count; i < n; ++i) items[i] = x; count = n; }
};
static void SerialiseIn(SStream& stream, SVec& out)
{
    long count = 0;
    stream.Read(&count);
    out.resize(count, CExplosionRing());
    for (long i = 0; i < count; ++i)
        out.items[i].TransferBinaryIn(stream);
}
int main()
{
    bool ok = true;
    const long data[] = { 3, 7, 8 };                 // the third ring keeps its default count
    SStream s(data, 3);
    SVec v; SerialiseIn(s, v);
    if (v.count != 3 || v.items[0].NumExplosions != 7 || v.items[1].NumExplosions != 8 || v.items[2].NumExplosions != 1) ok = false;
    if (v.items[2].RingRadius != 1.0f || v.items[2].AngleOffset != 0.0f || v.items[2].SecondsBeforeSpawnNewRing != 1.0f) ok = false;
    if (!ok) { std::printf("004eb219_TEST FAIL\n"); return 1; }
    std::printf("004eb219_TEST PASS\n");
    return 0;
}