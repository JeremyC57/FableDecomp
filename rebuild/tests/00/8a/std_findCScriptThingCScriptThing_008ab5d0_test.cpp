// behaviour test for 0x008AB5D0 std::__find<CScriptThing*, CScriptThing>(first, last, val, random_access_iterator_tag):
// STLport's four-way unrolled linear find; equality is the implementations' IsEqualTo, so a distinct wrapper of the same
// implementation matches; the first match wins, `last` when none (all remainders 0..3 after the unrolled trips)
#include <cstdio>
struct Imp { int key; virtual bool IsEqualTo(const Imp& o) const { return key == o.key; } virtual ~Imp() {} };
struct ScriptThing {
    void* vptrStandIn; Imp* Data; void* Info;
    bool operator==(const ScriptThing& o) const { return Data->IsEqualTo(*o.Data); }
};
struct random_access_iterator_tag { };
static ScriptThing* Find(ScriptThing* first, ScriptThing* last, const ScriptThing& val, const random_access_iterator_tag&)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    case 0:
    default: return last;
    }
}
int main()
{
    int fails = 0;
    Imp imps[11];
    ScriptThing things[11];
    for (int i = 0; i < 11; ++i) { imps[i].key = i % 7; things[i].vptrStandIn = 0; things[i].Data = &imps[i]; things[i].Info = 0; }
    random_access_iterator_tag tag;
    for (int n = 0; n <= 11; ++n) {
        for (int k = 0; k < 8; ++k) {
            Imp probe; probe.key = k;
            ScriptThing val = { 0, &probe, 0 };
            ScriptThing* expect = things + n;
            for (int i = 0; i < n; ++i) if (imps[i].key == k) { expect = things + i; break; }
            if (Find(things, things + n, val, tag) != expect) fails |= 1;
        }
    }
    if (fails) { std::printf("008ab5d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("008ab5d0_TEST PASS\n");
    return 0;
}