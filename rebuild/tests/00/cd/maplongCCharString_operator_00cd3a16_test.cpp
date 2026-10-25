// Behaviour test for map<long, CCharString>::operator[] (0x00CD3A16): lower_bound the key; when it is end() or a
// larger key, insert (key, default value) at that hint; return the mapped value.
#include <cstdio>

struct CCharString { int id; CCharString() : id(0) { } };
struct CEntry { long first; CCharString second; };
struct CLongStringMap {                         // a sorted array standing in for the tree
    CEntry entries[8];
    int count;
    int inserts;
    int lower_bound(long k) const { int i = 0; while (i < count && entries[i].first < k) ++i; return i; }
    int end() const { return count; }
    bool key_comp(long a, long b) const { return a < b; }
    int insert(int position, const CEntry& x)
    {
        ++inserts;
        for (int i = count; i > position; --i) entries[i] = entries[i - 1];
        entries[position] = x;
        ++count;
        return position;
    }
    CCharString& operator[](long k)
    {
        int i = lower_bound(k);
        if (i == end() || key_comp(k, entries[i].first)) {
            CEntry value;
            value.first = k;
            i = insert(i, value);
        }
        return entries[i].second;
    }
};

int main()
{
    CLongStringMap m;
    m.count = 0; m.inserts = 0;
    m[5].id = 50;
    m[2].id = 20;
    m[9].id = 90;
    int again = m[5].id;
    bool fresh = m[7].id == 0;
    if (m.count != 4 || m.inserts != 4 || again != 50 || !fresh || m.entries[0].first != 2 || m.entries[1].first != 5
        || m.entries[2].first != 7 || m.entries[3].first != 9 || m.entries[3].second.id != 90) {
        std::printf("00cd3a16_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd3a16_TEST PASS\n");
    return 0;
}