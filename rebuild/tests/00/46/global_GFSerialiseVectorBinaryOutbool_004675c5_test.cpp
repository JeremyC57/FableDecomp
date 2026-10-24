// Behaviour test for 0x004675C5 GFSerialiseVectorBinaryOut<bool>: the count as a long, then every flag through WriteBool, in order
#include <cstdio>
struct Writer {
    long count; int n; bool flags[8]; int calls;
    Writer() : count(-1), n(0), calls(0) { }
    void WriteLong(long v) { count = v; ++calls; }
    void WriteBool(bool v) { flags[n++] = v; ++calls; }
};
struct BoolVector { bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } bool& operator[](unsigned int i) { return _M_start[i]; } };
struct BoolTraits { bool DefaultVal; void TransferBinaryOut(Writer& s, const bool* v) const { s.WriteBool(*v); } };
static void GFSerialiseVectorBinaryOut(Writer& stream, BoolVector& value)
{
    BoolTraits traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i) traits.TransferBinaryOut(stream, &value[i]);
}
int main()
{
    bool data[4] = { true, false, false, true };
    BoolVector v = { data, data + 4, data + 4 };
    Writer w; GFSerialiseVectorBinaryOut(w, v);
    bool ok = w.count == 4 && w.n == 4 && w.flags[0] && !w.flags[1] && !w.flags[2] && w.flags[3] && w.calls == 5;
    if (!ok) { std::printf("004675c5_TEST FAIL\n"); return 1; }
    std::printf("004675c5_TEST PASS\n");
    return 0;
}