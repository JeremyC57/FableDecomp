// Behaviour test for 0x00464CED GFSerialiseVectorBinaryOut<float>: the count as a long, then every element as a float, in order
#include <cstdio>
struct Writer {
    int n; long longs[8]; float floats[8]; char kinds[16]; int k;
    Writer() : n(0), k(0) { }
    void WriteLong(long v) { longs[n] = v; kinds[k++] = 'L'; }
    void WriteFloat(float v) { floats[n++] = v; kinds[k++] = 'F'; }
};
struct FloatVector { float* _M_start; float* _M_finish; float* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } float& operator[](unsigned int i) { return _M_start[i]; } };
struct FloatTraits { float DefaultVal; void TransferBinaryOut(Writer& s, const float* v) const { s.WriteFloat(*v); } };
static void GFSerialiseVectorBinaryOut(Writer& stream, FloatVector& value)
{
    FloatTraits traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i) traits.TransferBinaryOut(stream, &value[i]);
}
int main()
{
    float data[3] = { 1.5f, -2.0f, 8.25f };
    FloatVector v = { data, data + 3, data + 3 };
    Writer w; GFSerialiseVectorBinaryOut(w, v);
    FloatVector e = { data, data, data };
    Writer w2; GFSerialiseVectorBinaryOut(w2, e);
    bool ok = w.longs[0] == 3 && w.n == 3 && w.floats[0] == 1.5f && w.floats[1] == -2.0f && w.floats[2] == 8.25f && w.k == 4 && w.kinds[0] == 'L';
    ok = ok && w2.k == 1 && w2.longs[0] == 0;
    if (!ok) { std::printf("00464ced_TEST FAIL\n"); return 1; }
    std::printf("00464ced_TEST PASS\n");
    return 0;
}