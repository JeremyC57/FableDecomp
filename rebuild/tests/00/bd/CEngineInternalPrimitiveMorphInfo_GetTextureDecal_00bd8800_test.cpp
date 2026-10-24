// behaviour test for 0x00BD8800 CEngineInternalPrimitiveMorphInfo::GetTextureDecal(long, long)
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
            if (Less(middle->first, key)) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    Entry* Find(const Key& key) { Entry* it = LowerBound(key); return (it == Last || Less(key, it->first)) ? Last : it; }
};

struct MorphInfo {
    DecalMap TextureDecals;
    Decal* GetTextureDecal(int originalTextureIndex, int decalID)
    {
        Key k; k.first = originalTextureIndex; k.second = decalID;
        Entry* it = TextureDecals.Find(k);
        return it != TextureDecals.Last ? &it->second : 0;
    }
};

int main()
{
    Entry e[4];
    int a[4] = { 0, 2, 2, 5 };
    int b[4] = { 1, 0, 3, 3 };
    for (int i = 0; i < 4; ++i) { e[i].first.first = a[i]; e[i].first.second = b[i]; e[i].second.OriginalTextureIndex = a[i]; e[i].second.DecalID = b[i]; }
    MorphInfo m; m.TextureDecals.First = e; m.TextureDecals.Last = e + 4;
    if (m.GetTextureDecal(2, 3) != &e[2].second) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    if (m.GetTextureDecal(0, 1) != &e[0].second) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    if (m.GetTextureDecal(5, 3) != &e[3].second) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    if (m.GetTextureDecal(2, 1) != 0) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    if (m.GetTextureDecal(9, 0) != 0) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    if (m.GetTextureDecal(-1, 0) != 0) { std::printf("00bd8800_TEST FAIL\n"); return 1; }
    std::printf("00bd8800_TEST PASS\n");
    return 0;
}