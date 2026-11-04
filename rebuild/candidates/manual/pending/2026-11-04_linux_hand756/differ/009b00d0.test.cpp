// Behaviour test for CParentDefClassBase::AddSubDef (0x009B00D0): a model of the row -- each sub-def appends {CRC(name),
// {defIndex, parentIndex}} to the map and marks it unsorted; the map is sorted by key afterwards.
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <utility>
typedef std::pair<unsigned long, std::pair<long, long> > Entry;
static unsigned long Crc(const char* s) { unsigned long h = 0; while (*s) h = h * 31 + (unsigned char)*s++; return h; }
static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }
struct Defs { std::vector<Entry> Map; bool Dirty;
    void AddSubDef(const char* name, long def, long parent) { Map.push_back(Entry(Crc(name), std::make_pair(def, parent))); Dirty = true;
        if (Dirty) { std::sort(Map.begin(), Map.end(), Less); Dirty = false; } } };
int main()
{
    Defs d; d.Dirty = false;
    d.AddSubDef("zeta", 1, 0); d.AddSubDef("alpha", 2, 1); d.AddSubDef("mid", 3, 1);
    int ok = d.Map.size() == 3 && !d.Dirty && d.Map[0].first < d.Map[1].first && d.Map[1].first < d.Map[2].first;
    for (size_t i = 0; i < d.Map.size(); ++i) if (d.Map[i].first == Crc("alpha")) ok = ok && d.Map[i].second.first == 2 && d.Map[i].second.second == 1;
    printf(ok ? "009b00d0_TEST PASS\n" : "009b00d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
