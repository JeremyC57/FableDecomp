// Behaviour test for CPersistContext::TransferMap<pair<long, long>, CCharString> @ 0x00756D00.
// A copy of the row's logic over plain host types: the mode switch (4 clear, 3 binary save, 2 binary load, 1 text save,
// default text load), the text-save line format "name[key] (first,second);\r\n", and the text-load value choice
// (zeroed traits DefaultVal, overwritten by the old entry or by the default when UseDefaults, then transferred in place;
// the entry is erased and re-inserted).
#include <cstdio>
#include <cstring>

struct LongPair { long first; long second; };
struct ValueTraits {                    // CPersistTraits<pair<long,long> >: DefaultVal at +0
    LongPair DefaultVal;
    // TransferIn: parse "(a,b)" from *text; on a parse failure the value keeps what it had (the default)
    void TransferIn(const char** text, LongPair* value)
    {
        long a, b; int used = 0;
        if (std::sscanf(*text, " (%ld , %ld )%n", &a, &b, &used) == 2 && used > 0) {
            value->first = a; value->second = b; *text += used;
        }
    }
};

enum { MAX_ENTRIES = 8 };
struct Map {
    char keys[MAX_ENTRIES][16];
    LongPair values[MAX_ENTRIES];
    int count;
    int find(const char* k) const { for (int i = 0; i < count; ++i) if (std::strcmp(keys[i], k) == 0) return i; return -1; }
    void erase(const char* k)
    {
        int i = find(k);
        if (i < 0) return;
        for (int j = i; j + 1 < count; ++j) { std::strcpy(keys[j], keys[j + 1]); values[j] = values[j + 1]; }
        --count;
    }
    void insert(const char* k, const LongPair& v) { if (find(k) >= 0) return; std::strcpy(keys[count], k); values[count] = v; ++count; }
};

struct Context {
    long Mode;
    bool UseDefaults;
    int binaryLoads, binarySaves;
    char out[256];
};

// one text-load entry: key already parsed, `text` points at the value
static void LoadEntry(Context& c, Map& m, const char* key, const char* text, const LongPair& defaultValue)
{
    ValueTraits valueTraits;
    valueTraits.DefaultVal.first = 0;
    valueTraits.DefaultVal.second = 0;
    int old = m.find(key);
    if (old >= 0)
        valueTraits.DefaultVal = m.values[old];
    else if (c.UseDefaults)
        valueTraits.DefaultVal = defaultValue;
    valueTraits.TransferIn(&text, &valueTraits.DefaultVal);
    m.erase(key);
    m.insert(key, valueTraits.DefaultVal);
}

static void TransferMap(Context& c, const char* name, Map& m, const LongPair& defaultValue, const char* key, const char* text)
{
    switch (c.Mode) {
    case 4: m.count = 0; return;
    case 3: ++c.binarySaves; return;
    case 2: ++c.binaryLoads; return;
    case 1: {
        c.out[0] = 0;
        for (int i = 0; i < m.count; ++i) {
            char line[64];
            std::sprintf(line, "%s[%s] (%ld,%ld);\r\n", name, m.keys[i], m.values[i].first, m.values[i].second);
            std::strcat(c.out, line);
        }
        return;
    }
    default:
        LoadEntry(c, m, key, text, defaultValue);
        return;
    }
}

int main()
{
    Context c; c.binaryLoads = c.binarySaves = 0; c.UseDefaults = false;
    Map m; m.count = 0;
    LongPair def; def.first = 7; def.second = 9;
    // text load, new key, no defaults: zeroed then parsed
    c.Mode = 0;
    TransferMap(c, "Crimes", m, def, "Oakvale", "(3,4);");
    if (m.count != 1 || m.values[0].first != 3 || m.values[0].second != 4) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    // unparsable value, new key, no defaults -> zero
    TransferMap(c, "Crimes", m, def, "Bowerstone", "junk;");
    if (m.count != 2 || m.values[m.find("Bowerstone")].first != 0 || m.values[m.find("Bowerstone")].second != 0) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    // unparsable value, new key, UseDefaults -> the default
    c.UseDefaults = true;
    TransferMap(c, "Crimes", m, def, "Knothole", "junk;");
    if (m.values[m.find("Knothole")].first != 7 || m.values[m.find("Knothole")].second != 9) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    // unparsable value, existing key -> the old entry survives the erase/re-insert
    TransferMap(c, "Crimes", m, def, "Oakvale", "junk;");
    if (m.count != 3 || m.values[m.find("Oakvale")].first != 3 || m.values[m.find("Oakvale")].second != 4) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    // text save
    Map s; s.count = 0; LongPair v; v.first = 1; v.second = 2; s.insert("A", v);
    c.Mode = 1; TransferMap(c, "M", s, def, 0, 0);
    if (std::strcmp(c.out, "M[A] (1,2);\r\n") != 0) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    // binary modes and clear
    c.Mode = 2; TransferMap(c, "M", s, def, 0, 0);
    c.Mode = 3; TransferMap(c, "M", s, def, 0, 0);
    if (c.binaryLoads != 1 || c.binarySaves != 1 || s.count != 1) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    c.Mode = 4; TransferMap(c, "M", m, def, 0, 0);
    if (m.count != 0) { std::printf("00756d00_TEST FAIL\n"); return 1; }
    std::printf("00756d00_TEST PASS\n");
    return 0;
}