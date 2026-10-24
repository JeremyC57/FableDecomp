#include <cstdio>
// Behaviour copy of GFSerialiseVectorBinaryOut<CExplosionRing>: the count, then each ring's own writer in order.
struct SStream { long out[64]; int n; SStream() : n(0) { } void WriteSLONG(long v) { out[n++] = v; } };
struct CExplosionRing {
    float RingRadius; long NumExplosions; float AngleOffset; float SecondsBeforeSpawnNewRing;
    void TransferBinaryOut(SStream& s) const { s.WriteSLONG(NumExplosions); s.WriteSLONG((long)RingRadius); }
};
static void SerialiseOut(SStream& stream, const CExplosionRing* first, const CExplosionRing* last)
{
    long count = long(last - first);
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        first[i].TransferBinaryOut(stream);
}
int main()
{
    bool ok = true;
    CExplosionRing r[3];
    for (int i = 0; i < 3; ++i) { r[i].RingRadius = 10.0f * (i + 1); r[i].NumExplosions = i + 4; r[i].AngleOffset = 0; r[i].SecondsBeforeSpawnNewRing = 1; }
    SStream s; SerialiseOut(s, r, r + 3);
    const long expect[7] = { 3, 4, 10, 5, 20, 6, 30 };
    if (s.n != 7) ok = false;
    for (int i = 0; ok && i < 7; ++i) if (s.out[i] != expect[i]) ok = false;
    SStream e; SerialiseOut(e, r, r);
    if (e.n != 1 || e.out[0] != 0) ok = false;
    if (!ok) { std::printf("004eb1a8_TEST FAIL\n"); return 1; }
    std::printf("004eb1a8_TEST PASS\n");
    return 0;
}