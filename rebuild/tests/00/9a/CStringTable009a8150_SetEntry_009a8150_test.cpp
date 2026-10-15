#include <cstdio>
#include <cstring>
class CCharString {
public:
    char text[32];
    CCharString& operator=(const CCharString& o) { std::memcpy(text, o.text, sizeof(text)); return *this; }
};
struct CStringMap {                               // a tiny two-slot map keyed by text
    CCharString keys[2]; CCharString values[2]; int used;
    CCharString& operator[](const CCharString& key)
    {
        for (int i = 0; i < used; ++i) if (!std::strcmp(keys[i].text, key.text)) return values[i];
        keys[used] = key; values[used].text[0] = 0; return values[used++];
    }
};
struct CStringTable {
    CStringMap m_entries;
    void SetEntry(const CCharString& value, const CCharString& key) { m_entries[key] = value; }
};
int main()
{
    CStringTable t; t.m_entries.used = 0;
    CCharString k, v; std::strcpy(k.text, "KEY"); std::strcpy(v.text, "VALUE");
    t.SetEntry(v, k);
    bool ok = t.m_entries.used == 1 && !std::strcmp(t.m_entries.keys[0].text, "KEY") && !std::strcmp(t.m_entries.values[0].text, "VALUE");
    std::strcpy(v.text, "OTHER");
    t.SetEntry(v, k);
    ok = ok && t.m_entries.used == 1 && !std::strcmp(t.m_entries.values[0].text, "OTHER");
    if (!ok) { std::printf("009a8150_TEST FAIL\n"); return 1; }
    std::printf("009a8150_TEST PASS\n");
    return 0;
}