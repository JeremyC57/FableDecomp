// Behaviour test for CTCInventoryTrade::RefreshBuyPossibilityNumbersPC @ 0x005A8175: the buy counts are recounted from the buy records -- a record
// counts when it is in stock, affordable, and the hero has room for it or it is a not-yet-owned special; a special adds to the
// specials total (only when not already owned), anything else to its menu branch and its clothes / weapons / items total. The branch
// captions get " x N" only when N > 0; a normal shop writes the category and buy totals, a special shop the specials total.
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <sstream>
struct Record { long def, count, price, type, branch; bool room, special, owned; };
struct Counts { long clothes, weapons, items, specials; std::map<long, long> perBranch; };
static const bool kPC = true;
static std::string Caption(const std::string& name, long n)
{
    std::ostringstream o; o << name;
    if (n > 0) o << " x " << n;
    return o.str();
}
static Counts Recount(const std::vector<Record>& records, long money, std::map<long, long>& branches)
{
    Counts c; c.clothes = c.weapons = c.items = 0; c.specials = 0;
    for (std::map<long, long>::iterator b = branches.begin(); b != branches.end(); ++b) b->second = 0;
    for (unsigned int i = 0; i < records.size(); ++i) {
        const Record& r = records[i];
        bool soldOut = (r.count == 0);
        if (!soldOut && money >= r.price && (r.room || (r.special && !r.owned))) {
            if (r.special && (!kPC || !r.owned)) {
                c.specials += r.count;
            } else {
                branches[r.branch] += r.count;
                switch (r.type) {
                case 0: c.clothes += r.count; break;
                case 1: c.weapons += r.count; break;
                case 2: c.items += r.count; break;
                }
            }
        }
    }
    c.perBranch = branches;
    return c;
}
int main()
{
    int ok = 1;
    std::vector<Record> v;
    Record r1 = { 1, 2, 50, 1, 10, true, false, false };    // weapons, branch 10
    Record r2 = { 2, 3, 20, 0, 11, true, false, false };    // clothes, branch 11
    Record r3 = { 3, 1, 20, 2, 12, true, false, false };    // items, branch 12
    Record r4 = { 4, 5, 900, 2, 12, true, false, false };   // too dear
    Record r5 = { 5, 0, 10, 1, 10, true, false, false };    // sold out
    Record r6 = { 6, 1, 10, 2, 12, false, true, false };    // special, not owned, no room
    Record r7 = { 7, 1, 10, 2, 12, false, false, false };   // no room, not special
    v.push_back(r1); v.push_back(r2); v.push_back(r3); v.push_back(r4); v.push_back(r5); v.push_back(r6); v.push_back(r7);
    std::map<long, long> branches; branches[10] = 7; branches[13] = 4;     // stale counts are reset
    Counts c = Recount(v, 100, branches);
    if (c.weapons != 2 || c.clothes != 3 || c.items != 1 || c.specials != 1) ok = 0;
    if (c.perBranch[10] != 2 || c.perBranch[11] != 3 || c.perBranch[12] != 1 || c.perBranch[13] != 0) ok = 0;
    if (c.weapons + c.items + c.clothes != 6) ok = 0;
    if (Caption("Melee", c.perBranch[10]) != "Melee x 2" || Caption("Feet", c.perBranch[13]) != "Feet") ok = 0;
    if (!ok) { std::printf("005a8175_TEST FAIL\n"); return 1; }
    std::printf("005a8175_TEST PASS\n");
    return 0;
}