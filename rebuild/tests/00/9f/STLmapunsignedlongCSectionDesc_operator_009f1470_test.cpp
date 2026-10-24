// Behaviour test for 0x009F1470 map<unsigned long, CSectionDesc>::operator[]: an existing handle returns its descriptor;
// a missing one inserts a default descriptor (empty strings) at the lower bound.
#include <cstdio>
#include <cstring>

struct CCharString { const char* Text; CCharString() : Text("") {} CCharString(const char* t) : Text(t) {} };
struct CSectionDesc { CCharString ShortName; CCharString LongName; bool Loaded; long Size; };

template <class _T1, class _T2> struct pair
{
    _T1 first; _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
};
template <class _Tp> inline _Tp DefaultConstructed(_Tp*) { return _Tp(); }

struct SectionMap
{
    typedef pair<unsigned long, CSectionDesc> value_type;
    value_type* Items[16];
    int Count;
    int Inserts;
    int lower_bound(const unsigned long& k) const { int i = 0; while (i < Count && Items[i]->first < k) ++i; return i; }
    int end() const { return Count; }
    int insert(int pos, const value_type& x)
    {
        ++Inserts;
        for (int j = Count; j > pos; --j) Items[j] = Items[j - 1];
        Items[pos] = new value_type(x);
        ++Count;
        return pos;
    }
    CSectionDesc& operator[](const unsigned long& k)
    {
        int i = lower_bound(k);
        if (i == end() || k < Items[i]->first)
            i = insert(i, value_type(k, DefaultConstructed((CSectionDesc*)0)));
        return Items[i]->second;
    }
};

int main()
{
    SectionMap m; m.Count = 0; m.Inserts = 0;
    m[7].ShortName = CCharString("seven"); m[7].Size = 70;
    m[0x80000000UL].ShortName = CCharString("big");
    bool ok = m.Inserts == 2 && std::strcmp(m[7].ShortName.Text, "seven") == 0 && m[7].Size == 70 && m.Inserts == 2;
    ok = ok && std::strcmp(m[3].LongName.Text, "") == 0 && m.Count == 3 && m.Items[0]->first == 3
            && m.Items[1]->first == 7 && m.Items[2]->first == 0x80000000UL;          // unsigned order
    ok = ok && std::strcmp(m[0x80000000UL].ShortName.Text, "big") == 0 && m.Inserts == 3;
    for (int i = 0; i < m.Count; ++i) delete m.Items[i];
    if (!ok) { std::printf("009f1470_TEST FAIL\n"); return 1; }
    std::printf("009f1470_TEST PASS\n");
    return 0;
}