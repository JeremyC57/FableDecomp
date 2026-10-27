// behaviour test for 0x0041cd13 CAnimFilterNoGroup::operator()(const CAnimationEntry&) const: true exactly when the
// entry has no group (GroupName.TablePos == -1), through the virtual predicate slot
#include <cstdio>
struct Entry { long BankIndex; long AnimName; long GroupName; };
struct FilterBase { virtual bool operator()(const Entry&) const { return true; } virtual ~FilterBase() { } };
struct NoGroup : FilterBase { virtual bool operator()(const Entry& e) const { return e.GroupName == -1; } };
int main()
{
    int fails = 0;
    NoGroup f; const FilterBase& b = f;
    Entry e; e.BankIndex = 1; e.AnimName = 4; e.GroupName = -1;
    if (!b(e)) fails |= 1;
    e.GroupName = 0;
    if (b(e)) fails |= 2;
    e.GroupName = 12;
    if (b(e)) fails |= 4;
    if (fails) { std::printf("0041cd13_TEST FAIL %d\n", fails); return 1; }
    std::printf("0041cd13_TEST PASS\n");
    return 0;
}