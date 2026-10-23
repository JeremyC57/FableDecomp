#include <cstdio>
#include <cstring>
// behaviour test: CRuntimeQuestPersistence::LoadFromGameStateScript -- every START_NEW_QUEST <name> <script>
// END_NEW_QUEST block lands in the quest map and every master-data block in the other map, each sorted by name
struct Entry { char name[32]; char script[64]; };
struct Map { Entry e[8]; int n; bool dirty; };
static void SortMap(Map& m)
{
    for (int i = 0; i < m.n; ++i)
        for (int j = i + 1; j < m.n; ++j)
            if (std::strcmp(m.e[j].name, m.e[i].name) < 0) { Entry t = m.e[i]; m.e[i] = m.e[j]; m.e[j] = t; }
}
static const char* SkipWs(const char* p) { while (*p == ' ' || *p == '\n') ++p; return p; }
static void LoadBlocks(const char* text, const char* start, const char* end, Map& m)
{
    const char* p = std::strstr(text, start);
    while (p) {
        p = SkipWs(p + std::strlen(start));
        Entry& e = m.e[m.n];
        int k = 0;
        while (*p && *p != ' ' && *p != '\n') e.name[k++] = *p++;
        e.name[k] = 0;
        p = SkipWs(p);
        const char* q = std::strstr(p, end);
        k = 0;
        while (p < q) e.script[k++] = *p++;
        e.script[k] = 0;
        ++m.n; m.dirty = true;
        p = std::strstr(q, start);
    }
}
int main()
{
    const char* text = "junk START_NEW_QUEST Q_B b=1 END_NEW_QUEST START_NEW_QUEST Q_A a=2 END_NEW_QUEST "
                       "START_NEW_MASTER_QUEST_DATA M_X x=3 END_NEW_MASTER_QUEST_DATA";
    Map quests; quests.n = 0; quests.dirty = false;
    Map master; master.n = 0; master.dirty = false;
    LoadBlocks(text, "START_NEW_QUEST", "END_NEW_QUEST", quests);
    LoadBlocks(text, "START_NEW_MASTER_QUEST_DATA", "END_NEW_MASTER_QUEST_DATA", master);
    if (quests.dirty) { SortMap(quests); quests.dirty = false; }
    if (master.dirty) { SortMap(master); master.dirty = false; }
    if (quests.n != 2 || master.n != 1 || std::strcmp(quests.e[0].name, "Q_A") != 0 || std::strcmp(quests.e[1].script, "b=1 ") != 0
        || std::strcmp(master.e[0].name, "M_X") != 0) {
        std::printf("004b5080_TEST FAIL\n"); return 1;
    }
    std::printf("004b5080_TEST PASS\n");
    return 0;
}
