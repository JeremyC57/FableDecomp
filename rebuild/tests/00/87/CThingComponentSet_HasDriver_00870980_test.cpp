#include <cstdio>
// behaviour test for CThingComponentSet::HasDriver (0x00870980)
#pragma pack(push, 1)
struct CDefString { int TablePos; CDefString() : TablePos(-1) { } };
struct CEntry {
    CDefString Name;
    unsigned int DriverType;
    bool Override;
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CIsDriverEntry {
    bool operator()(const CEntry& entry) const { return entry.DriverType != 0; }   // stand-in for the def lookup
};
CEntry* find_if_entries(CEntry* first, CEntry* last, CIsDriverEntry pred, const random_access_iterator_tag& tag)
{
    for (; first != last; ++first)
        if (pred(*first)) return first;
    return last;
}
inline CEntry* find_if(CEntry* first, CEntry* last, CIsDriverEntry pred)
{
    return find_if_entries(first, last, pred, random_access_iterator_tag());
}
template<class T, int N> struct CSmallVector {
    T* Start;
    signed char Size;
    T* begin() const { return Start; }
    T* end() const { return Start + Size; }
};
struct CThingComponentSet {
    CSmallVector<CEntry, 8> Entries;
    bool HasDriver() const;
};
bool CThingComponentSet::HasDriver() const
{
    CEntry* found = find_if(Entries.begin(), Entries.end(), CIsDriverEntry());
    return found != Entries.end();
}
int main()
{
    CEntry e[3];
    CThingComponentSet set; set.Entries.Start = e; set.Entries.Size = 3;
    if (set.HasDriver()) { std::printf("00870980_TEST FAIL\n"); return 1; }
    e[2].DriverType = 4;
    if (!set.HasDriver()) { std::printf("00870980_TEST FAIL\n"); return 1; }
    set.Entries.Size = 2;
    if (set.HasDriver()) { std::printf("00870980_TEST FAIL\n"); return 1; }
    std::printf("00870980_TEST PASS\n");
    return 0;
}