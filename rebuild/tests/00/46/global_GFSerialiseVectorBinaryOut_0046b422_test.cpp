// Behaviour test for 0x0046B422 GFSerialiseVectorBinaryOut<CMusicEntry>: count, then one traits write per entry
#include <cstdio>
struct Writer { float log[32]; int n; void Write(float v) { log[n++] = v; } };
struct CMusicEntry {
    unsigned int ID; int Filename; float Gain; bool CacheToHD;
    CMusicEntry() : ID(0), Filename(-1), Gain(0.0f), CacheToHD(false) {}
};
struct Traits {
    CMusicEntry DefaultVal;
    void TransferBinaryOut(Writer& w, const CMusicEntry* e) const
    { w.Write((float)e->ID); w.Write((float)e->Filename); w.Write(e->Gain); w.Write(e->CacheToHD ? 1.0f : 0.0f); }
};
struct EntryVector { CMusicEntry* _M_start; CMusicEntry* _M_finish; CMusicEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    Traits traits;
    if (traits.DefaultVal.ID != 0 || traits.DefaultVal.Filename != -1 || traits.DefaultVal.Gain != 0.0f || traits.DefaultVal.CacheToHD) stream.Write(-99.0f);
    int count = (int)(value._M_finish - value._M_start);
    stream.Write((float)count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, value._M_start + i);
}
int main()
{
    CMusicEntry e[2];
    e[0].ID = 4; e[0].Filename = 5; e[0].Gain = 0.5f; e[0].CacheToHD = true;
    e[1].ID = 6;
    EntryVector v = { e, e + 2, e + 2 };
    Writer w; w.n = 0;
    SerialiseOut(w, v);
    const float want[9] = { 2.0f, 4.0f, 5.0f, 0.5f, 1.0f, 6.0f, -1.0f, 0.0f, 0.0f };
    if (w.n != 9) { std::printf("0046b422_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (w.log[i] != want[i]) { std::printf("0046b422_TEST FAIL\n"); return 1; }
    std::printf("0046b422_TEST PASS\n");
    return 0;
}