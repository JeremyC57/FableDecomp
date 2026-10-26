// Behaviour test for 0x008ED440 CCreateIndicesFromItemNames::operator(): appends (def index, "") per name.
#include <cstdio>
#include <cstring>

struct Item { long first; const char* second; };

static long GetDefGlobalIndexFromName(const char* name)
{
    if (!std::strcmp(name, "OBJECT_APPLE")) return 11;
    if (!std::strcmp(name, "OBJECT_SWORD")) return 42;
    return -1;
}

struct ItemVector {
    Item items[8];
    int  count;
    void push_back(const Item& x) { items[count++] = x; }
};

class CCreateIndicesFromItemNames { public:
    ItemVector* ItemIndices;
    CCreateIndicesFromItemNames(ItemVector& v) : ItemIndices(&v) {}
    void operator()(const char* name)
    {
        long defIndex = GetDefGlobalIndexFromName(name);
        Item item = { defIndex, "" };
        ItemIndices->push_back(item);
    }
};

int main()
{
    ItemVector v;
    v.count = 0;
    CCreateIndicesFromItemNames f(v);
    f("OBJECT_SWORD"); f("OBJECT_APPLE"); f("NOT_A_DEF");
    int fail = 0;
    if (v.count != 3 || v.items[0].first != 42 || v.items[1].first != 11 || v.items[2].first != -1) fail = 1;
    if (v.items[0].second[0] != 0 || v.items[2].second[0] != 0) fail = 1;
    if (fail) { std::printf("008ed440_TEST FAIL\n"); return 1; }
    std::printf("008ed440_TEST PASS\n");
    return 0;
}