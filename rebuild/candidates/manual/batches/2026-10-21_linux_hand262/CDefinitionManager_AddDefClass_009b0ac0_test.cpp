// behaviour test for 009b0ac0 CDefinitionManager::AddDefClass
#include <cstdio>
#include <cstring>
static unsigned long Crc(const char* s, unsigned long n)
{
    unsigned long h = 0;
    for (unsigned long i = 0; i < n; ++i)
        h = h * 31 + (unsigned char)s[i];
    return h;
}
struct Pair { unsigned long key; const char* name; };
struct Manager {
    Pair map[8]; int count; bool unsorted; int sorts;
    char instantiated[8][32]; int instCount; int persisted;
};
static void Sort(Manager& m)
{
    if (m.unsorted) {
        for (int i = 1; i < m.count; ++i)
            for (int j = i; j > 0 && m.map[j].key < m.map[j - 1].key; --j) { Pair t = m.map[j]; m.map[j] = m.map[j - 1]; m.map[j - 1] = t; }
        m.unsorted = false;
        ++m.sorts;
    }
}
static void AddDefClass(Manager& m, const char* name, bool compiling, bool loaded)
{
    const char* s = name == 0 ? "" : name;
    unsigned long len = name == 0 ? 0 : (unsigned long)std::strlen(name);
    m.map[m.count].key = Crc(s, len); m.map[m.count].name = name; ++m.count;
    m.unsorted = true;
    Sort(m);
    if (compiling || !loaded) {
        std::strcpy(m.instantiated[m.instCount], "NULLDEF_");
        std::strcat(m.instantiated[m.instCount], s);
        ++m.instCount;
        ++m.persisted;
    }
}
int main()
{
    int bad = 0;
    Manager m; std::memset(&m, 0, sizeof(m));
    AddDefClass(m, "ZEBRA", false, true);
    AddDefClass(m, "APPLE", true, true);
    AddDefClass(m, "MANGO", false, false);
    if (m.count != 3 || m.sorts != 3) bad |= 1;
    if (!(m.map[0].key <= m.map[1].key && m.map[1].key <= m.map[2].key)) bad |= 2;
    if (m.instCount != 2 || std::strcmp(m.instantiated[0], "NULLDEF_APPLE") != 0 || std::strcmp(m.instantiated[1], "NULLDEF_MANGO") != 0) bad |= 4;
    if (m.persisted != 2) bad |= 8;
    if (bad) { std::printf("009b0ac0_TEST FAIL\n"); return 1; }
    std::printf("009b0ac0_TEST PASS\n");
    return 0;
}
