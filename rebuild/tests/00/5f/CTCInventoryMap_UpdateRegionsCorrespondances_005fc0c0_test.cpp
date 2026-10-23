// Behaviour test for CTCInventoryMap::UpdateRegionsCorrespondances @ 0x005FC0C0: newest-first pairing of left/entered
// messages; the (to, from) region pair is appended only when neither orientation is already recorded
#include <cstdio>
struct Pair { int first, second; };
struct Store { Pair items[16]; int count; };
static bool Has(const Store& s, int a, int b)
{
    for (int i = 0; i < s.count; ++i) if (s.items[i].first == a && s.items[i].second == b) return true;
    return false;
}
static void Update(Store& s, const int* lefts, int nl, const int* entereds, int ne)
{
    int i = nl - 1;
    for (int j = ne - 1; j >= 0; --j, --i) {
        int a = lefts[i], b = entereds[j];
        bool absent = !Has(s, a, b);
        if (!Has(s, b, a) && absent) { s.items[s.count].first = b; s.items[s.count].second = a; ++s.count; }
    }
}
int main()
{
    Store s; s.count = 0;
    int lefts[3] = { 1, 2, 3 };
    int entereds[3] = { 4, 5, 6 };
    Update(s, lefts, 3, entereds, 3);
    int ok = s.count == 3 && s.items[0].first == 6 && s.items[0].second == 3 && s.items[2].first == 4 && s.items[2].second == 1;
    int l2[2] = { 6, 7 };
    int e2[2] = { 3, 8 };
    Update(s, l2, 2, e2, 2);
    if (s.count != 4 || s.items[3].first != 8 || s.items[3].second != 7) ok = 0;
    if (!ok) { std::printf("005fc0c0_TEST FAIL\n"); return 1; }
    std::printf("005fc0c0_TEST PASS\n");
    return 0;
}