// Behaviour test for 004bbbc0: CVectorMap<CCharString,CPersistInfo,..>::Sort -- the map's entries sorted by quest name, Dirty cleared
#include <cstdio>
#include <cstring>
#include <algorithm>
class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { }
    CCharString(const char* t) : Text(t) { }
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
    bool operator==(const char* t) const { return std::strcmp(Text, t) == 0; }
};
struct CPersistInfo { CCharString QuestName; CCharString PersistScript; };
struct Entry { CCharString first; CPersistInfo second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static const char* const K[12] = { "Q_Hob", "Q_Arena", "Q_Witch", "Q_Bandit", "Q_Guild", "Q_Twin", "Q_Lady", "Q_Oak", "Q_Barrow", "Q_Lych", "Q_Snow", "Q_Dragon" };
static const char* const S[12] = { "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11" };
static void fill(Entry* e, int n)
{
    for (int i = 0; i < n; ++i) { e[i].first = CCharString(K[i % 12]); e[i].second.QuestName = CCharString(K[i % 12]); e[i].second.PersistScript = CCharString(S[i % 12]); }
}
static int sorted(const Entry* e, int n)
{
    int ok = 1;
    for (int i = 0; i < n; ++i) {
        ok &= std::strcmp(e[i].first.Text, e[i].second.QuestName.Text) == 0;
        if (i) ok &= !(e[i].first < e[i - 1].first);
    }
    return ok;
}
static void unguarded_linear_insert(Entry* last, Entry val, Less comp)
{
    Entry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (first == last) return;
    for (Entry* i = first + 1; i != last; ++i) {
        Entry val = *i;
        if (comp(val, *first)) { for (Entry* p = i; p != first; --p) *p = *(p - 1); *first = val; }
        else unguarded_linear_insert(i, val, comp);
    }
}
static void final_insertion_sort(Entry* first, Entry* last, Less comp)
{
    if (last - first > 16) { insertion_sort(first, first + 16, comp); for (Entry* i = first + 16; i != last; ++i) unguarded_linear_insert(i, *i, comp); }
    else insertion_sort(first, last, comp);
}
static void sort_rows(Entry* first, Entry* last, Less comp)   // the row: introsort (stand-in std::sort on 16-blocks) + final insertion sort
{
    if (first == last) return;
    std::sort(first, last, comp);
    final_insertion_sort(first, last, comp);
}
struct CVectorMap {
    Entry* First; Entry* Last; Entry* EndOfStorage; Less Compare; bool Dirty;
    void Sort() { sort_rows(First, Last, Compare); Dirty = false; }
    void SortIfDirty() { if (Dirty) { sort_rows(First, Last, Compare); Dirty = false; } }
};

int main()
{
    Entry e[12]; fill(e, 12);
    CVectorMap m; m.First = e; m.Last = e + 12; m.EndOfStorage = e + 12; m.Dirty = true;
    m.Sort();
    int ok = sorted(e, 12) && !m.Dirty;
    std::printf(ok ? "004bbbc0_TEST PASS\n" : "004bbbc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
