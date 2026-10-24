// behaviour test for 0x00BD8480 CVectorMap<pair<long,long>, CEngineTextureDecal>::LowerBound
#include <cstdio>

struct Key { int first; int second; };
static bool Less(const Key& a, const Key& b) { return a.first < b.first || (!(b.first < a.first) && a.second < b.second); }
struct Decal { int OriginalTextureIndex; int DecalID; void* Blends[3]; };
struct Entry { Key first; Decal second; };

struct DecalMap {
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const Key& key)
    {
        Entry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            Entry* middle = first + half;
            if (Less(middle->first, key)) {
                first = middle + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return first;
    }
};

static Key K(int a, int b) { Key k; k.first = a; k.second = b; return k; }

int main()
{
    Entry e[5];
    int a[5] = { -3, 1, 1, 1, 7 };
    int b[5] = { 0, -2, 4, 9, 0 };
    for (int i = 0; i < 5; ++i) { e[i].first = K(a[i], b[i]); e[i].second.OriginalTextureIndex = a[i]; e[i].second.DecalID = b[i]; }
    DecalMap m; m.First = e; m.Last = e + 5;
    if (m.LowerBound(K(-5, 0)) != e)     { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    if (m.LowerBound(K(1, -2)) != e + 1) { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    if (m.LowerBound(K(1, 5)) != e + 3)  { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    if (m.LowerBound(K(2, -100)) != e + 4) { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    if (m.LowerBound(K(7, 1)) != e + 5)  { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    m.Last = e;
    if (m.LowerBound(K(0, 0)) != e) { std::printf("00bd8480_TEST FAIL\n"); return 1; }
    std::printf("00bd8480_TEST PASS\n");
    return 0;
}