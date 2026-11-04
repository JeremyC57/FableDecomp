// Behaviour test for 0x009D5BF0 CBankFileAsync::HandleFailedAllocation(): a model over a sorted set; checks false when empty, the round-robin walk from just after the last restarted entry (wrapping), and the first finished entry with a completion pending being reset, queued and remembered.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Data { bool finished, pending; };
static bool Handle(std::set<int>& all, std::vector<Data>& d, std::set<int>& restart, int& last)
{
    size_t count = all.size(); if (!count) return false;
    std::set<int>::iterator it = all.upper_bound(last);
    for (size_t i = 0; i < count; ++i) {
        if (it == all.end()) it = all.begin();
        Data& x = d[*it];
        if (x.finished && x.pending) { x.finished = false; restart.insert(*it); last = *it; return true; }
        ++it;
    }
    return false;
}
int main()
{
    int ok = 1; std::set<int> all, rs; std::vector<Data> d(4); int last = 2;
    ok &= !Handle(all, d, rs, last);
    for (int i = 0; i < 4; ++i) { all.insert(i); d[i].finished = true; d[i].pending = (i == 1 || i == 3); }
    ok &= Handle(all, d, rs, last) && last == 3 && !d[3].finished;
    ok &= Handle(all, d, rs, last) && last == 1 && rs.size() == 2;
    ok &= !Handle(all, d, rs, last);
    std::printf(ok ? "009D5BF0_TEST PASS\n" : "009D5BF0_TEST FAIL\n"); return ok ? 0 : 1;
}
