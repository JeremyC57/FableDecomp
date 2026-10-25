// Behaviour test for 0x00CB8170 CScriptBase::RunEntityThreads: over a snapshot of the sorted entity-script map, give
// each script one frame; every script whose frame reports "finished" is erased (lower..upper bound range) from the
// live map, the snapshot itself is untouched and released at the end.
#include <cstdio>

struct Entry { unsigned long key; int script; };

struct Map {
    Entry items[16];
    unsigned int count;
    unsigned int LowerBound(unsigned long key) const
    {
        unsigned int first = 0, n = count;
        while (n > 0) {
            unsigned int half = n / 2;
            if (items[first + half].key < key) { first += half + 1; n -= half + 1; }
            else n = half;
        }
        return first;
    }
    unsigned int UpperBound(unsigned long key) const
    {
        unsigned int first = 0, n = count;
        while (n > 0) {
            unsigned int half = n / 2;
            if (!(key < items[first + half].key)) { first += half + 1; n -= half + 1; }
            else n = half;
        }
        return first;
    }
    void Erase(unsigned long key)
    {
        unsigned int lower = LowerBound(key);
        if (lower != count) {
            unsigned int upper = UpperBound(key);
            unsigned int out = lower;
            for (unsigned int i = upper; i < count; ++i) items[out++] = items[i];
            count = out;
        }
    }
};

static int g_frames[16];
static int g_calls = 0;

struct CScriptBase {
    Map EntityScripts;
    bool UpdateSpawnedFunction(int script) { g_frames[g_calls++] = script; return (script % 2) == 0; }
    void RunEntityThreads()
    {
        Map scripts = EntityScripts;                 // snapshot
        for (unsigned int i = 0; i != scripts.count; ++i)
            if (UpdateSpawnedFunction(scripts.items[i].script))
                EntityScripts.Erase(scripts.items[i].key);
    }
};

int main()
{
    CScriptBase base;
    base.EntityScripts.count = 5;
    unsigned long keys[5] = { 3, 7, 7, 12, 20 };
    int scripts[5] = { 1, 2, 3, 5, 8 };
    for (unsigned int i = 0; i < 5; ++i) { base.EntityScripts.items[i].key = keys[i]; base.EntityScripts.items[i].script = scripts[i]; }
    base.RunEntityThreads();
    int ok = 1;
    if (g_calls != 5) ok = 0;                                           // every snapshot entry ran once
    for (unsigned int i = 0; i < 5 && ok; ++i) if (g_frames[i] != scripts[i]) ok = 0;
    if (base.EntityScripts.count != 2) ok = 0;                          // key 7 (both entries) and key 20 erased
    if (ok && (base.EntityScripts.items[0].key != 3 || base.EntityScripts.items[1].key != 12)) ok = 0;
    if (ok && (base.EntityScripts.items[0].script != 1 || base.EntityScripts.items[1].script != 5)) ok = 0;
    base.EntityScripts.Erase(99);                                       // absent key: no change
    if (base.EntityScripts.count != 2) ok = 0;
    std::printf(ok ? "00cb8170_TEST PASS\n" : "00cb8170_TEST FAIL\n");
    return ok ? 0 : 1;
}