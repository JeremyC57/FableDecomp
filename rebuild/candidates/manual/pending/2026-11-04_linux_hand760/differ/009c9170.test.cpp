// Behaviour test for 0x009C9170 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Entry { int Type; std::vector<unsigned long> Members; };
static int Validate(const std::vector<Entry>& e) {
    int checked = 0;
    for (unsigned long i = 1; i < e.size(); ++i) if (e[i].Type == 1) for (unsigned long j = 0; j < e[i].Members.size(); ++j) if (e[e[i].Members[j]].Type == 0) ++checked;
    return checked;
}
int main() {
    std::vector<Entry> e(4); e[0].Type = 1; e[1].Type = 1; e[1].Members.push_back(2); e[1].Members.push_back(3); e[2].Type = 0; e[3].Type = 0;
    e[0].Members.push_back(2);                              // entry 0 is skipped
    bool ok = Validate(e) == 2;
    std::printf(ok ? "009c9170_TEST PASS\n" : "009c9170_TEST FAIL\n");
    return ok ? 0 : 1;
}
