#include <cstdio>
struct CComponent { int id; };
struct Entry { long first; CComponent* second; Entry(long p, CComponent* c) : first(p), second(c) {} Entry() : first(0), second(0) {} };
struct EntryVector { Entry buf[4]; int n; void push_back(const Entry& e) { buf[n++] = e; } };
struct Clothing {
    EntryVector ClothingMenuEntries;
    void AddClothingMenuEntry(long part, CComponent* component) { ClothingMenuEntries.push_back(Entry(part, component)); }
};
int main()
{
    CComponent a, b; Clothing c; c.ClothingMenuEntries.n = 0;
    c.AddClothingMenuEntry(3, &a); c.AddClothingMenuEntry(7, &b);
    if (c.ClothingMenuEntries.n != 2 || c.ClothingMenuEntries.buf[0].first != 3 || c.ClothingMenuEntries.buf[0].second != &a ||
        c.ClothingMenuEntries.buf[1].first != 7 || c.ClothingMenuEntries.buf[1].second != &b) { std::printf("005b4771_TEST FAIL\n"); return 1; }
    std::printf("005b4771_TEST PASS\n");
    return 0;
}