// Behaviour test for 0x0065FBA4 CSoundMap::GetPSoundPair (model copy: CRC key lookup in a sorted pair vector)
#include <cstdio>
struct SoundPair { long FirstSound; long LastSound; };
struct Entry { unsigned int first; SoundPair second; };
struct Map {
    Entry* First; Entry* Last;
    const Entry* LowerBound(const unsigned int& key) const
    {
        int count = int(Last - First); const Entry* first = First;
        while (count > 0) { int half = count / 2; const Entry* mid = first + half; if (mid->first < key) { first = mid + 1; count -= half + 1; } else count = half; }
        return first;
    }
    const Entry* Find(const unsigned int& key) const { const Entry* it = LowerBound(key); if (it == Last || key < it->first) return Last; return it; }
};
const SoundPair* GetPSoundPair(const Map& m, unsigned int crc)
{
    const Entry* it = m.Find(crc);
    if (it != m.Last) return &it->second;
    return 0;
}
int main()
{
    Entry e[3] = { { 5, { 1, 2 } }, { 9, { 3, 4 } }, { 20, { 5, 6 } } };
    Map m; m.First = e; m.Last = e + 3;
    if (GetPSoundPair(m, 9) != &e[1].second || GetPSoundPair(m, 20) != &e[2].second || GetPSoundPair(m, 5) != &e[0].second) { std::printf("0065fba4_TEST FAIL\n"); return 1; }
    if (GetPSoundPair(m, 7) != 0 || GetPSoundPair(m, 30) != 0 || GetPSoundPair(m, 1) != 0) { std::printf("0065fba4_TEST FAIL\n"); return 1; }
    std::printf("0065fba4_TEST PASS\n");
    return 0;
}