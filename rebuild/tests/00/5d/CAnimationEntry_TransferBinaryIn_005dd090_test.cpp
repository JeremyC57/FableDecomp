// behaviour test for CAnimationEntry::TransferBinaryIn 0x005DD090: bank index, two names, then a count; a non-zero count
// builds a component map with one clone of the matching prototype per key (null for an unknown key, which retail then calls)
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
struct Stream { const long* p; long Read() { return *p++; } };
struct Comp { int kind; };
struct Entry { long bank, anim, group; std::vector<std::pair<long, Comp> > comps; bool hasMap; };
static void TransferBinaryIn(Entry& e, Stream& s, const std::map<long, int>& protos)
{
    e.bank = s.Read(); e.anim = s.Read(); e.group = s.Read();
    unsigned long count = (unsigned long)s.Read();
    if (count == 0) return;
    e.hasMap = true;
    e.comps.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        long key = s.Read();
        std::map<long, int>::const_iterator it = protos.lower_bound(key);
        Comp c = { (it == protos.end() || key < it->first) ? -1 : it->second };
        e.comps.push_back(std::make_pair(key, c));
    }
}
int main()
{
    std::map<long, int> protos; protos[3] = 30; protos[5] = 50;
    long data[] = { 7, 11, 12, 2, 5, 3 };
    Stream s = { data }; Entry e; e.hasMap = false;
    TransferBinaryIn(e, s, protos);
    long none[] = { 1, 2, 3, 0 }; Stream s2 = { none }; Entry f; f.hasMap = false;
    TransferBinaryIn(f, s2, protos);
    int ok = e.bank == 7 && e.anim == 11 && e.group == 12 && e.comps.size() == 2 && e.comps[0].second.kind == 50
          && e.comps[1].second.kind == 30 && !f.hasMap && f.comps.empty();
    std::printf(ok ? "005dd090_TEST PASS\n" : "005dd090_TEST FAIL\n");
    return !ok;
}