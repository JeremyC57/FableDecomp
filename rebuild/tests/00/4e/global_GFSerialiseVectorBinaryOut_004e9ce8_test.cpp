#include <cstdio>
// Behaviour copy of GFSerialiseVectorBinaryOut<CMusicEntry>: the count, then one traits write per element in order; the
// traits' DefaultVal is the default CMusicEntry (ID 0, Filename -1, Gain 0, CacheToHD false).
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct CMusicEntry {
    unsigned long ID; CDefString Filename; float Gain; bool CacheToHD;
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
};
struct SStream { long out[64]; int n; SStream() : n(0) { } void WriteSLONG(long v) { out[n++] = v; } };
struct STraits {
    CMusicEntry DefaultVal;
    void TransferBinaryOut(SStream& s, const CMusicEntry* v) const
    {
        s.WriteSLONG((long)v->ID); s.WriteSLONG(v->Filename.TablePos); s.WriteSLONG((long)(v->Gain * 100.0f)); s.WriteSLONG(v->CacheToHD ? 1 : 0);
    }
};
static void SerialiseOut(SStream& stream, const CMusicEntry* first, const CMusicEntry* last, STraits& traits)
{
    long count = long(last - first);
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, first + i);
}
int main()
{
    bool ok = true;
    STraits traits;
    if (traits.DefaultVal.ID != 0 || traits.DefaultVal.Filename.TablePos != -1 || traits.DefaultVal.Gain != 0.0f || traits.DefaultVal.CacheToHD) ok = false;
    CMusicEntry v[2];
    v[0].ID = 5; v[0].Filename.TablePos = 9; v[0].Gain = 0.5f; v[0].CacheToHD = true;
    v[1].ID = 6;
    SStream s; SerialiseOut(s, v, v + 2, traits);
    const long expect[9] = { 2, 5, 9, 50, 1, 6, -1, 0, 0 };
    if (s.n != 9) ok = false;
    for (int i = 0; ok && i < 9; ++i) if (s.out[i] != expect[i]) ok = false;
    SStream e; SerialiseOut(e, v, v, traits);
    if (e.n != 1 || e.out[0] != 0) ok = false;
    if (!ok) { std::printf("004e9ce8_TEST FAIL\n"); return 1; }
    std::printf("004e9ce8_TEST PASS\n");
    return 0;
}