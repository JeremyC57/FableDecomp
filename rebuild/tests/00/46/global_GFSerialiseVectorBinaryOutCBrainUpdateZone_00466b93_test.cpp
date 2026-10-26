// Behaviour test for 0x00466B93 GFSerialiseVectorBinaryOut<CBrainUpdateZone>: the count, then each zone's Distance,
// UpdateFrequency and InterruptionFrequency as longs, in element order
#include <cstdio>
struct Writer { long out[16]; int n; Writer() : n(0) { } void WriteLong(long v) { out[n++] = v; } };
class CBrainUpdateZone { public: long Distance; long UpdateFrequency; long InterruptionFrequency; };
struct ZoneVector { CBrainUpdateZone* _M_start; CBrainUpdateZone* _M_finish; CBrainUpdateZone* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } CBrainUpdateZone& operator[](unsigned int i) { return _M_start[i]; } };
struct ZoneTraits {
    CBrainUpdateZone DefaultVal;
    ZoneTraits() { DefaultVal.Distance = DefaultVal.UpdateFrequency = DefaultVal.InterruptionFrequency = 0; }
    void TransferBinaryOut(Writer& s, const CBrainUpdateZone* v) const { s.WriteLong(v->Distance); s.WriteLong(v->UpdateFrequency); s.WriteLong(v->InterruptionFrequency); }
};
static void GFSerialiseVectorBinaryOut(Writer& stream, ZoneVector& value)
{
    ZoneTraits traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i) traits.TransferBinaryOut(stream, &value[i]);
}
int main()
{
    CBrainUpdateZone z[2] = { { 10, 20, 30 }, { 40, 50, 60 } };
    ZoneVector v = { z, z + 2, z + 2 };
    Writer w; GFSerialiseVectorBinaryOut(w, v);
    bool ok = w.n == 7 && w.out[0] == 2;
    for (int i = 1; i < 7; ++i) ok = ok && w.out[i] == 10 * i;
    if (!ok) { std::printf("00466b93_TEST FAIL\n"); return 1; }
    std::printf("00466b93_TEST PASS\n");
    return 0;
}