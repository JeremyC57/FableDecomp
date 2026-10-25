// Behaviour test for 0x00870530 std::__find_if over CThingComponentSet::CEntry with CPred_EntryIsDriver
// The model mirrors the source: STLport 4x-unrolled random-access find_if over packed 9-byte entries; the predicate maps
// the entry's def-string index to a name, looks the registered TC info up (null when unregistered) and accepts a
// non-zero driver type. Every converted name temporary must be released.
#include <cstdio>
#include <cstring>

static int g_live_strings = 0;
static int g_lookups = 0;

struct Name {                                   // stands in for the CCharString temporary
    int index;
    explicit Name(int i) : index(i) { ++g_live_strings; }
    Name(const Name& o) : index(o.index) { ++g_live_strings; }
    ~Name() { --g_live_strings; }
};

struct RegisteredTCInfo { void* alloc; unsigned long driver_type; };
static RegisteredTCInfo g_infos[8];

static const RegisteredTCInfo* GetPTCInfo(const Name& name)       // index 0..7 registered, else unknown
{
    ++g_lookups;
    if (name.index < 0 || name.index >= 8) return 0;
    return &g_infos[name.index];
}

#pragma pack(push, 1)
struct CEntry { int name_pos; unsigned int driver_type; bool override_flag; };   // 32-bit fields: 9 bytes
#pragma pack(pop)

struct Pred {
    bool operator()(const CEntry& entry) const
    {
        const RegisteredTCInfo* info = GetPTCInfo(Name((int)entry.name_pos));
        if (info == 0) return false;
        return info->driver_type != 0;
    }
};

template <class It, class P> It find_if4(It first, It last, P pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first;
    case 0:
    default: return last;
    }
}

int main()
{
    int fail = 0;
    if (sizeof(CEntry) != 9) fail |= 1;
    for (int i = 0; i < 8; ++i) { g_infos[i].alloc = 0; g_infos[i].driver_type = (i == 5 || i == 7) ? 3 : 0; }
    CEntry entries[11];
    static const int pos[11] = { 1, 20, 2, 3, 0, 40, 6, 5, 1, 7, 9 };   // 20/40/9 unregistered; 5 at [7], 7 at [9]
    for (int i = 0; i < 11; ++i) { entries[i].name_pos = pos[i]; entries[i].driver_type = 99; entries[i].override_flag = true; }
    Pred p;
    for (int n = 0; n <= 11; ++n) {
        const CEntry* r = find_if4((const CEntry*)entries, (const CEntry*)entries + n, p);
        int want = n;
        for (int i = 0; i < n; ++i) if (pos[i] == 5 || pos[i] == 7) { want = i; break; }
        if (r != entries + want) fail |= 2;
    }
    g_lookups = 0;
    if (find_if4((const CEntry*)entries, (const CEntry*)entries + 11, p) != entries + 7) fail |= 4;
    if (g_lookups != 8) fail |= 8;                       // stops at the first driver
    g_infos[5].driver_type = 0; g_infos[7].driver_type = 0;
    if (find_if4((const CEntry*)entries, (const CEntry*)entries + 11, p) != entries + 11) fail |= 16;
    if (g_live_strings != 0) fail |= 32;
    if (fail) { std::printf("00870530_TEST FAIL\n"); return 1; }
    std::printf("00870530_TEST PASS\n");
    return 0;
}