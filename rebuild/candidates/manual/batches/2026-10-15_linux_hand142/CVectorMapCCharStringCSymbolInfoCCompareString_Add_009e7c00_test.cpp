#include <cstdio>
#include <cstring>
// Host model of CVectorMap<CCharString,CSymbolInfo,...>::Add: append (key, value) to the unsorted record vector, mark dirty.
struct SymbolInfo { const char* name; const char* type; long value; };
struct Record { const char* key; SymbolInfo info; };
struct VectorMap {
    Record data[8]; int count; bool compare; bool dirty;
    VectorMap() : count(0), compare(false), dirty(false) {}
    void push_back(const Record& r) { data[count++] = r; }
    void Add(const char* key, const SymbolInfo& value)
    {
        Record r; r.key = key; r.info = value;
        push_back(r);
        dirty = true;
    }
};
int main()
{
    int fail = 0;
    VectorMap m;
    SymbolInfo a = { "A", "int", 3 };
    SymbolInfo b = { "B", "float", 9 };
    m.Add("zeta", a);
    if (!m.dirty || m.count != 1 || std::strcmp(m.data[0].key, "zeta") != 0 || m.data[0].info.value != 3) fail = 1;
    m.dirty = false;
    m.Add("alpha", b);
    if (!m.dirty || m.count != 2 || std::strcmp(m.data[1].key, "alpha") != 0 || std::strcmp(m.data[1].info.type, "float") != 0 || m.data[1].info.value != 9) fail = 1;
    if (std::strcmp(m.data[0].key, "zeta") != 0) fail = 1;   // appended, not sorted
    if (fail) { std::printf("009e7c00_TEST FAIL\n"); return 1; }
    std::printf("009e7c00_TEST PASS\n");
    return 0;
}
