// Behaviour test for 0x00711640: in-place stable sort (insertion sort below 15, else halves + buffer-free merge)
#include <cstdio>

struct Item { int key; int seq; };

static bool Less(const Item& a, const Item& b) { return a.key < b.key; }

static void InsertionSort(Item* first, Item* last)
{
    if (first == last) return;
    for (Item* i = first + 1; i != last; ++i) {
        Item v = *i;
        Item* j = i;
        while (j != first && Less(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

static void Rotate(Item* first, Item* middle, Item* last)
{
    // reverse-based rotation
    Item* a; Item* b; Item t;
    for (a = first, b = middle - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
    for (a = middle, b = last - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
    for (a = first, b = last - 1; a < b; ++a, --b) { t = *a; *a = *b; *b = t; }
}

static Item* LowerBound(Item* first, Item* last, const Item& v)
{
    int len = (int)(last - first);
    while (len > 0) { int h = len / 2; Item* m = first + h; if (Less(*m, v)) { first = m + 1; len -= h + 1; } else len = h; }
    return first;
}
static Item* UpperBound(Item* first, Item* last, const Item& v)
{
    int len = (int)(last - first);
    while (len > 0) { int h = len / 2; Item* m = first + h; if (Less(v, *m)) len = h; else { first = m + 1; len -= h + 1; } }
    return first;
}

static void MergeWithoutBuffer(Item* first, Item* middle, Item* last, int len1, int len2)
{
    if (len1 == 0 || len2 == 0) return;
    if (len1 + len2 == 2) { if (Less(*middle, *first)) { Item t = *first; *first = *middle; *middle = t; } return; }
    Item* cut1; Item* cut2; int l11, l22;
    if (len1 > len2) { l11 = len1 / 2; cut1 = first + l11; cut2 = LowerBound(middle, last, *cut1); l22 = (int)(cut2 - middle); }
    else { l22 = len2 / 2; cut2 = middle + l22; cut1 = UpperBound(first, middle, *cut2); l11 = (int)(cut1 - first); }
    Rotate(cut1, middle, cut2);
    Item* nm = cut1 + (cut2 - middle);
    MergeWithoutBuffer(first, cut1, nm, l11, l22);
    MergeWithoutBuffer(nm, cut2, last, len1 - l11, len2 - l22);
}

static void InplaceStableSort(Item* first, Item* last)
{
    if (last - first < 15) { InsertionSort(first, last); return; }
    Item* middle = first + (last - first) / 2;
    InplaceStableSort(first, middle);
    InplaceStableSort(middle, last);
    MergeWithoutBuffer(first, middle, last, (int)(middle - first), (int)(last - middle));
}

int main()
{
    Item a[100];
    unsigned int r = 12345u;
    for (int i = 0; i < 100; ++i) { r = r * 1103515245u + 12345u; a[i].key = (int)((r >> 16) % 7u); a[i].seq = i; }
    InplaceStableSort(a, a + 100);
    for (int i = 1; i < 100; ++i) {
        if (a[i - 1].key > a[i].key || (a[i - 1].key == a[i].key && a[i - 1].seq > a[i].seq)) {
            std::printf("00711640_TEST FAIL\n"); return 1;
        }
    }
    Item b[5] = { {3, 0}, {1, 1}, {3, 2}, {0, 3}, {1, 4} };
    InplaceStableSort(b, b + 5);
    if (b[0].seq != 3 || b[1].seq != 1 || b[2].seq != 4 || b[3].seq != 0 || b[4].seq != 2) { std::printf("00711640_TEST FAIL\n"); return 1; }
    std::printf("00711640_TEST PASS\n");
    return 0;
}