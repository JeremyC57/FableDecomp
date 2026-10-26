// Behaviour test for 0x008ED3B0 CCreateItemNamesFromIndices::operator(): appends the def name for each item index.
#include <cstdio>
#include <cstring>

struct Item { long first; const char* second; };

static const char* g_names[] = { "OBJECT_APPLE", "OBJECT_BREAD", "OBJECT_SWORD" };

class CDefString { public: long TablePos; };
static CDefString GetDefNameFromGlobalIndex(long index) { CDefString s; s.TablePos = index * 10; return s; }
static const char* GetString(long tablePos) { return g_names[tablePos / 10]; }

struct NameVector {
    const char* items[8];
    int count;
    void push_back(const char* s) { items[count++] = s; }
};

class CCreateItemNamesFromIndices { public:
    NameVector* ItemNames;
    CCreateItemNamesFromIndices(NameVector& names) : ItemNames(&names) {}
    void operator()(const Item& item)
    {
        const char* name = GetString(GetDefNameFromGlobalIndex(item.first).TablePos);
        ItemNames->push_back(name);
    }
};

int main()
{
    Item items[3] = { { 2, "" }, { 0, "" }, { 2, "x" } };
    NameVector names;
    names.count = 0;
    CCreateItemNamesFromIndices f(names);
    for (int i = 0; i < 3; ++i) f(items[i]);
    int fail = 0;
    if (names.count != 3) fail = 1;
    else if (std::strcmp(names.items[0], "OBJECT_SWORD") || std::strcmp(names.items[1], "OBJECT_APPLE")
             || std::strcmp(names.items[2], "OBJECT_SWORD")) fail = 1;
    if (fail) { std::printf("008ed3b0_TEST FAIL\n"); return 1; }
    std::printf("008ed3b0_TEST PASS\n");
    return 0;
}