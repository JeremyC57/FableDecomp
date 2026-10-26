// Behaviour test for 0x008ED040 CTCContainer::RemoveItem: removes only the first item with the definition index.
#include <cstdio>

struct Item { long first; int second; };

class CTCContainer { public:
    Item  items[8];
    Item* start;
    Item* finish;
    CTCContainer() { start = items; finish = items; }
    void Add(long def, int tag) { finish->first = def; finish->second = tag; ++finish; }
    void RemoveItem(long defIndex)
    {
        Item* last = finish;
        Item* it = start;
        while (it != last && it->first != defIndex) ++it;          // __find_if
        if (it != last) {                                          // erase(it)
            if (it + 1 != last)
                for (Item* src = it + 1, *dst = it; src != last; ++src, ++dst) *dst = *src;
            --finish;
            finish->second = -1;                                   // _Destroy(finish)
        }
    }
};

int main()
{
    CTCContainer c;
    c.Add(5, 50); c.Add(7, 70); c.Add(5, 51); c.Add(9, 90);
    int fail = 0;
    c.RemoveItem(5);
    if (c.finish - c.start != 3 || c.items[0].first != 7 || c.items[1].second != 51 || c.items[2].first != 9) fail = 1;
    if (c.items[3].second != -1) fail = 1;                         // the old last slot was destroyed
    c.RemoveItem(42);                                              // absent: unchanged
    if (c.finish - c.start != 3) fail = 1;
    c.RemoveItem(9);                                               // last element: no copy
    if (c.finish - c.start != 2 || c.items[1].first != 5) fail = 1;
    if (fail) { std::printf("008ed040_TEST FAIL\n"); return 1; }
    std::printf("008ed040_TEST PASS\n");
    return 0;
}