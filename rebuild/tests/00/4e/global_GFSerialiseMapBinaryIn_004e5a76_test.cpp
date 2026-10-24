#include <cstdio>
// Behaviour copy of GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long> >: clear, read the count, then per
// entry a value-initialised pair {CDefString(-1), 0L} is filled key-then-value and inserted (unique keys, first wins).
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct SStream {
    const long* data; int pos; int size;
    SStream(const long* d, int n) : data(d), pos(0), size(n) { }
    void Read(long* out) { if (pos < size) *out = data[pos++]; }   // short read leaves the target untouched
};
struct SPair { CDefString first; long second; SPair() : first(), second(0) { } };
struct SMap {
    SPair items[16]; int count;
    SMap() : count(0) { }
    void clear() { count = 0; }
    bool insert(const SPair& p)
    {
        int i = 0;
        while (i < count && items[i].first.TablePos < p.first.TablePos) ++i;
        if (i < count && items[i].first.TablePos == p.first.TablePos) return false;
        for (int j = count; j > i; --j) items[j] = items[j - 1];
        items[i] = p; ++count;
        return true;
    }
};
static void SerialiseIn(SStream& stream, SMap& out)
{
    out.clear();
    long count;
    stream.Read(&count);
    for (; count > 0; --count) {
        SPair value;
        stream.Read(&value.first.TablePos);
        stream.Read(&value.second);
        out.insert(value);
    }
}
int main()
{
    bool ok = true;
    const long data[] = { 3, 7, 70, 2, 20, 7, 99 };   // count 3: (7,70) (2,20) (7,99 duplicate -> ignored)
    SStream s(data, 7);
    SMap m; m.count = 5;                               // stale contents are cleared first
    SerialiseIn(s, m);
    if (m.count != 2) ok = false;
    if (ok && (m.items[0].first.TablePos != 2 || m.items[0].second != 20)) ok = false;
    if (ok && (m.items[1].first.TablePos != 7 || m.items[1].second != 70)) ok = false;
    const long data2[] = { 2, 5 };                     // truncated: second entry keeps its defaults (-1, 0)
    SStream s2(data2, 2);
    SMap m2; SerialiseIn(s2, m2);
    if (m2.count != 2 || m2.items[0].first.TablePos != -1 || m2.items[0].second != 0 || m2.items[1].first.TablePos != 5) ok = false;
    if (!ok) { std::printf("004e5a76_TEST FAIL\n"); return 1; }
    std::printf("004e5a76_TEST PASS\n");
    return 0;
}