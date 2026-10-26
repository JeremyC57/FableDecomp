// Behaviour test for 0x00467090 GFSerialiseVectorBinaryOut<CBrainBehaviour>: traits built once first, then the count, then
// each behaviour's name table position and the two priorities as longs
#include <cstdio>
static int g_traitsBuilt = 0;
struct Writer { long out[16]; int n; Writer() : n(0) { } void WriteLong(long v) { out[n++] = v; } };
class CDefString { public: long TablePos; void TransferBinaryOut(Writer& s) const { s.WriteLong(TablePos); } };
class CBrainBehaviour { public: CDefString Name; long Priority; long ScriptAIPriority; };
struct BehaviourVector { CBrainBehaviour* _M_start; CBrainBehaviour* _M_finish; CBrainBehaviour* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } CBrainBehaviour& operator[](unsigned int i) { return _M_start[i]; } };
struct BehaviourTraits {
    CBrainBehaviour DefaultVal;
    BehaviourTraits() { DefaultVal.Name.TablePos = -1; DefaultVal.Priority = DefaultVal.ScriptAIPriority = 0; ++g_traitsBuilt; }
    void TransferBinaryOut(Writer& s, const CBrainBehaviour* v) const { v->Name.TransferBinaryOut(s); s.WriteLong(v->Priority); s.WriteLong(v->ScriptAIPriority); }
};
static void GFSerialiseVectorBinaryOut(Writer& stream, BehaviourVector& value)
{
    BehaviourTraits traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i) traits.TransferBinaryOut(stream, &value[i]);
}
int main()
{
    CBrainBehaviour b[3] = { { { 7 }, 1, 2 }, { { 8 }, 3, 4 }, { { -1 }, 5, 6 } };
    BehaviourVector v = { b, b + 3, b + 3 };
    Writer w; GFSerialiseVectorBinaryOut(w, v);
    bool ok = g_traitsBuilt == 1 && w.n == 10 && w.out[0] == 3 && w.out[1] == 7 && w.out[2] == 1 && w.out[3] == 2;
    ok = ok && w.out[4] == 8 && w.out[7] == -1 && w.out[8] == 5 && w.out[9] == 6;
    if (!ok) { std::printf("00467090_TEST FAIL\n"); return 1; }
    std::printf("00467090_TEST PASS\n");
    return 0;
}