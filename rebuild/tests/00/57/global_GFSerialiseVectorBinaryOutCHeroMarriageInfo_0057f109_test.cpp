// Behaviour test for 0x0057F109 GFSerialiseVectorBinaryOut<CHeroMarriageInfo>: the count first, then each record in
// order through the traits writer (two 64-bit UIDs, the state, the flag).
#include <cstdio>

class CHeroMarriageInfo { public: unsigned long long WifeUID; unsigned long long MaritalHomeUID; int State; bool GayMarriage; };
struct Stream {
    unsigned long long log[64]; int n;
    void WriteLong(int v) { log[n++] = (unsigned long long)(unsigned int)v; }
    void WriteUInt64(unsigned long long v) { log[n++] = v; }
    void WriteEBOOL(bool v) { log[n++] = v ? 1 : 0; }
};
struct Traits {
    CHeroMarriageInfo DefaultVal;
    void TransferBinaryOut(Stream* s, const CHeroMarriageInfo* v)
    {
        s->WriteUInt64(v->WifeUID); s->WriteUInt64(v->MaritalHomeUID); s->WriteLong(v->State); s->WriteEBOOL(v->GayMarriage);
    }
};
struct Vec { CHeroMarriageInfo* start; CHeroMarriageInfo* finish; unsigned int size() const { return (unsigned int)(finish - start); } };

static void SerialiseOut(Stream& stream, Vec& value)
{
    Traits traits;
    int count = (int)value.size();
    stream.WriteLong(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryOut(&stream, &value.start[i]);
}

int main()
{
    CHeroMarriageInfo m[2] = { { 0x1111000022220000ull, 7, 1, false }, { 42, 0x99ull << 40, 2, true } };
    Vec v = { m, m + 2 };
    Stream s; s.n = 0;
    SerialiseOut(s, v);
    bool ok = s.n == 9 && s.log[0] == 2 && s.log[1] == 0x1111000022220000ull && s.log[2] == 7 && s.log[3] == 1 &&
              s.log[4] == 0 && s.log[5] == 42 && s.log[6] == (0x99ull << 40) && s.log[7] == 2 && s.log[8] == 1;
    Vec e = { m, m };
    Stream t; t.n = 0;
    SerialiseOut(t, e);
    ok = ok && t.n == 1 && t.log[0] == 0;
    std::printf(ok ? "0057f109_TEST PASS\n" : "0057f109_TEST FAIL\n");
    return ok ? 0 : 1;
}