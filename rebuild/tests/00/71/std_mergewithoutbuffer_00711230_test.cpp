// Behaviour test for 0x00711230 std::__merge_without_buffer over anim-set iterators by priority: two sorted runs become one
// sorted run, stably (equal priorities keep their run order), for unequal run lengths both ways and the 2-element case.
#include <cstdio>
#include <algorithm>
struct Item { long Priority; int Tag; };
static bool Less(const Item& a, const Item& b) { return a.Priority < b.Priority; }
static Item* LowerBound(Item* f, Item* l, const Item& v) { return std::lower_bound(f, l, v, Less); }
static Item* UpperBound(Item* f, Item* l, const Item& v) { return std::upper_bound(f, l, v, Less); }
static void Merge(Item* first, Item* middle, Item* last, int len1, int len2)
{
    while (len1 != 0 && len2 != 0) {
        if (len1 + len2 == 2) { if (Less(*middle, *first)) std::swap(*first, *middle); return; }
        Item* first_cut = first; Item* second_cut = middle; int len11 = 0, len22 = 0;
        if (len1 > len2) { len11 = len1 / 2; first_cut += len11; second_cut = LowerBound(middle, last, *first_cut); len22 += second_cut - middle; }
        else { len22 = len2 / 2; second_cut += len22; first_cut = UpperBound(first, middle, *second_cut); len11 += first_cut - first; }
        std::rotate(first_cut, middle, second_cut);
        Item* new_middle = first_cut + (second_cut - middle);
        Merge(first, first_cut, new_middle, len11, len22);
        first = new_middle; middle = second_cut; len1 -= len11; len2 -= len22;
    }
}
static bool Sorted(const Item* a, int n)
{
    for (int i = 1; i < n; ++i) {
        if (a[i - 1].Priority > a[i].Priority) return false;
        if (a[i - 1].Priority == a[i].Priority && a[i - 1].Tag > a[i].Tag) return false;   // stability
    }
    return true;
}
int main()
{
    Item a[] = { { 1, 0 }, { 3, 1 }, { 5, 2 }, { 7, 3 }, { 9, 4 }, { 2, 5 }, { 3, 6 }, { 8, 7 } };
    Merge(a, a + 5, a + 8, 5, 3);
    Item b[] = { { 4, 0 }, { 6, 1 }, { 1, 2 }, { 2, 3 }, { 4, 4 }, { 5, 5 }, { 9, 6 } };
    Merge(b, b + 2, b + 7, 2, 5);
    Item c[] = { { 5, 0 }, { 2, 1 } };
    Merge(c, c + 1, c + 2, 1, 1);
    bool ok = Sorted(a, 8) && Sorted(b, 7) && c[0].Priority == 2 && c[1].Priority == 5;
    if (!ok) { std::printf("00711230_TEST FAIL\n"); return 1; }
    std::printf("00711230_TEST PASS\n");
    return 0;
}