// Behaviour test for 0x008709C0: CThingComponentSet::GetDriverInfo returns the first entry the driver predicate accepts.
#include <cstdio>

struct CEntry { int NameTablePos; unsigned int DriverType; bool Override; };
class CPred_EntryIsDriver { public: bool operator()(const CEntry& e) const { return e.DriverType != 0u; } };

template <class It, class Pred> It find_if_model(It first, It last, Pred pred)
{
    for (; first != last; ++first) if (pred(*first)) break;
    return first;
}

class CThingComponentSet
{ public:
    CEntry* Start;
    signed char Size;
    const CEntry& GetDriverInfo() const { const CEntry* begin = Start; return *find_if_model(begin, begin + Size, CPred_EntryIsDriver()); }
    bool HasDriver() const { const CEntry* begin = Start; return find_if_model(begin, begin + Size, CPred_EntryIsDriver()) != begin + Size; }
};

int main()
{
    CEntry e[4] = { { 1, 0, false }, { 2, 0, false }, { 3, 7, true }, { 4, 9, false } };
    CThingComponentSet set; set.Start = e; set.Size = 4;
    int fails = 0;
    if (&set.GetDriverInfo() != &e[2] || set.GetDriverInfo().NameTablePos != 3) ++fails;
    if (!set.HasDriver()) ++fails;
    set.Size = 2;
    if (set.HasDriver()) ++fails;
    if (fails) { std::printf("008709c0_TEST FAIL\n"); return 1; }
    std::printf("008709c0_TEST PASS\n");
    return 0;
}