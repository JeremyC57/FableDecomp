// Behaviour test for 0x00758220 std::fill over the 0x2C-byte wound-morph entries
#include <cstdio>

enum EBodyLocation { LOC0 = 0, LOC4 = 4 };
struct CWoundMorphsEntry { long Words[10]; };
template <class A, class B> struct pair { A first; B second; };
typedef pair<EBodyLocation, CWoundMorphsEntry> TWoundPair;

void fill(TWoundPair* first, TWoundPair* last, const TWoundPair& val)
{
    for (; first != last; ++first)
        *first = val;
}

int main()
{
    TWoundPair a[5];
    for (int i = 0; i < 5; ++i) { a[i].first = LOC0; for (int k = 0; k < 10; ++k) a[i].second.Words[k] = i * 100 + k; }
    TWoundPair v;
    v.first = LOC4;
    for (int k = 0; k < 10; ++k) v.second.Words[k] = 7000 + k;
    int ok = 1;
    fill(a + 1, a + 4, v);
    for (int i = 0; i < 5; ++i) {
        bool filled = i >= 1 && i < 4;
        if (filled) {
            if (a[i].first != LOC4) ok = 0;
            for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != 7000 + k) ok = 0;
        } else {
            if (a[i].first != LOC0) ok = 0;
            for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != i * 100 + k) ok = 0;
        }
    }
    fill(a + 2, a + 2, a[0]);              // empty range: nothing written
    if (a[2].first != LOC4) ok = 0;
    if (!ok) { std::printf("00758220_TEST FAIL\n"); return 1; }
    std::printf("00758220_TEST PASS\n");
    return 0;
}