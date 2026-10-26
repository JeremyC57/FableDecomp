// Behaviour test for 0x009EFCF0 map<char, CCharString>::operator[]: an existing key returns its value without an
// insert; a missing key inserts a default (empty) value at the lower bound and returns it.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString() : Text("") {} CCharString(const char* t) : Text(t) {} };

template <class _T1, class _T2> struct pair
{
    _T1 first; _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
};

template <class _Tp> inline _Tp DefaultConstructed(_Tp*) { return _Tp(); }

// A sorted array stands in for the tree: the iterator is an index, end() is the size.
struct CharMap
{
    typedef pair<char, CCharString> value_type;
    value_type* Items[16];
    int Count;
    int Inserts;
    int lower_bound(const char& k) const { int i = 0; while (i < Count && Items[i]->first < k) ++i; return i; }
    int end() const { return Count; }
    int insert(int pos, const value_type& x)
    {
        ++Inserts;
        for (int j = Count; j > pos; --j) Items[j] = Items[j - 1];
        Items[pos] = new value_type(x);
        ++Count;
        return pos;
    }
    CCharString& operator[](const char& k)
    {
        int i = lower_bound(k);
        if (i == end() || k < Items[i]->first)
            i = insert(i, value_type(k, DefaultConstructed((CCharString*)0)));
        return Items[i]->second;
    }
};

int main()
{
    CharMap m; m.Count = 0; m.Inserts = 0;
    m['b'] = CCharString("bee");
    m['d'] = CCharString("dee");
    bool ok = m.Inserts == 2 && std::strcmp(m['b'].Text, "bee") == 0 && m.Inserts == 2;
    ok = ok && std::strcmp(m['c'].Text, "") == 0 && m.Inserts == 3 && m.Count == 3
            && m.Items[0]->first == 'b' && m.Items[1]->first == 'c' && m.Items[2]->first == 'd';
    ok = ok && std::strcmp(m['a'].Text, "") == 0 && m.Items[0]->first == 'a' && std::strcmp(m['d'].Text, "dee") == 0;
    ok = ok && m.Inserts == 4;
    for (int i = 0; i < m.Count; ++i) delete m.Items[i];
    if (!ok) { std::printf("009efcf0_TEST FAIL\n"); return 1; }
    std::printf("009efcf0_TEST PASS\n");
    return 0;
}