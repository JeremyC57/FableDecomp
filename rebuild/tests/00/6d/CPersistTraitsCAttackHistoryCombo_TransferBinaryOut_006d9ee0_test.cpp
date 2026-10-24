// Behaviour test for 0x006D9EE0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryOut: the stream receives the pair
// count, then first/second of every pair in order, then the multiplier.
#include <cstdio>

struct Stream {
    long longs[16]; int nLongs; float floats[4]; int nFloats; int order[20]; int nOrder;
    void WriteSLONG(long v) { longs[nLongs++] = v; order[nOrder++] = 0; }
    void WriteFloat(float v) { floats[nFloats++] = v; order[nOrder++] = 1; }
};
struct Pair { long first; long second; };
struct Combo { Pair* start; Pair* finish; Pair* eos; float Multiplier; };

static void TransferBinaryOut(Stream& stream, const Combo& value)
{
    stream.WriteSLONG((long)(value.finish - value.start));
    for (const Pair* it = value.start; it != value.finish; ++it) { stream.WriteSLONG(it->first); stream.WriteSLONG(it->second); }
    stream.WriteFloat(value.Multiplier);
}

int main()
{
    Pair p[2] = { { 3, 5 }, { 4, 6 } };
    Combo c; c.start = p; c.finish = p + 2; c.eos = p + 2; c.Multiplier = 1.25f;
    Stream s; s.nLongs = s.nFloats = s.nOrder = 0;
    TransferBinaryOut(s, c);
    if (s.nLongs != 5 || s.longs[0] != 2 || s.longs[1] != 3 || s.longs[2] != 5 || s.longs[3] != 4 || s.longs[4] != 6) { std::printf("006d9ee0_TEST FAIL\n"); return 1; }
    if (s.nFloats != 1 || s.floats[0] != 1.25f || s.order[5] != 1) { std::printf("006d9ee0_TEST FAIL\n"); return 1; }
    Combo e; e.start = e.finish = e.eos = 0; e.Multiplier = 0.5f;
    Stream t; t.nLongs = t.nFloats = t.nOrder = 0;
    TransferBinaryOut(t, e);
    if (t.nLongs != 1 || t.longs[0] != 0 || t.nFloats != 1) { std::printf("006d9ee0_TEST FAIL\n"); return 1; }
    std::printf("006d9ee0_TEST PASS\n");
    return 0;
}