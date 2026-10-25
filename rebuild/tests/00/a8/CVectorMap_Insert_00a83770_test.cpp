// Behaviour test for 0x00A83770 CVectorMap<unsigned __int64, bool>::Insert: append the (key, value) pair at the end
// (in place while there is capacity, else through the grow path) and mark the map dirty.
#include <cstdio>

struct Entry { unsigned int keyLo, keyHi; bool value; };

struct Map {
    Entry items[8];
    unsigned int count, capacity;
    int grows;
    bool Dirty;
    void insert_overflow(const Entry& x) { ++grows; capacity *= 2; items[count++] = x; }
    void push_back(const Entry& x)
    {
        if (count != capacity) { items[count] = x; ++count; }
        else insert_overflow(x);
    }
    void Insert(unsigned int lo, unsigned int hi, bool value)
    {
        Entry e; e.keyLo = lo; e.keyHi = hi; e.value = value;
        push_back(e);
        Dirty = true;
    }
};

int main()
{
    Map m; m.count = 0; m.capacity = 2; m.grows = 0; m.Dirty = false;
    int ok = 1;
    m.Insert(1, 0x10, true);
    if (m.count != 1 || !m.Dirty || m.grows != 0) ok = 0;
    m.Dirty = false;
    m.Insert(2, 0x20, false);
    m.Insert(3, 0x30, true);                                 // full: grows
    if (m.count != 3 || m.grows != 1 || !m.Dirty) ok = 0;
    if (m.items[0].keyHi != 0x10 || !m.items[0].value || m.items[1].value || m.items[2].keyLo != 3 || !m.items[2].value) ok = 0;
    std::printf(ok ? "00a83770_TEST PASS\n" : "00a83770_TEST FAIL\n");
    return ok ? 0 : 1;
}