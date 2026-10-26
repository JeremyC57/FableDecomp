#include <cstdio>
// Behaviour copy of GFSerialiseVectorBinaryIn<CMusicEntry>: read the count, resize (new elements default), then read each
// element in place through the traits.
class CDefString { public: long TablePos; CDefString() : TablePos(-1) { } };
class CMusicEntry { public:
    unsigned long ID; CDefString Filename; float Gain; bool CacheToHD;
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
};
struct SStream {
    const long* data; int pos; int size;
    SStream(const long* d, int n) : data(d), pos(0), size(n) { }
    void Read(long* out) { if (pos < size) *out = data[pos++]; }
};
struct SVec {
    CMusicEntry items[16]; int count;
    SVec() : count(0) { }
    void resize(int n) { for (int i = count; i < n; ++i) items[i] = CMusicEntry(); count = n; }
};
struct STraits {
    CMusicEntry DefaultVal;
    void TransferBinaryIn(SStream& s, CMusicEntry* v) const { long id = 0; s.Read(&id); v->ID = (unsigned long)id; s.Read(&v->Filename.TablePos); }
};
static void SerialiseIn(SStream& stream, SVec& out)
{
    STraits traits;
    long count = 0;
    stream.Read(&count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out.items[i]);
}
int main()
{
    bool ok = true;
    const long data[] = { 3, 11, 110, 12, 120, 13 };   // third entry truncated after its ID
    SStream s(data, 6);
    SVec v;
    SerialiseIn(s, v);
    if (v.count != 3) ok = false;
    if (ok && (v.items[0].ID != 11 || v.items[0].Filename.TablePos != 110)) ok = false;
    if (ok && (v.items[1].ID != 12 || v.items[1].Filename.TablePos != 120)) ok = false;
    if (ok && (v.items[2].ID != 13 || v.items[2].Filename.TablePos != -1 || v.items[2].Gain != 0.0f || v.items[2].CacheToHD)) ok = false;
    if (!ok) { std::printf("004e9d71_TEST FAIL\n"); return 1; }
    std::printf("004e9d71_TEST PASS\n");
    return 0;
}